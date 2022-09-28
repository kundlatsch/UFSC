// EPOS ARMv7 CPU Mediator Implementation

#include <architecture/armv7/armv7_cpu.h>

__BEGIN_SYS

// Class attributes
unsigned int CPU::_cpu_clock;
unsigned int CPU::_bus_clock;

// Class methods
void CPU::Context::save() volatile
{
    ASM("       str     r12, [sp,#-68]          \n"
        "       mov     r12, pc                 \n");
if(thumb)
    ASM("       orr     r12, #1                 \n");

    ASM("       str     r12, [%0, #-64]         \n"
        "       ldr     r12, [sp, #-68]         \n"
        "       str     lr,  [%0, #-60]         \n"
        "       str     r12, [%0, #-56]         \n"
        "       str     r11, [%0, #-52]         \n"
        "       str     r10, [%0, #-48]         \n"
        "       str     r9,  [%0, #-44]         \n"
        "       str     r8,  [%0, #-40]         \n"
        "       str     r7,  [%0, #-36]         \n"
        "       str     r6,  [%0, #-32]         \n"
        "       str     r5,  [%0, #-28]         \n"
        "       str     r4,  [%0, #-24]         \n"
        "       str     r3,  [%0, #-20]         \n"
        "       str     r2,  [%0, #-16]         \n"
        "       str     r1,  [%0, #-12]         \n"
        "       str     r0,  [%0, #-8]          \n" : : "r"(this));
    psr_to_tmp();
    ASM("       str     r12, [%0, #-4]          \n"
        "       ldr     r12, [sp, #-68]         \n"
        "       add     %0, %0, #-68            \n"
        "       bx      lr                      \n" : : "r"(this));
}

void CPU::Context::load() const volatile
{
    sp(this);
    Context::pop();
}

void CPU::switch_context(Context ** o, Context * n)
{
    // Push the context into the stack and update "o"
    Context::push();
    *o = sp();

    // Set the stack pointer to "n" and pop the context
    sp(n);
    Context::pop();

    // Cross-domain return point used in save_regs()
    iret();
}

__END_SYS
