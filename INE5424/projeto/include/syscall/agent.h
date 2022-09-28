// EPOS Thread Component Declarations

#ifndef __agent_h
#define __agent_h

#include <architecture.h>
#include <machine.h>
#include <syscall/message.h>

#include <time.h>
#include <memory.h>
#include <process.h>
#include <synchronizer.h>
#include <time.h>

__BEGIN_SYS

class Agent: public Message
{
public:
    static void _exec(){
        Agent * agt;
        ASM("mov %0, x0  " : "=r"(agt) :);
        agt->exec();
    }
    void exec() {
        switch(entity()) {
            case Message::ENTITY::DISPLAY:
                handle_display();
                break;
            case Message::ENTITY::THREAD:
                handle_thread();
                break;
            case Message::ENTITY::TASK:
                handle_task();
                break; 
            case Message::ENTITY::ADDRESS_SPACE:
                handle_address_space();
                break;  
            case Message::ENTITY::SEGMENT:
                handle_segment();
                break;
            case Message::ENTITY::MUTEX:
                handle_mutex();
                break;
            case Message::ENTITY::SEMAPHORE:
                handle_semaphore();
                break;
            case Message::ENTITY::CONDITION:
                handle_condition();
                break;
            case Message::ENTITY::CLOCK:
                handle_clock();
                break;
            case Message::ENTITY::ALARM:
                handle_alarm();
                break;
            case Message::ENTITY::DELAY:
                handle_delay();
                break;
            case Message::ENTITY::CHRONOMETER:
                handle_chronometer();
                break;
            default:
                break;
        }
    }

private:
    void handle_display() {
        switch(method()) {
            case Message::PRINT: {
                const char * s;
                get_params(s);
                Display::puts(s);
            } break;
            default:
                break;
        }
    }

    void handle_thread() {
        switch(method()) {
            case Message::THREAD_CREATE: {
                int (* entry)();
                get_params(entry);
                Thread * t = new (SYSTEM) Thread(Thread::Configuration(Thread::READY, Thread::NORMAL), entry);
                result(reinterpret_cast<long>(t));
            }   break;
            case Message::THREAD_CREATE_CONFIG: {
                int (* entry)();
                get_params(entry);
                new (SYSTEM) Thread(entry);
            }   break;
            /*
            case Message::THREAD_PRIORITY1: {
                Thread * t = reinterpret_cast<Thread *>(id());
                Criterion & p;
                get_params(p);
                t->priority(p);
            } */
            case Message::THREAD_TASK: {
                Thread * t = reinterpret_cast<Thread *>(id());
                Task * task = t->task();
                result(reinterpret_cast<long>(task));
            }   break;
            case Message::THREAD_JOIN: {
                Thread * t = reinterpret_cast<Thread *>(id());
                int r = t->join();
                result(r);
            }   break;
            case Message::THREAD_PASS: {
                Thread * t = reinterpret_cast<Thread *>(id());
                t->pass();
            }   break;
            case Message::THREAD_SUSPEND: {
                Thread * t = reinterpret_cast<Thread *>(id());
                t->suspend();
            }   break;
            case Message::THREAD_RESUME: {
                Thread * t = reinterpret_cast<Thread *>(id());
                t->resume();
            }   break;
            case Message::THREAD_SELF: {
                Thread * t = Thread::self();
                result(reinterpret_cast<long>(t));
                break;
            }
            case Message::THREAD_YIELD: {
                Thread::yield();
            }   break;
            case Message::THREAD_EXIT: {
                int status;
                get_params(status);
                Thread::exit(status);
            }   break;
            default:
                db<Agent>(TRC) << "FAILED :(" << endl;
                break;
        }
    }

