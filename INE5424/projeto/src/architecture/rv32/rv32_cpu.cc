// EPOS RISC-V 32 CPU Mediator Implementation

#include <architecture/rv32/rv32_cpu.h>
#include <system.h>

__BEGIN_SYS

unsigned int CPU::_cpu_clock;
unsigned int CPU::_bus_clock;

void CPU::Context::save() volatile
{
    ASM("       csrr     x3,  sscratch          \n"     // sscratch = usp (sscratch holds ksp in user-land and usp in kernel; usp = 0 for kernel threads)
        "       sw       x3,    0(a0)           \n"     // push usp
        "       sw       x1,    4(a0)           \n");   // push lr as pc
if(multitask)
    ASM("       csrr     x3,  sstatus           \n");
else
    ASM("       csrr     x3,  mstatus           \n");

    ASM("       sw       x3,    8(a0)           \n"     // push st
        "       sw       x1,   12(a0)           \n"     // push ra
        "       sw       x5,   16(a0)           \n"     // push x5-x31
        "       sw       x6,   20(a0)           \n"
        "       sw       x7,   24(a0)           \n"
        "       sw       x8,   28(a0)           \n"
        "       sw       x9,   32(a0)           \n"
        "       sw      x10,   36(a0)           \n"
        "       sw      x11,   40(a0)           \n"
        "       sw      x12,   44(a0)           \n"
        "       sw      x13,   48(a0)           \n"
        "       sw      x14,   52(a0)           \n"
        "       sw      x15,   56(a0)           \n"
        "       sw      x16,   60(a0)           \n"
        "       sw      x17,   64(a0)           \n"
        "       sw      x18,   68(a0)           \n"
        "       sw      x19,   72(a0)           \n"
        "       sw      x20,   76(a0)           \n"
        "       sw      x21,   80(a0)           \n"
        "       sw      x22,   84(a0)           \n"
        "       sw      x23,   88(a0)           \n"
        "       sw      x24,   92(a0)           \n"
        "       sw      x25,   96(a0)           \n"
        "       sw      x26,  100(a0)           \n"
        "       sw      x27,  104(a0)           \n"
        "       sw      x28,  108(a0)           \n"
        "       sw      x29,  112(a0)           \n"
        "       sw      x30,  116(a0)           \n"
        "       sw      x31,  120(a0)           \n"
        "       ret                             \n");
}

// Context load does not verify if interrupts were previously enabled by the Context's constructor
// We are setting mstatus to MPP | MPIE, therefore, interrupts will be enabled only after mret
void CPU::Context::load() const volatile
{
    sp(Log_Addr(this));
    pop();
    iret();
}

void CPU::switch_context(Context ** o, Context * n)     // "o" is in a0 and "n" is in a1
{   
    // Push the context into the stack and update "o"
    Context::push();
    ASM("sw sp, 0(a0)");   // update Context * volatile * o, which is in a0

    // Set the stack pointer to "n" and pop the context from the stack
    ASM("mv sp, a1");   // "n" is in a1
    Context::pop();
    iret();
}

__END_SYS

