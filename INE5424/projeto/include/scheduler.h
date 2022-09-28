// EPOS Scheduler Component Declarations

#ifndef __scheduler_h
#define __scheduler_h

#include <architecture/cpu.h>
#include <architecture/pmu.h>
#include <architecture/tsc.h>
#include <utility/scheduling.h>
#include <utility/math.h>
#include <utility/convert.h>

__BEGIN_SYS

// All scheduling criteria, or disciplines, must define operator int() with
// the semantics of returning the desired order of a given object within the
// scheduling list
class Scheduling_Criterion_Common
{
    friend class _SYS::Thread;
    friend class _SYS::Periodic_Thread;
    friend class _SYS::RT_Thread;
    friend class _SYS::Clerk<System>;         // for _statistics

public:
    // Priorities
    enum : int {
        MAIN   = 0,
        HIGH   = 1,
        NORMAL = (unsigned(1) << (sizeof(int) * 8 - 1)) - 3,
        LOW    = (unsigned(1) << (sizeof(int) * 8 - 1)) - 2,
        IDLE   = (unsigned(1) << (sizeof(int) * 8 - 1)) - 1
    };

    // Constructor helpers
    enum : unsigned int {
        SAME        = 0,
        NOW         = 0,
        UNKNOWN     = 0,
        ANY         = -1U
    };

    // Policy types
    enum : int {
        PERIODIC    = HIGH,
        APERIODIC   = NORMAL,
        SPORADIC    = NORMAL
    };

    // Policy traits
    static const bool timed = false;
    static const bool dynamic = false;
    static const bool preemptive = true;
    static const bool collecting = false;
    static const bool charging = false;
    static const bool awarding = false;
    static const bool migrating = false;
    static const bool track_idle = false;
    static const bool task_wide = false;
    static const bool cpu_wide = false;
    static const bool system_wide = false;
    static const unsigned int QUEUES = 1;

    // Runtime Statistics (for policies that don't use any; that´s why its a union)
    union Statistics {
        // Thread Execution Time
        TSC::Time_Stamp thread_execution_time;  // accumulated thread execution time
        TSC::Time_Stamp last_thread_dispatch;   // time stamp of last dispatch

        // Deadline Miss count - Used By Clerk
        Alarm * alarm_times;                    // pointer to RT_Thread private alarm (for monitoring purposes)
        unsigned int finished_jobs;             // number of finished jobs given by the number of times alarm->p() was called for this thread
        unsigned int missed_deadlines;          // number of missed deadlines given by the number of finished jobs (finished_jobs) minus the number of dispatched jobs (alarm_times->times)

        // CPU Execution Time (capture ts)
        static TSC::Time_Stamp _cpu_time[Traits<Build>::CPUS];              // accumulated CPU time in the current hyperperiod for each CPU
        static TSC::Time_Stamp _last_dispatch_time[Traits<Build>::CPUS];    // time Stamp of last dispatch in each CPU
        static TSC::Time_Stamp _last_activation_time;                       // global time stamp of the last heuristic activation
    };

protected:
    Scheduling_Criterion_Common() {}

public:
    const Microsecond period() { return 0;}
    void period(const Microsecond & p) {}

    unsigned int queue() const { return 0; }
    void queue(unsigned int q) {}

    bool update() { return false; }

    bool collect(bool end = false) { return false; }
    bool charge(bool end = false) { return true; }
    bool award(bool end = false) { return true; }

    volatile Statistics & statistics() { return _statistics; }

    static void init() {}

protected:
    Statistics _statistics;
};

// Priority (static and dynamic)
class Priority: public Scheduling_Criterion_Common
{
    friend class _SYS::Thread;
    friend class _SYS::Periodic_Thread;
    friend class _SYS::RT_Thread;

public:
    template <typename ... Tn>
    Priority(int p = NORMAL, Tn & ... an): _priority(p) {}

    operator const volatile int() const volatile { return _priority; }

protected:
    volatile int _priority;
};

// Round-Robin
class RR: public Priority
{
public:
    static const bool timed = true;
    static const bool dynamic = false;
    static const bool preemptive = true;

public:
    template <typename ... Tn>
    RR(int p = NORMAL, Tn & ... an): Priority(p) {}
};

// First-Come, First-Served (FIFO)
class FCFS: public Priority
{
public:
    static const bool timed = false;
    static const bool dynamic = false;
    static const bool preemptive = false;

public:
    template <typename ... Tn>
    FCFS(int p = NORMAL, Tn & ... an);
};


class Real_Time_Scheduler: public Priority
{
protected:
    Real_Time_Scheduler(int p): Priority(p), _deadline(0), _period(0), _capacity(0) {}
    Real_Time_Scheduler(int i, const Microsecond & d, const Microsecond & p, const Microsecond & c)
    : Priority(i), _deadline(d), _period(p), _capacity(c) {}

public:
    const Microsecond period() { return _period; }
    void period(const Microsecond & p) { _period = p; }

public:
    Microsecond _deadline;
    Microsecond _period;
    Microsecond _capacity;
};

// Deadline Monotonic
class DM: public Real_Time_Scheduler
{

public:
    static const bool timed = false;
    static const bool dynamic = false;
    static const bool preemptive = true;

public:
    DM(int p = APERIODIC): Real_Time_Scheduler(p) {}
    DM(const Microsecond & d, const Microsecond & p = SAME, const Microsecond & c = UNKNOWN, unsigned int cpu = ANY)
    : Real_Time_Scheduler(d, d, p, c) {}
};

__END_SYS

#endif