    void handle_task(){
        switch(method()) {
            case Message::TASK_CREATE: {
                Segment * cs; 
                Segment * ds; 
                int (* entry)();
                Address_Space::Log_Addr code; 
                Address_Space::Log_Addr data; 
                get_params(cs, ds, entry, code, data);
                Task * t = new (SYSTEM) Task(cs, ds, entry, code, data);
                result(reinterpret_cast<long>(t));
            }   break;
            case Message::TASK_ADDRESS_SPACE: {
                Task * t = reinterpret_cast<Task *>(id());
                Address_Space * a = t->address_space();
                result(reinterpret_cast<long>(a));
            }   break;
            case Message::TASK_CODE_SEGMENT: {
                Task * t = reinterpret_cast<Task *>(id());
                Segment * s = t->code_segment();
                result(reinterpret_cast<long>(s));
            }   break;
            case Message::TASK_DATA_SEGMENT: {
                Task * t = reinterpret_cast<Task *>(id());
                Segment * s = t->data_segment();
                result(reinterpret_cast<long>(s));
            }   break;
            case Message::TASK_CODE: {
                Task * t = reinterpret_cast<Task *>(id());
                Address_Space::Log_Addr l = t->code();
                result(l);
            }   break;
            case Message::TASK_DATA: {
                Task * t = reinterpret_cast<Task *>(id());
                Address_Space::Log_Addr l = t->data();
                result(l);
            }   break;
            case Message::TASK_MAIN: {
                Task * t = reinterpret_cast<Task *>(id());
                Thread * m = t->main();
                result(reinterpret_cast<long>(m));
            }   break;
            case Message::TASK_ID: {
                Task * t = reinterpret_cast<Task *>(id());
                unsigned int r = t->id();
                result(r);
            }   break;
            default:
                db<Agent>(TRC) << "FAILED :(" << endl;
                break;
        }
    }

