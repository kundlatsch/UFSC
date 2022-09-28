// EPOS Thread Implementation

#include <machine.h>
#include <system.h>
#include <process.h>

// This_Thread class attributes
__BEGIN_UTIL
bool This_Thread::_not_booting;
__END_UTIL

__BEGIN_SYS

volatile unsigned int Thread::_thread_count;
Scheduler_Timer * Thread::_timer;
Scheduler<Thread> Thread::_scheduler;


void Thread::constructor_prologue(unsigned int stack_size)
{
    lock();

    _thread_count++;
    _scheduler.insert(this);

    _stack = new (SYSTEM) char[stack_size];
}


void Thread::constructor_epilogue(Log_Addr entry, unsigned int stack_size)
{
    db<Thread>(TRC) << "Thread(entry=" << entry
                    << ",state=" << _state
                    << ",priority=" << _link.rank()
                    << ",stack={b=" << reinterpret_cast<void *>(_stack)
                    << ",s=" << stack_size
                    << "},context={b=" << _context
                    << "," << *_context << "}) => " << this << endl;

    assert((_state != WAITING) && (_state != FINISHING)); // invalid states

    if((_state != READY) && (_state != RUNNING))
        _scheduler.suspend(this);

    if(preemptive && (_state == READY) && (_link.rank() != IDLE))
        reschedule();

    unlock();
}


Thread::~Thread()
{
    lock();

    db<Thread>(TRC) << "~Thread(this=" << this
                    << ",state=" << _state
                    << ",priority=" << _link.rank()
                    << ",stack={b=" << reinterpret_cast<void *>(_stack)
                    << ",context={b=" << _context
                    << "," << *_context << "})" << endl;

    // The running thread cannot delete itself!
    assert(_state != RUNNING);

    switch(_state) {
    case RUNNING:  // For switch completion only: the running thread would have deleted itself! Stack wouldn't have been released!
        exit(-1);
        break;
    case READY:
        _scheduler.remove(this);
        _thread_count--;
        break;
    case SUSPENDED:
        _scheduler.resume(this);
        _scheduler.remove(this);
        _thread_count--;
        break;
    case WAITING:
        _waiting->remove(this);
        _scheduler.resume(this);
        _scheduler.remove(this);
        _thread_count--;
        break;
    case FINISHING: // Already called exit()
        break;
    }

    if(_joining)
        _joining->resume();
    
    _task->remove(this);

    unlock();

    delete _stack;
}


void Thread::priority(const Criterion & c)
{
    lock();

    db<Thread>(TRC) << "Thread::priority(this=" << this << ",prio=" << c << ")" << endl;

    if(_state != RUNNING) { // reorder the scheduling queue
        _scheduler.remove(this);
        _link.rank(c);
        _scheduler.insert(this);
    } else
        _link.rank(c);

    if(preemptive)
        reschedule();

    unlock();
}


int Thread::join()
{
    lock();

    db<Thread>(TRC) << "Thread::join(this=" << this << ",state=" << _state << ")" << endl;

    // Precondition: no Thread::self()->join()
    assert(running() != this);

    // Precondition: a single joiner
    assert(!_joining);

    if(_state != FINISHING) {
        Thread * prev = running();

        _joining = prev;
        prev->_state = SUSPENDED;
        _scheduler.suspend(prev); // implicitly choose() if suspending chosen()

        Thread * next = _scheduler.chosen();

        dispatch(prev, next);
    }

    unlock();

    return *reinterpret_cast<int *>(_stack);
}


void Thread::pass()
{
    lock();

    db<Thread>(TRC) << "Thread::pass(this=" << this << ")" << endl;

    Thread * prev = running();
    Thread * next = _scheduler.choose(this);

    if(next)
        dispatch(prev, next, false);
    else
        db<Thread>(WRN) << "Thread::pass => thread (" << this << ") not ready!" << endl;

    unlock();
}


void Thread::suspend()
{
    lock();

    db<Thread>(TRC) << "Thread::suspend(this=" << this << ")" << endl;

    Thread * prev = running();

    _state = SUSPENDED;
    _scheduler.suspend(this);

    Thread * next = _scheduler.chosen();

    dispatch(prev, next);

    unlock();
}


void Thread::resume()
{
    lock();

    db<Thread>(TRC) << "Thread::resume(this=" << this << ")" << endl;

    if(_state == SUSPENDED) {
        _state = READY;
        _scheduler.resume(this);

        if(preemptive)
            reschedule();
    } else
        db<Thread>(WRN) << "Resume called for unsuspended object!" << endl;

    unlock();
}


// Class methods
void Thread::yield()
{
    lock();

    db<Thread>(TRC) << "Thread::yield(running=" << running() << ")" << endl;

    Thread * prev = running();
    Thread * next = _scheduler.choose_another();

    dispatch(prev, next);

    unlock();
}


