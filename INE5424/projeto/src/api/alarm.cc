// EPOS Alarm Implementation

#include <machine/display.h>
#include <synchronizer.h>
#include <time.h>
#include <process.h>

__BEGIN_SYS

Alarm_Timer * Alarm::_timer;
volatile Alarm::Tick Alarm::_elapsed;
Alarm::Queue Alarm::_request;

Alarm::Alarm(const Microsecond & time, Handler * handler, unsigned int times)
: _time(time), _handler(handler), _times(times), _ticks(ticks(time)), _link(this, _ticks)
{
    lock();

    db<Alarm>(TRC) << "Alarm(t=" << time << ",tk=" << _ticks << ",h=" << reinterpret_cast<void *>(handler) << ",x=" << times << ") => " << this << endl;

    if(_ticks) {
        _request.insert(&_link);
        unlock();
    } else {
        assert(times == 1);
        unlock();
        (*handler)();
    }
}

Alarm::~Alarm()
{
    lock();

    db<Alarm>(TRC) << "~Alarm(this=" << this << ")" << endl;

    _request.remove(this);

    unlock();
}

void Alarm::reset()
{
    bool locked = Thread::locked();
    if(!locked)
        lock();

    db<Alarm>(TRC) << "Alarm::reset(this=" << this << ")" << endl;

    _request.remove(this);
    _link.rank(_ticks);
    _request.insert(&_link);

    if(!locked)
        unlock();
}

void Alarm::period(const Microsecond & p)
{
    bool locked = Thread::locked();
    if(!locked)
        lock();

    db<Alarm>(TRC) << "Alarm::period(this=" << this << ",p=" << p << ")" << endl;

    _request.remove(this);
    _time = p;
    _ticks = ticks(p);
    _request.insert(&_link);

    if(!locked)
        unlock();
}


void Alarm::delay(const Microsecond & time)
{
    db<Alarm>(TRC) << "Alarm::delay(time=" << time << ")" << endl;

    Semaphore semaphore(0);
    Semaphore_Handler handler(&semaphore);
    Alarm alarm(time, &handler, 1); // if time < tick trigger v()
    semaphore.p();
}


void Alarm::handler(IC::Interrupt_Id i)
{
    lock();

    _elapsed++;

    if(Traits<Alarm>::visible) {
        Display display;
        int lin, col;
        display.position(&lin, &col);
        display.position(0, 79);
        display.putc(_elapsed);
        display.position(lin, col);
    }

    Alarm * alarm = 0;

    if(!_request.empty()) {
        // Replacing the following "if" by a "while" loop is tempting, but recovering the lock and dispatching the handler is
        // troublesome if the Alarm gets destroyed in between, like is the case for the idle thread returning to shutdown the machine
        if(_request.head()->promote() <= 0) { // rank can be negative whenever multiple handlers get created for the same time tick
            Queue::Element * e = _request.remove();
            alarm = e->object();
            if(alarm->_times != INFINITE)
                alarm->_times--;
            if(alarm->_times > 0) {
                e->rank(alarm->_ticks);
                _request.insert(e);
            }
        }
    }

    unlock();

    if(alarm) {
        db<Alarm>(TRC) << "Alarm::handler(this=" << alarm << ",e=" << _elapsed << ",h=" << reinterpret_cast<void*>(alarm->handler) << ")" << endl;
        (*alarm->_handler)();
    }
}

__END_SYS
