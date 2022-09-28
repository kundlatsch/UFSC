// EPOS Real-time Declarations

#ifndef __real_time_h
#define __real_time_h

#include <utility/handler.h>
#include <utility/math.h>
#include <utility/convert.h>
#include <time.h>
#include <process.h>
#include <synchronizer.h>

__BEGIN_SYS

// Aperiodic Thread
typedef Thread Aperiodic_Thread;

// Periodic threads are achieved by programming an alarm handler to invoke
// p() on a control semaphore after each job (i.e. task activation). Base
// threads are created in BEGINNING state, so the scheduler won't dispatch
// them before the associate alarm and semaphore are created. The first job
// is dispatched by resume() (thus the _state = SUSPENDED statement)

// Periodic Thread
class Periodic_Thread: public Thread
{
public:
    enum {
        SAME    = Real_Time_Scheduler_Common::SAME,
        NOW     = Real_Time_Scheduler_Common::NOW,
        UNKNOWN = Real_Time_Scheduler_Common::UNKNOWN,
        ANY     = Real_Time_Scheduler_Common::ANY
    };

protected:
    // Alarm Handler for periodic threads under static scheduling policies
    class Static_Handler: public Semaphore_Handler
    {
    public:
        Static_Handler(Semaphore * s, Periodic_Thread * t): Semaphore_Handler(s) {}
        ~Static_Handler() {}
    };

    // Alarm Handler for periodic threads under dynamic scheduling policies
    class Dynamic_Handler: public Semaphore_Handler
    {
    public:
        Dynamic_Handler(Semaphore * s, Periodic_Thread * t): Semaphore_Handler(s), _thread(t) {}
        ~Dynamic_Handler() {}

        void operator()() {
            _thread->criterion().update();

            Semaphore_Handler::operator()();
        }

    private:
        Periodic_Thread * _thread;
    };

    typedef IF<Criterion::dynamic, Dynamic_Handler, Static_Handler>::Result Handler;

public:
    struct Configuration: public Thread::Configuration {
        Configuration(const Microsecond & p, const Microsecond & d = SAME, const Microsecond & cap = UNKNOWN, const Microsecond & act = NOW, const unsigned int n = INFINITE, const State & s = READY, const Criterion & c = NORMAL, unsigned int ss = STACK_SIZE)
        : Thread::Configuration(s, c, ss), period(p), deadline(d == SAME ? p : d), capacity(cap), activation(act), times(n) {}

        Microsecond period;
        Microsecond deadline;
        Microsecond capacity;
        Microsecond activation;
        unsigned int times;
    };

public:
    template<typename ... Tn>
    Periodic_Thread(const Microsecond & p, int (* entry)(Tn ...), Tn ... an)
    : Thread(Thread::Configuration(SUSPENDED, Criterion(p)), entry, an ...),
      _semaphore(0), _handler(&_semaphore, this), _alarm(p, &_handler, INFINITE) { resume(); }

    template<typename ... Tn>
    Periodic_Thread(const Configuration & conf, int (* entry)(Tn ...), Tn ... an)
    : Thread(Thread::Configuration(SUSPENDED, (conf.criterion != NORMAL) ? conf.criterion : Criterion(conf.period), conf.stack_size), entry, an ...),
      _semaphore(0), _handler(&_semaphore, this), _alarm(conf.period, &_handler, conf.times) {
        if((conf.state == READY) || (conf.state == RUNNING)) {
            _state = SUSPENDED;
            resume();
        } else
            _state = conf.state;
    }

    const Microsecond & period() const { return _alarm.period(); }
    void period(const Microsecond & p) { _alarm.period(p); }

    static volatile bool wait_next() {
        Periodic_Thread * t = reinterpret_cast<Periodic_Thread *>(running());

        db<Thread>(TRC) << "Thread::wait_next(this=" << t << ",times=" << t->_alarm.times() << ")" << endl;

        if(t->_alarm.times())
            t->_semaphore.p();

        return t->_alarm.times();
    }

protected:
    Semaphore _semaphore;
    Handler _handler;
    Alarm _alarm;
};

typedef Periodic_Thread::Configuration RTConf;

__END_SYS

#endif