void Thread::exit(int status)
{
    lock();

    db<Thread>(TRC) << "Thread::exit(status=" << status << ") [running=" << running() << "]" << endl;

    Thread * prev = running();
    _scheduler.remove(prev);
    prev->_state = FINISHING;
    *reinterpret_cast<int *>(prev->_stack) = status;

    _thread_count--;

    if(prev->_joining) {
        prev->_joining->_state = READY;
        _scheduler.resume(prev->_joining);
        prev->_joining = 0;
    }

    Thread * next = _scheduler.choose(); // at least idle will always be there

    dispatch(prev, next);

    unlock();
}


void Thread::sleep(Queue * q)
{
    db<Thread>(TRC) << "Thread::sleep(running=" << running() << ",q=" << q << ")" << endl;

    assert(locked()); // locking handled by caller

    Thread * prev = running();
    _scheduler.suspend(prev);
    prev->_state = WAITING;
    prev->_waiting = q;
    q->insert(&prev->_link);

    Thread * next = _scheduler.chosen();

    dispatch(prev, next);
}


void Thread::wakeup(Queue * q)
{
    db<Thread>(TRC) << "Thread::wakeup(running=" << running() << ",q=" << q << ")" << endl;

    assert(locked()); // locking handled by caller

    if(!q->empty()) {
        Thread * t = q->remove()->object();
        t->_state = READY;
        t->_waiting = 0;
        _scheduler.resume(t);

        if(preemptive)
            reschedule();
    }
}


void Thread::wakeup_all(Queue * q)
{
    db<Thread>(TRC) << "Thread::wakeup_all(running=" << running() << ",q=" << q << ")" << endl;

    assert(locked()); // locking handled by caller

    if(!q->empty()) {
        while(!q->empty()) {
            Thread * t = q->remove()->object();
            t->_state = READY;
            t->_waiting = 0;
            _scheduler.resume(t);
        }

        if(preemptive)
            reschedule();
    }
}


void Thread::reschedule()
{
    if(!Criterion::timed || Traits<Thread>::hysterically_debugged)
        db<Thread>(TRC) << "Thread::reschedule()" << endl;

    assert(locked()); // locking handled by caller

    Thread * prev = running();
    Thread * next = _scheduler.choose();

    dispatch(prev, next);
}


void Thread::time_slicer(IC::Interrupt_Id i)
{
    lock();
    reschedule();
    unlock();
}


void Thread::dispatch(Thread * prev, Thread * next, bool charge)
{
    // "next" is not in the scheduler's queue anymore. It's already "chosen"

    if(charge) {
        if(Criterion::timed)
            _timer->restart();
    }

    if(prev != next) {
        if(prev->_state == RUNNING)
            prev->_state = READY;
        next->_state = RUNNING;

        db<Thread>(TRC) << "Thread::dispatch(prev=" << prev << ",next=" << next << ")" << endl;
        if(Traits<Thread>::hysterically_debugged) {
            CPU::Context tmp;
            tmp.save();
            db<Thread>(INF) << "Thread::dispatch:prev={" << prev << ",ctx=" << tmp << "}" << endl;
        }
        db<Thread>(INF) << "Thread::dispatch:next={" << next << ",ctx=" << *next->_context << "}" << endl;

        if (prev->_task != next->_task) {
            next->_task->activate_context();
        }

        // The non-volatile pointer to volatile pointer to a non-volatile context is correct
        // and necessary because of context switches, but here, we are locked() and
        // passing the volatile to switch_constext forces it to push prev onto the stack,
        // disrupting the context (it doesn't make a difference for Intel, which already saves
        // parameters on the stack anyway).
        CPU::switch_context(const_cast<Context **>(&prev->_context), next->_context);
    }
}


int Thread::idle()
{
    db<Thread>(TRC) << "Thread::idle(this=" << running() << ")" << endl;

    while(_thread_count > 1) { // someone else besides idle
        if(Traits<Thread>::trace_idle)
            db<Thread>(TRC) << "Thread::idle(this=" << running() << ")" << endl;

        CPU::int_enable();
        CPU::halt();
    }

    CPU::int_disable();
    db<Thread>(WRN) << "The last thread has exited!" << endl;
    if(reboot) {
        db<Thread>(WRN) << "Rebooting the machine ..." << endl;
        Machine::reboot();
    } else {
        db<Thread>(WRN) << "Halting the machine ..." << endl;
        CPU::halt();
    }

    // Some machines will need a little time to actually reboot
    for(;;);

    return 0;
}

__END_SYS

// Id forwarder to the spin lock
__BEGIN_UTIL

volatile CPU::Reg This_Thread::id()
{
    return _not_booting ? CPU::Reg(Thread::self()) : CPU::Reg(CPU::id() + 1);
}

__END_UTIL
