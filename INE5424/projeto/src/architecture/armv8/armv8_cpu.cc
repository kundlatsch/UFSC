// EPOS ARMv7 CPU Mediator Implementation

#include <architecture/armv8/armv8_cpu.h>

__BEGIN_SYS

unsigned int CPU::_cpu_clock;
unsigned int CPU::_bus_clock;

void CPU::Context::save() volatile {
    ASM(R"(
        // Store Registers
        stp x0,  x1,  [%0,  #16]
        stp x2,  x3,  [%0, #32]
        stp x4,  x5,  [%0, #48]
        stp x6,  x7,  [%0, #64]
        stp x8,  x9,  [%0, #80]
        stp x10, x11, [%0, #96]
        stp x12, x13, [%0, #112]
        stp x14, x15, [%0, #128]
        stp x16, x17, [%0, #144]
        stp x18, x19, [%0, #160]
        stp x20, x21, [%0, #176]
        stp x22, x23, [%0, #192]
        stp x24, x25, [%0, #208]
        stp x26, x27, [%0, #224]
        stp x28, x29, [%0, #240]
        // Save LR and PC (Before LR)
        adr x16, ret
        stp x30, x16, [%0, #256]
        
        // Compute and store PSR
        mrs x16, daif
        mrs x17, nzcv
        orr x16, x16, x17
        mrs x17, CurrentEL
        orr x16, x16, x17
        mrs x17, SPSel
        orr x16, x16, x17
        str x16,      [%0,  #0]
        // Restore IPC Registers
        ldp x16, x17, [%0, #144]
        ret: ret
    )"::"r"(this));
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
