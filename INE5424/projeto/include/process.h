// EPOS Thread Component Declarations

#ifndef __process_h
#define __process_h

#include <architecture.h>
#include <machine.h>
#include <utility/queue.h>
#include <utility/handler.h>
#include <scheduler.h>

extern "C" { void __exit(); }

__BEGIN_SYS

class Thread
{
    friend class Init_End;              // context->load()
    friend class Init_System;           // for init() on CPU != 0
    friend class Scheduler<Thread>;     // for link()
    friend class Synchronizer_Common;   // for lock() and sleep()
    friend class Alarm;                 // for lock()
    friend class System;                // for init()
    friend class IC;                    // for link() for priority ceiling

protected:
    static const bool preemptive = Traits<Thread>::Criterion::preemptive;
    static const bool reboot = Traits<System>::reboot;
    static const bool multitask = Traits<System>::multitask;

    static const unsigned int QUANTUM = Traits<Thread>::QUANTUM;
    static const unsigned int STACK_SIZE = Traits<Application>::STACK_SIZE;

    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Context Context;

public:
    // Thread State
    enum State {
        RUNNING,
        READY,
        SUSPENDED,
        WAITING,
        FINISHING
    };

    // Thread Scheduling Criterion
    typedef Traits<Thread>::Criterion Criterion;
    enum {
        HIGH    = Criterion::HIGH,
        NORMAL  = Criterion::NORMAL,
        LOW     = Criterion::LOW,
        MAIN    = Criterion::MAIN,
        IDLE    = Criterion::IDLE
    };

    // Thread Queue
    typedef Ordered_Queue<Thread, Criterion, Scheduler<Thread>::Element> Queue;

    // Thread Configuration
    struct Configuration {
        Configuration(const State & s = READY, const Criterion & c = NORMAL, unsigned int ss = STACK_SIZE, Task * t = 0)
        : state(s), criterion(c), stack_size(ss), task(t) {}

        State state;
        Criterion criterion;
        unsigned int stack_size;
        Task * task;
    };


public:
    template<typename ... Tn>
    Thread(int (* entry)(Tn ...), Tn ... an);
    template<typename ... Tn>
    Thread(const Configuration & conf, int (* entry)(Tn ...), Tn ... an);
    ~Thread();

    const volatile State & state() const { return _state; }
    const volatile Criterion::Statistics & statistics() { return criterion().statistics(); }

    const volatile Criterion & priority() const { return _link.rank(); }
    void priority(const Criterion & p);

    int join();
    void pass();
    void suspend();
    void resume();

    static Thread * volatile self() { return running(); }
    Task * task() const { return _task; }
    static void yield();
    static void exit(int status = 0);

protected:
    void constructor_prologue(unsigned int stack_size);
    void constructor_epilogue(Log_Addr entry, unsigned int stack_size);

    Criterion & criterion() { return const_cast<Criterion &>(_link.rank()); }
    Queue::Element * link() { return &_link; }

    static Thread * volatile running() { return _scheduler.chosen(); }

    static void lock() { CPU::int_disable(); }
    static void unlock() { CPU::int_enable(); }
    static bool locked() { return CPU::int_disabled(); }

    static void sleep(Queue * q);
    static void wakeup(Queue * q);
    static void wakeup_all(Queue * q);

    static void reschedule();
    static void time_slicer(IC::Interrupt_Id interrupt);

    static void dispatch(Thread * prev, Thread * next, bool charge = true);

    static int idle();

private:
    static void init();

protected:
    char * _stack;
    Task * _task;
    Segment  * _ustack;
    Context * volatile _context;
    volatile State _state;
    Queue * _waiting;
    Thread * volatile _joining;
    Queue::Element _link;

    static volatile unsigned int _thread_count;
    static Scheduler_Timer * _timer;
    static Scheduler<Thread> _scheduler;
};

 // Task (only used in multitasking configurations)