    void handle_address_space(){
        switch(method()) {
            case Message::ADDRESS_SPACE_CREATE: {
                Address_Space * a = new (SYSTEM) Address_Space();
                result(reinterpret_cast<long>(a));
            }   break;
            case Message::ADDRESS_SPACE_CREATE_PD: {
                MMU::Page_Directory * pd;
                get_params(pd);
                Address_Space * a = new (SYSTEM) Address_Space(pd);
                result(reinterpret_cast<long>(a));
            }   break;
            case Message::ADDRESS_SPACE_ATTACH1: {
                Segment * seg;
                Address_Space * a = reinterpret_cast<Address_Space *>(id());
                get_params(seg);
                result(a->attach(seg));
            }   break;
            case Message::ADDRESS_SPACE_ATTACH2: {
                Segment * seg;
                Address_Space::Log_Addr addr;
                Address_Space * a = reinterpret_cast<Address_Space *>(id());
                get_params(seg, addr);
                result(a->attach(seg, addr));
            }   break;
            case Message::ADDRESS_SPACE_DETACH1 : {
                Segment * seg;
                Address_Space * a = reinterpret_cast<Address_Space *>(id());
                get_params(seg);
                a->detach(seg);
            }   break;
            case Message::ADDRESS_SPACE_DETACH2 : {
                Segment * seg;
                Address_Space::Log_Addr addr;
                Address_Space * a = reinterpret_cast<Address_Space *>(id());
                get_params(seg, addr);
                a->detach(seg, addr);
            }   break;
            case Message::ADDRESS_SPACE_PHYSICAL : {
                Address_Space::Log_Addr address;
                Address_Space * a = reinterpret_cast<Address_Space *>(id());
                get_params(address);
                result(a->physical(address));
            }   break;
            default:
                db<Agent>(TRC) << "FAILED :(" << endl;
                break;
        }
    }
    void handle_segment(){
        switch(method()) {
            case Message::SEGMENT_CREATE: {
                unsigned int bytes;
                Segment::Flags flags;
                get_params(bytes, flags);
                Segment * s = new (SYSTEM) Segment(bytes, flags);
                result(reinterpret_cast<long>(s));
            }   break;
            case Message::SEGMENT_CREATE_PHY: {
                Segment::Phy_Addr phy_addr;
                unsigned int bytes;
                Segment::Flags flags;
                get_params(phy_addr, bytes, flags);
                Segment * s = new (SYSTEM) Segment(phy_addr, bytes, flags);
                result(reinterpret_cast<long>(s));
            } break;
            case Message::SEGMENT_SIZE: {
                Segment * s = reinterpret_cast<Segment *>(id());
                int r = s->size();
                result(r);
            } break;
            case Message::SEGMENT_PHY_ADDRESS: {
                Segment * s = reinterpret_cast<Segment *>(id());
                int r = s->phy_address();
                result(r);
            } break;
            case Message::SEGMENT_RESIZE: {
                Segment * s = reinterpret_cast<Segment *>(id());
                int amount;
                get_params(amount);
                int r = s->resize(amount);
                result(r);
            } break;
            default:
                db<Agent>(TRC) << "FAILED :(" << endl;
                break;
        } 
    }
    void handle_mutex() {
        switch(method()) {
            case Message::MUTEX_CREATE: {
                Mutex * m = new (SYSTEM) Mutex();
                db<Agent>(TRC) << "AGENT CREATE MUTEX" << endl;
                result(reinterpret_cast<long>(m));
            }   break;
            case Message::MUTEX_LOCK: {
                Mutex * m = reinterpret_cast<Mutex *>(id());
                m->lock();
                db<Agent>(TRC) << "AGENT MUTEX LOCK" << endl;
            }   break;
            case Message::MUTEX_UNLOCK: {
                Mutex * m = reinterpret_cast<Mutex *>(id());
                m->unlock();
                db<Agent>(TRC) << "AGENT MUTEX UNLOCK" << endl;
            }   break;
            case Message::MUTEX_HANDLER: {
                Mutex * m = reinterpret_cast<Mutex *>(id());
                Mutex_Handler * h = new (SYSTEM) Mutex_Handler(m);
                db<Agent>(TRC) << "AGENT NEW MUTEX_HANDLER" << endl;
                result(reinterpret_cast<long>(h));
            }   break;
            case Message::MUTEX_OPERATOR: {
                Mutex_Handler * h = reinterpret_cast<Mutex_Handler *>(id());
                (* h)();
                db<Agent>(TRC) << "AGENT MUTEX_HANDLER ()" << endl;
            }   break;
            default:
                db<Agent>(TRC) << "MUTEX FAILED :(" << endl;
                break;
        }
    }
    void handle_semaphore(){
        switch(method()) {
            case Message::SEMAPHORE_CREATE: {
                int v;
                get_params(v);
                Semaphore * s = new (SYSTEM) Semaphore(v);
                result(reinterpret_cast<long>(s));
            }   break;
            case Message::SEMAPHORE_P: {
                Semaphore * s = reinterpret_cast<Semaphore *>(id());
                s->p();
            }   break;
            case Message::SEMAPHORE_V: {
                Semaphore * s = reinterpret_cast<Semaphore *>(id());
                s->v();
            }   break;
            case Message::SEMAPHORE_HANDLER: {
                Semaphore * s;
                get_params(s);
                Semaphore_Handler * sh = new (SYSTEM) Semaphore_Handler(s);
                result(reinterpret_cast<long>(sh));
            }   break;
            case Message::SEMAPHORE_OPERATOR : {
                Semaphore_Handler * sh;
                get_params(sh);
                (*sh)();
            }   break;
            default:
                db<Agent>(TRC) << "FAILED :(" << endl;
                break;
        }
    }
    void handle_condition(){
        switch(method()) {
            case Message::CONDITION_CREATE: {
                Condition * c = new (SYSTEM) Condition();
                result(reinterpret_cast<long>(c));
            }   break;
            case Message::CONDITION_WAIT: {
                Condition * c = reinterpret_cast<Condition *>(id());
                c->wait();
                // db<Agent>(TRC) << "AHH CREATE THREAD" << endl;
            }   break;
            case Message::CONDITION_SIGNAL: {
                Condition * c = reinterpret_cast<Condition *>(id());
                c->signal();
                // db<Agent>(TRC) << "AHH CREATE THREAD" << endl;
            }   break;
            case Message::CONDITION_BROADCAST: {
                Condition * c = reinterpret_cast<Condition *>(id());
                c->broadcast();
                // db<Agent>(TRC) << "AHH EXIT THREAD" << endl;
            }   break;
            case Message::CONDITION_HANDLER: {
                Condition * c;
                get_params(c);
                Condition_Handler * ch = new (SYSTEM) Condition_Handler(c);
                result(reinterpret_cast<long>(ch));
            }   break;
            case Message::CONDITION_OPERATOR : {
                Condition_Handler * ch;
                get_params(ch);
                (*ch)();
            }   break;
            default:
                db<Agent>(TRC) << "FAILED :(" << endl;
                break;
        }
    }
    void handle_clock(){}
    void handle_alarm(){
        switch(method()) {
            case Message::ALARM_CREATE: {
                Microsecond time;
                Handler * handler;
                unsigned int times;
                get_params(time, handler, times);
                Alarm * a = new (SYSTEM) Alarm(time, handler, times);
                result(reinterpret_cast<long>(a));
            }   break;
            case Message::ALARM_PERIOD: {
                Microsecond p;
                Alarm * a = reinterpret_cast<Alarm *>(id());
                p = a->period();
                result(p);
            }   break;
            case Message::ALARM_PERIOD1: {
                Microsecond p;
                Alarm * a = reinterpret_cast<Alarm *>(id());
                get_params(p);
                a->period(p);
            }   break;
            case Message::ALARM_RESET: {
                Alarm * a = reinterpret_cast<Alarm *>(id());
                a->reset();
            }   break;
            case Message::ALARM_FREQUENCY : {
                result(Alarm::frequency());
            }   break;
            case Message::ALARM_DELAY : {
                Microsecond time;
                get_params(time);
                Alarm * a = reinterpret_cast<Alarm *>(id());
                a->delay(time);
            }   break;
            default:
                db<Agent>(TRC) << "FAILED :(" << endl;
                break;
        }
    }
    void handle_delay(){
        switch(method()) {
            case Message::DELAY_CREATE: {
                Microsecond time;
                get_params(time);
                Delay * d = new (SYSTEM) Delay(reinterpret_cast<const Microsecond &>(time));
                result(reinterpret_cast<long>(d));
            }   break;
          default:
                db<Agent>(TRC) << "FAILED :(" << endl;
                break;
        }
    }
    void handle_chronometer(){
        switch(method()) {
            case Message::CHRONOMETER_CREATE: {
                Chronometer * c = new (SYSTEM) Chronometer();
                result(reinterpret_cast<long>(c));
            }   break;
            case Message::CHRONOMETER_FREQUENCY: {
                Chronometer * c = reinterpret_cast<Chronometer *>(id());
                Hertz p = c->frequency();
                result(p);
                // db<Agent>(TRC) << "AHH CREATE THREAD" << endl;
            }   break;
            case Message::CHRONOMETER_RESET: {
                Chronometer * c = reinterpret_cast<Chronometer *>(id());
                c->reset();
                // db<Agent>(TRC) << "AHH EXIT THREAD" << endl;
            }   break;
            case Message::CHRONOMETER_START: {
              Chronometer * c = reinterpret_cast<Chronometer *>(id());
              c->start();
          }   break;
            case Message::CHRONOMETER_LAP : {
              Chronometer * c = reinterpret_cast<Chronometer *>(id());
              c->lap();
          }   break;
            case Message::CHRONOMETER_STOP : {
              Chronometer * c = reinterpret_cast<Chronometer *>(id());
              c->stop();
          }   break;
            case Message::CHRONOMETER_READ : {
              Chronometer * c = reinterpret_cast<Chronometer *>(id());
              result(c->read());
          }   break;
            default:
                db<Agent>(TRC) << "FAILED :(" << endl;
                break;
        }
    }
};

__END_SYS

#endif
  