// EPOS Condition Variable Implementation

#include <synchronizer.h>

// This is actually no Condition Variable
// check http://www.cs.duke.edu/courses/spring01/cps110/slides/sem/sld002.htm

__BEGIN_SYS

// Methods

Condition::Condition() {
    db<Synchronizer>(TRC) << "Condition() => " << this << endl;
}


Condition::~Condition() {
    db<Synchronizer>(TRC) << "~Condition(this=" << this << ")" << endl;
}


void Condition::wait() {
    db<Synchronizer>(TRC) << "Condition::wait(this=" << this << ")" << endl;

    begin_atomic();
    sleep();
    end_atomic();
}


void Condition::signal() {
    db<Synchronizer>(TRC) << "Condition::signal(this=" << this << ")" << endl;

    begin_atomic();
    wakeup();
    end_atomic();
}


void Condition::broadcast() {
    db<Synchronizer>(TRC) << "Condition::broadcast(this=" << this << ")" << endl;

    begin_atomic();
    wakeup_all();
    end_atomic();
}

// This is an alternative implementation, which does impose ordering
// on threads waiting at "wait". Nontheless, it's still susceptible to counter
// overflow
// class Condition: public Synchronizer_Common
// {
// private:
//     typedef Traits<Synchronizer> Traits;
//     static const Type_Id TYPE = Type<Condition>::TYPE;

// public:
//     Condition() : _wait(0), _signal(0) {
// 	db<Synchronizer>(TRC) << "Condition() => " << this << endl;
//     }
//     ~Condition() {
// 	db<Synchronizer>(TRC) << "~Condition(this=" << this << ")" << endl;
//     }

//     void wait() {
// 	db<Synchronizer>(TRC) << "Condition::wait(this=" << this
//  			      << ",wt=" << _wait
//  			      << ",sg=" << _signal << ")" << endl;
// 	int rank = finc(_wait);
// 	while(rank >= _signal)
// 	    sleep();
//     }
//     void signal() {
// 	db<Synchronizer>(TRC) << "Condition::signal(this=" << this
// 			      << ",wt=" << _wait
// 			      << ",sg=" << _signal << ")" << endl;
// 	finc(_signal);
// 	wakeup();
//     }
//     void broadcast() { // warning: overflow is not being handled!
// 	db<Synchronizer>(TRC) << "Condition::broadcast(this=" << this
// 			      << ",wt=" << _wait
// 			      << ",sg=" << _signal << ")" << endl;
// 	_signal = _wait + 1;
// 	wakeup_all();
//     }

// static int init(System_Info * si);

// private:
//     volatile int _wait;
//     volatile int _signal;
// };

// This is an alternative implementation, which does not impose any ordering
// on threads waiting at "wait". In this case, ordering would be implemented
// in "sleep" and "wakeup" through ordered queues.
// class Condition: public Synchronizer_Common
// {
// private:
//     typedef Traits<Condition> Traits;
//     static const Type_Id TYPE = Type<Condition>::TYPE;

// public:
//     Condition() : _not_condition(true), _broadcast(0), _time_stamp(1) {
// 	db<Condition>(TRC) << "Condition() => " << this << ")" << endl;
//     }
//     ~Condition() {
// 	db<Condition>(TRC) << "Condition() => " << this << ")" << endl;
//     }

//     void wait() {
// 	db<Condition>(TRC) << "Condition::wait(this=" << this
// 			  << ",ts=" << _time_stamp << ")" << endl;
// 	int ts = finc(_time_stamp);
// 	while(tsl(_not_condition) && (ts > _broadcast))
// 	    sleep();
//     }
//     void signal() {
// 	db<Condition>(TRC) << "Condition::signal(this=" << this
// 			  << ",ts=" << _time_stamp << ")" << endl;
// 	_not_condition = false;
// 	wakeup();
//     }
//     void broadcast() {
// 	db<Condition>(TRC) << "Condition::broadcast(this=" << this
// 			  << ",ts=" << _time_stamp << ")" << endl;
// 	_broadcast = finc(_time_stamp);
// 	wakeup_all();
//     }

//     static int init(System_Info * si);

// private:
//     volatile bool _not_condition;
//     volatile int _broadcast;
//     volatile int _time_stamp;
// };

__END_SYS
