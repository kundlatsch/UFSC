// EPOS Common Types and Type Management System

typedef __SIZE_TYPE__ size_t;

#ifndef __types_h
#define __types_h

__BEGIN_API

// Memory allocators
enum System_Allocator { SYSTEM };
enum Shared_Allocator { SHARED };
enum Scratchpad_Allocator { SCRATCHPAD };
enum Color {
    COLOR_0,  COLOR_1,  COLOR_2,  COLOR_3,  COLOR_4,  COLOR_5,  COLOR_6,  COLOR_7,
    COLOR_8,  COLOR_9,  COLOR_10, COLOR_11, COLOR_12, COLOR_13, COLOR_14, COLOR_15,
    COLOR_16, COLOR_17, COLOR_18, COLOR_19, COLOR_20, COLOR_21, COLOR_22, COLOR_23,
    COLOR_24, COLOR_25, COLOR_26, COLOR_27, COLOR_28, COLOR_29, COLOR_30, COLOR_31,
    WHITE = COLOR_0
};

// Power Management Modes
enum Power_Mode
{
    ENROLL,
    DISMISS,
    SAME,
    FULL,
    LIGHT,
    SLEEP,
    OFF
};

__END_API

extern "C"
{
    void * malloc(size_t);
    void free(void *);
}

inline void * operator new(size_t s, void * a) { return a; }
inline void * operator new[](size_t s, void * a) { return a; }

void * operator new(size_t, const EPOS::System_Allocator &);
void * operator new[](size_t, const EPOS::System_Allocator &);

void * operator new(size_t, const EPOS::Shared_Allocator &);
void * operator new[](size_t, const EPOS::Shared_Allocator &);

void * operator new(size_t, const EPOS::Scratchpad_Allocator &);
void * operator new[](size_t, const EPOS::Scratchpad_Allocator &);

void * operator new(size_t, const EPOS::Color &);
void * operator new[](size_t, const EPOS::Color &);

// Utilities
__BEGIN_UTIL

template<int BITS> class Padding {} __attribute__((packed));
template<> class Padding<8>  { char _padding;          } __attribute__((packed));
template<> class Padding<16> { short int _padding;     } __attribute__((packed));
template<> class Padding<32> { long int _padding;      } __attribute__((packed));
template<> class Padding<64> { long long int _padding; } __attribute__((packed));

typedef unsigned char Percent;
typedef unsigned char UUID[8];

template <typename ... Tn> struct Dummy {
    Dummy(Tn ... an){}
    friend Debug & operator<<(Debug & db, const Dummy & d) { return db; };
};

__END_UTIL

__BEGIN_SYS

static const int MAX_INT = -1U/2;
static const long int MAX_LONG_INT = -1UL/2;
static const long long int MAX_LONG_LONG_INT = -1ULL/2;
static const unsigned MAX_UNSIGNED = -1U;
static const unsigned long MAX_UNSIGNED_LONG = -1UL;
static const unsigned long long MAX_UNSIGNED_LONG_LONG = -1ULL;

// Adjusts the basic time type to express time with a resolution of microseconds according to the system's life span,
// forcing a compilation error through void when a counter overflow becomes possible.
typedef IF<(Traits<System>::LIFE_SPAN * 1000000 <= MAX_UNSIGNED_LONG), unsigned long,
           IF<(Traits<System>::LIFE_SPAN * 1000000ULL <= MAX_UNSIGNED_LONG_LONG), unsigned long long,
              void>::Result>::Result Time_Base;


// The time (as defined by God Chronos)
class Second;
class Milisecond;
class Microsecond;

// Infinite times (for alarms and periodic threads)
enum Infinity : Time_Base { INFINITE = -1U };

class Second
{
public:
	typedef Time_Base Type;

public:
    Second() {};
    Second(const Infinity &): _time(INFINITE) {};
    Second(const Time_Base & time): _time(time) {}

    operator Time_Base() { return _time; }
    operator Time_Base() const { return _time; }
    operator Time_Base() volatile { return _time; }

private:
    Time_Base _time;
};

class Milisecond
{
public:
	typedef Time_Base Type;

public:
    Milisecond() {};
    Milisecond(const Infinity &): _time(INFINITE) {};
    Milisecond(const Time_Base & time): _time(time) {}
    Milisecond(const Second & time): _time(reinterpret_cast<const Time_Base &>(time) * 1000) {}

    operator Time_Base() { return _time; }
    operator Time_Base() const { return _time; }
    operator Time_Base() volatile { return _time; }

private:
    Time_Base _time;
};

