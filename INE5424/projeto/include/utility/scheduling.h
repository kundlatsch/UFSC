// EPOS Scheduling Class Utility

#ifndef __scheduling_h
#define __scheduling_h

#include <utility/list.h>

__BEGIN_UTIL

// All scheduling criteria, or disciplines, must define operator int() with
// the semantics of returning the desired order of a given object within the
// scheduling list

// Scheduling_Queue
template<typename T, typename R = typename T::Criterion>
class Scheduling_Queue: public Scheduling_List<T> {};


// Scheduler
// Objects subject to scheduling by Scheduler must declare a type "Criterion"
// that will be used as the scheduling queue sorting criterion (viz, through
// operators <, >, and ==) and must also define a method "link" to export the
// list element pointing to the object being handled.
template<typename T>
class Scheduler: public Scheduling_Queue<T>
{
private:
    typedef Scheduling_Queue<T> Base;

public:
    typedef typename T::Criterion Criterion;
    typedef Scheduling_List<T, Criterion> Queue;
    typedef typename Queue::Element Element;

public:
    Scheduler() {}

    unsigned int schedulables() { return Base::size(); }

    T * volatile chosen() {
    	// If called before insert(), chosen will dereference a null pointer!
    	// For threads, we assume this won't happen (see Init_End).
    	// But if you are unsure about your new use of the scheduler,
    	// please, pay the price of the extra "if" bellow.
        // Hysterically debugging also causes chosen() to be called before insert()
        if(Traits<Build>::hysterically_debugged || Traits<Thread>::trace_idle)
            return const_cast<T * volatile>((Base::chosen()) ? Base::chosen()->object() : 0);
        else
            return const_cast<T * volatile>(Base::chosen()->object());
    }

    void insert(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::insert(" << obj << ")" << endl;

        Base::insert(obj->link());
    }

    T * remove(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::remove(" << obj << ")" << endl;

        return Base::remove(obj->link()) ? obj : 0;
    }

    void suspend(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::suspend(" << obj << ")" << endl;

        Base::remove(obj->link());
    }

    void resume(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::resume(" << obj << ")" << endl;

        Base::insert(obj->link());
    }

    T * choose() {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::choose() => ";

        T * obj = Base::choose()->object();

        db<Scheduler>(TRC) << obj << endl;

        return obj;
    }

    T * choose_another() {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::choose_another() => ";

        T * obj = Base::choose_another()->object();

        db<Scheduler>(TRC) << obj << endl;

        return obj;
    }

    T * choose(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::choose(" << obj;

        if(!Base::choose(obj->link()))
            obj = 0;

        db<Scheduler>(TRC) << obj << endl;

        return obj;
    }
};

__END_UTIL

#endif
