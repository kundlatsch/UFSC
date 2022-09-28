// EPOS Task Implementation

#include <process.h>

__BEGIN_SYS

// Class attributes
volatile unsigned int Task::_task_count;
Task* volatile Task::_current;

// Methods
Task::~Task()
{
    db<Task>(TRC) << "~Task(this=" << this << ")" << endl;

    while (!_threads.empty()){
        delete _threads.remove()->object();
    }

    lock();
    _task_count--;
    unlock();

    delete _as;
}

__END_SYS