class Microsecond
{
public:
	typedef Time_Base Type;

public:
    Microsecond() {};
    Microsecond(const Infinity &): _time(INFINITE) {};
    Microsecond(const Time_Base & time): _time(time) {}
    Microsecond(const Second & time): _time(reinterpret_cast<const Time_Base &>(time) * 1000000) {}
    Microsecond(const Milisecond & time): _time(reinterpret_cast<const Time_Base &>(time) * 1000) {}

    operator Time_Base() { return _time; }
    operator Time_Base() const { return _time; }
    operator Time_Base() volatile { return _time; }

private:
    Time_Base _time;
};

typedef unsigned long Hertz;
typedef unsigned long PPM; // parts per million
typedef unsigned long long PPB; // parts per billion

// System Components IDs
// The order in this enumeration defines many things in the system (e.g. init)
typedef unsigned int Type_Id;
enum
{
    FIRST_COMPONENT_ID = 0,
    THREAD_ID = FIRST_COMPONENT_ID,
    TASK_ID,
    ACTIVE_ID,
    ADDRESS_SPACE_ID,
    SEGMENT_ID,
    MUTEX_ID,
    SEMAPHORE_ID,
    CONDITION_ID,
    CLOCK_ID,
    ALARM_ID,
    CHRONOMETER_ID,
    UTILITY_ID,
    LAST_COMPONENT_ID,

    FIRST_MEDIATOR_ID = 100,
    CPU_ID = FIRST_MEDIATOR_ID,
    TSC_ID,
    MMU_ID,
    FPU_ID,
    PMU_ID,

    MACHINE_ID,
    PCI_ID,
    IC_ID,
    TIMER_ID,
    RTC_ID,
    EEPROM_ID,
    SCRATCHPAD_ID,
    UART_ID,
    DISPLAY_ID,
    KEYBOARD_ID,
    NIC_ID,
    LAST_MEDIATOR_ID,

    LAST_TYPE_ID = LAST_MEDIATOR_ID,

    UNKNOWN_TYPE_ID = 0xffff
};

// Type IDs for system components
template<typename T> struct Type { static const Type_Id ID = UNKNOWN_TYPE_ID; };

template<> struct Type<CPU> { static const Type_Id ID = CPU_ID; };
template<> struct Type<TSC> { static const Type_Id ID = TSC_ID; };
template<> struct Type<MMU> { static const Type_Id ID = MMU_ID; };
template<> struct Type<FPU> { static const Type_Id ID = FPU_ID; };
template<> struct Type<PMU> { static const Type_Id ID = PMU_ID; };

template<> struct Type<Machine> { static const Type_Id ID = MACHINE_ID; };
template<> struct Type<IC> { static const Type_Id ID = IC_ID; };
template<> struct Type<Timer> { static const Type_Id ID = TIMER_ID; };
template<> struct Type<UART> { static const Type_Id ID = UART_ID; };
template<> struct Type<RTC> { static const Type_Id ID = RTC_ID; };
template<> struct Type<PCI> { static const Type_Id ID = PCI_ID; };
template<> struct Type<Display> { static const Type_Id ID = DISPLAY_ID; };
template<> struct Type<Keyboard> { static const Type_Id ID = KEYBOARD_ID; };
template<> struct Type<Scratchpad> { static const Type_Id ID = SCRATCHPAD_ID; };
template<> struct Type<Ethernet> { static const Type_Id ID = NIC_ID; };
template<> struct Type<IEEE802_15_4> { static const Type_Id ID = NIC_ID; };

template<> struct Type<Thread> { static const Type_Id ID = THREAD_ID; };
template<> struct Type<Periodic_Thread> { static const Type_Id ID = THREAD_ID; };
template<> struct Type<RT_Thread> { static const Type_Id ID = THREAD_ID; };
template<> struct Type<Active> { static const Type_Id ID = ACTIVE_ID; };
template<> struct Type<Task> { static const Type_Id ID = TASK_ID; };

template<> struct Type<Address_Space> { static const Type_Id ID = ADDRESS_SPACE_ID; };
template<> struct Type<Segment> { static const Type_Id ID = SEGMENT_ID; };

template<> struct Type<Mutex> { static const Type_Id ID = MUTEX_ID; };
template<> struct Type<Semaphore> { static const Type_Id ID = SEMAPHORE_ID; };
template<> struct Type<Condition> { static const Type_Id ID = CONDITION_ID; };

template<> struct Type<Clock> { static const Type_Id ID = CLOCK_ID; };
template<> struct Type<Chronometer> { static const Type_Id ID = CHRONOMETER_ID; };
template<> struct Type<Alarm> { static const Type_Id ID = ALARM_ID; };
template<> struct Type<Delay> { static const Type_Id ID = ALARM_ID; };

template<> struct Type<Utility> { static const Type_Id ID = UTILITY_ID; };

// Type IDs for system components whose parameters are themselves components are defined where they are declared.

__END_SYS

#endif
