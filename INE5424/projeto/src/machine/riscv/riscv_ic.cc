// EPOS RISC-V IC Mediator Implementation

#include <machine/machine.h>
#include <machine/ic.h>
#include <machine/timer.h>
#include <process.h>

extern "C" { void _int_entry() __attribute__ ((nothrow, alias("_ZN4EPOS1S2IC5entryEv"))); }
extern "C" { void __exit(); }
extern "C" { static void print_context(); }

__BEGIN_SYS

static CPU::Reg a0;
static CPU::Reg a1;

// Class attributes
IC::Interrupt_Handler IC::_int_vector[IC::INTS];

// Class methods
void IC::entry()
{
    // Save context
    CPU::Context::push(true);
    ASM("       la     ra, 1f                   \n"     // set LR to restore context before returning
        "       j      %0                       \n" : : "i"(&dispatch));

    // Entry-point for the dummy contexts used by the first dispatching of newly created threads
    ASM("       .global _int_leave              \n"
        "_int_leave:                            \n");
if(Traits<IC>::hysterically_debugged)
    ASM("       jalr    %0                      \n" : : "r"(print_context));

    // Restore context
    ASM("1:                                     \n");
    CPU::Context::pop(true);
    CPU::iret();
}

void IC::dispatch()
{
    Interrupt_Id id = int_id();
    a0 = CPU::a0(); // exit passes status through a0
    a1 = CPU::a1(); // syscalls pass messages through a1

    if((id != INT_SYS_TIMER) || Traits<IC>::hysterically_debugged)
        db<IC>(TRC) << "IC::dispatch(i=" << id << ")" << endl;

    // IPIs must be acknowledged before calling the ISR, because in RISC-V, MIP set bits will keep on triggering interrupts until they are cleared
    if(id == INT_RESCHEDULER)
        IC::ipi_eoi(id);

    // MIP.MTI is a direct logic on (MTIME == MTIMECMP) and reseting the Timer seems to be the only way to clear it
    if(id == INT_SYS_TIMER)
        Timer::reset();

    CPU::a1(a1);
    _int_vector[id](id);

    if(id >= HARD_INT)
        CPU::a0(0); // tell CPU::Context::pop(true) not to increment PC since it is automatically incremented for hardware interrupts
}

void IC::int_not(Interrupt_Id id)
{
    db<IC>(WRN) << "IC::int_not(i=" << id << ")" << endl;
    if(Traits<Build>::hysterically_debugged)
        Machine::panic();
}

void IC::exception(Interrupt_Id id)
{
    CPU::Reg core = CPU::id();
    CPU::Reg epc = multitask ? CPU::sepc() : CPU::mepc();
    CPU::Reg sp = CPU::sp();
    CPU::Reg status = multitask ? CPU::sstatus() : CPU::mstatus();
    CPU::Reg cause = multitask? CPU::scause() : CPU::mcause();
    CPU::Reg tval = multitask ? CPU::stval() : CPU::mtval();
    Thread * thread = Thread::self();

    if((id == 12) && (epc == CPU::Reg(&__exit))) { // a page fault on __exit is triggered by MAIN after returing to CRT0
        db<IC, Thread>(TRC) << " => Thread::exit()";
        CPU::a0(a0);
        __exit();
    } else {
        db<IC,System>(WRN) << "IC::Exception(" << id << ") => {" << hex << "core=" << core << ",thread=" << thread << ",epc=" << epc << ",sp=" << sp << ",status=" << status << ",cause=" << cause << ",tval=" << tval << "}" << dec;

        switch(id) {
        case 0: // unaligned instruction
            db<IC, System>(WRN) << " => unaligned instruction";
            break;
        case 1: // instruction access failure
            db<IC, System>(WRN) << " => instruction protection violation";
            break;
        case 2: // illegal instruction
            db<IC, System>(WRN) << " => illegal instruction";
            break;
        case 3: // break point
            db<IC, System>(WRN) << " => break point";
            break;
        case 4: // unaligned load address
            db<IC, System>(WRN) << " => unaligned data read";
            break;
        case 5: // load access failure
            db<IC, System>(WRN) << " => data protection violation (read)";
            break;
        case 6: // unaligned store address
            db<IC, System>(WRN) << " => unaligned data write";
            break;
        case 7: // store access failure
            db<IC, System>(WRN) << " => data protection violation (write)";
            break;
        case 8: // user-mode environment call
        case 9: // supervisor-mode environment call
        case 10: // reserved... not described
        case 11: // machine-mode environment call
            db<IC, System>(WRN) << " => bad ecall";
            break;
        case 12: // Instruction Page Table failure
            db<IC, System>(WRN) << " => page fault";
            break;
        case 13: // Load Page Table failure
        case 14: // reserved... not described
        case 15: // Store Page Table failure
            db<IC, System>(WRN) << " => page table protection violation";
            break;
        default:
            int_not(id);
            break;
        }

        db<IC, System>(WRN) << endl;

        if(Traits<Build>::hysterically_debugged)
            Machine::panic();
    }

    CPU::fr(sizeof(void *)); // tell CPU::Context::pop(true) to perform PC = PC + 4 on return
}

__END_SYS

static void print_context() {
    __USING_SYS
    db<IC, System>(TRC) << "IC::leave:ctx=" << *reinterpret_cast<CPU::Context *>(CPU::sp() + 32) << endl;
    CPU::fr(0);
}

