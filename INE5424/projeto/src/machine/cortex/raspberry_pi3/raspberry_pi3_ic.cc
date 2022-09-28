// EPOS Raspberry Pi3 (ARM Cortex-A53) IC Mediator Implementation

#include <machine/machine.h>
#include <machine/ic.h>
#include <machine/timer.h>

extern "C" { void _go_user_mode() __attribute__ ((naked)); }
extern "C" { void __exit(); }
extern "C" { void _exit(int s); }

void _go_user_mode() {
        ASM("       ldr   x30, [sp], #8             // pop PSR into x30             \t\n\
                    ldp    x0,  x1, [sp], #16                                       \t\n\
                    ldp    x2,  x3, [sp], #16                                       \t\n\
                    ldp    x4,  x5, [sp], #16                                       \t\n\
                    ldp    x6,  x7, [sp], #16                                       \t\n\
                    ldp    x8,  x9, [sp], #16                                       \t\n\
                    ldp   x10, x11, [sp], #16                                       \t\n\
                    ldp   x12, x13, [sp], #16                                       \t\n\
                    ldp   x14, x15, [sp], #16                                       \t\n\
                    ldp   x16, x17, [sp], #16                                       \t\n\
                    ldp   x18, x19, [sp], #16                                       \t\n\
                    ldp   x20, x21, [sp], #16                                       \t\n\
                    ldp   x22, x23, [sp], #16                                       \t\n\
                    ldp   x24, x25, [sp], #16                                       \t\n\
                    ldp   x26, x27, [sp], #16                                       \t\n\
                    ldp   x28, x29, [sp], #16                                       \t\n\
                    msr   spsr_el1, x30                                             \t\n\
                    ldr   x30, [sp], #8             // pop LR to get to PC          \t\n\
                    ldr   x30, [sp], #8             // pop PC                       \t\n\
                    msr   ELR_EL1, x30                                              \t\n\
                    ldr   x30, [sp, #-16]           // pop LR                       \t\n\
                    eret                                                            \t" : : : "cc");
    }

__BEGIN_SYS

IC::Interrupt_Handler IC::_eoi_vector[INTS] = {
    0,
    Timer::eoi,         // System Timer C1
    0,
    0,                  // System Timer C3 (User_Timer)--> should we add the EOI function here?
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    Timer::eoi,         // ARM TIMER INT
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    // For the sake of the simplicity of the code, a single interrupt id is used to issue IPIs.
    // In this way, we only map a single handler, agnostic of MBOX number and Core ID.
    // On the other hand, handlers take Core ID into consideration when performing EOIs.
    // As a single MBOX handler is used to address all IPIs, we clear all MBOX of a Core in this handler.
    mailbox_eoi,        // MB0_CPU0
    0, // mailbox_eoi,  // MB1_CPU0
    0, // mailbox_eoi,  // MB2_CPU0
    0, // mailbox_eoi,  // MB3_CPU0
    0, // mailbox_eoi,  // MB0_CPU1
    0, // mailbox_eoi,  // MB1_CPU1
    0, // mailbox_eoi,  // MB2_CPU1
    0, // mailbox_eoi,  // MB3_CPU1
    0, // mailbox_eoi,  // MB0_CPU2
    0, // mailbox_eoi,  // MB1_CPU2
    0, // mailbox_eoi,  // MB2_CPU2
    0, // mailbox_eoi,  // MB3_CPU2
    0, // mailbox_eoi,  // MB0_CPU3
    0, // mailbox_eoi,  // MB1_CPU3
    0, // mailbox_eoi,  // MB2_CPU3
    0, // mailbox_eoi,  // MB3_CPU3
    Timer::eoi,         // CORE0 LOCAL TIMER INT = 117
    0
};

__END_SYS