class Task
{
    friend class Thread;

private:
    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::Context Context;

protected:
    template<typename ... Tn>
    Task(Address_Space * as, Segment * cs, Segment * ds, int (* entry)(Tn ...), const Log_Addr & code, const Log_Addr & data, Tn ... an)
    : _as(as), _cs(cs), _ds(ds), _entry(entry), _code(code), _data(data) {
        db<Task, Init>(TRC) << "Task(as=" << _as << ",cs=" << _cs << ",ds=" << _ds << ",entry=" << _entry << ",code=" << _code << ",data=" << _data << ") => " << this << endl;
        lock();
        _id = _task_count++;
        unlock();
        _current = this;
        activate();
        _main = new (SYSTEM) Thread(Thread::Configuration(Thread::RUNNING, Thread::MAIN, Traits<Application>::STACK_SIZE, this), entry, an ...);
    }

public:
    template<typename ... Tn>
    Task(Segment * cs, Segment * ds, int (* entry)(Tn ...), const Log_Addr & code, const Log_Addr & data, Tn ... an)
    : _as (new (SYSTEM) Address_Space), _cs(cs), _ds(ds), _entry(entry), _code(_as->attach(_cs, code)), _data(_as->attach(_ds, data)) {
        db<Task>(TRC) << "Task(as=" << _as << ",cs=" << _cs << ",ds=" << _ds << ",entry=" << _entry << ",code=" << _code << ",data=" << _data << ") => " << this << endl;
        lock();
        _id = _task_count++;
        unlock();
        _main = new (SYSTEM) Thread(Thread::Configuration(Thread::READY, Thread::MAIN, Traits<Application>::STACK_SIZE, this), entry, an ...);
    }
    ~Task();

    Address_Space * address_space() const { return _as; }

    Segment * code_segment() const { return _cs; }
    Segment * data_segment() const { return _ds; }

    Log_Addr code() const { return _code; }
    Log_Addr data() const { return _data; }

    Thread * main() const { return _main; }

    static Task * volatile self() { return current(); }
    Log_Addr entry() { return _entry; }

    unsigned int id() {return _id;}

    void activate_context() {
        activate();
        MMU::flush_tlb();
        lock();
        _current = this;
        unlock();
    }

private:
    void activate() const { _as->activate(); }

    void insert(Thread * t) { _threads.insert(new (SYSTEM) Thread::Queue::Element(t)); }
    void remove(Thread * t) { Thread::Queue::Element * el = _threads.remove(t); if(el) delete el; }

    static Task * volatile current() { return _current; }
    static void current(Task * t) { _current = t; }

    static void lock() { CPU::int_disable(); }
    static void unlock() { CPU::int_enable(); }

private:
    unsigned int _id;
    Address_Space * _as;
    Segment * _cs;
    Segment * _ds;
    Log_Addr _entry;
    Log_Addr _code;
    Log_Addr _data;
    Thread * _main;
    Thread::Queue _threads;

    static Task * volatile _current;

protected:
    static volatile unsigned int _task_count;
};


// A Java-like Active Object
class Active: public Thread
{
public:
    Active(): Thread(Configuration(Thread::SUSPENDED), &entry, this) {}
    virtual ~Active() {}

    virtual int run() = 0;

    void start() { resume(); }

private:
    static int entry(Active * runnable) { return runnable->run(); }
};


// An event handler that triggers a thread (see handler.h)
class Thread_Handler : public Handler
{
public:
    Thread_Handler(Thread * h) : _handler(h) {}
    ~Thread_Handler() {}

    void operator()() { _handler->resume(); }

private:
    Thread * _handler;
};

template<typename ... Tn>
inline Thread::Thread(int (* entry)(Tn ...), Tn ... an)
:_task(Task::self()), _state(READY), _waiting(0), _joining(0), _link(this, NORMAL)
{
    constructor_prologue(STACK_SIZE);
    _context = CPU::init_stack(0, _stack + STACK_SIZE, &__exit, entry, an ...);
    constructor_epilogue(entry, STACK_SIZE);

    _task->insert(this);
}

template<typename ... Tn>
inline Thread::Thread(const Configuration & conf, int (* entry)(Tn ...), Tn ... an)
:_task(conf.task ? conf.task : Task::self()), _state(conf.state), _waiting(0), _joining(0), _link(this, conf.criterion)
{
    constructor_prologue(conf.stack_size);

    if (conf.criterion == Thread::IDLE) {
        _context = CPU::init_stack(0, _stack + conf.stack_size, &__exit, entry, an ...);
    } else {
        _ustack = new (SYSTEM) Segment(Traits<Machine>::STACK_SIZE, Segment::Flags::APP);
        CPU::Log_Addr usp = _task->address_space()->attach(_ustack);
        db<Thread>(TRC) << "UStack attached at vaddr=" << usp << endl;
        _context = CPU::init_user_stack(usp + Traits<Machine>::STACK_SIZE, _stack + Traits<Machine>::STACK_SIZE, &__exit, entry, an ...);
        db<Thread>(TRC) << "Context attached at vaddr=" << hex << _context << endl;
    }
    constructor_epilogue(entry, conf.stack_size);
    if (conf.criterion != Thread::IDLE) {
        _task->insert(this);
    }
}

__END_SYS

#endif
