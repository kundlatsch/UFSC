// EPOS Zynq (ARM Cortex-A9) SETUP

// If emulating on QEMU, uboot has a vector table in ROM at 0x00000000.
// It relocates EPOS image (.bin), which has this vector table at the
// first address, to 0x00010000 and jumps to it.

#include <system/info.h>

extern "C" {
    void _start();

    // SETUP entry point is the Vector Table and resides in the .init section (not in .text), so it will be linked first and will be the first function after the ELF header in the image.
    void _entry() __attribute__ ((naked, nothrow, section(".init")));
    void _reset(); // so it can be safely reached from the vector table

    // LD eliminates this variable while performing garbage collection, that's why the used attribute.
    char __boot_time_system_info[sizeof(EPOS::S::System_Info)] __attribute__ ((used)) = "<System_Info placeholder>"; // actual System_Info will be added by mkbi!
}

void _entry()
{
    // Interrupt Vector Table
    // We use and indirection table for the ldr instructions because the offset can be to far from the PC to be encoded
    ASM("               ldr pc, .reset                                          \t\n\
                        ldr pc, .ui                                             \t\n\
                        ldr pc, .si                                             \t\n\
                        ldr pc, .pa                                             \t\n\
                        ldr pc, .da                                             \t\n\
                        ldr pc, .reserved                                       \t\n\
                        ldr pc, .irq                                            \t\n\
                        ldr pc, .fiq                                            \t\n\
                                                                                \t\n\
                        .balign 32                                              \t\n\
        .reset:         .word _reset                                            \t\n\
        .ui:            .word 0x0                                               \t\n\
        .si:            .word 0x0                                               \t\n\
        .pa:            .word 0x0                                               \t\n\
        .da:            .word 0x0                                               \t\n\
        .reserved:      .word 0x0                                               \t\n\
        .irq:           .word 0x0                                               \t\n\
        .fiq:           .word 0x0                                               \t");
}

void _reset()
{
    ASM("		mrc     p15, 0, r2, c0, c0, 5   // CPU::id()	     		\t\n\
        		ands    r2, r2, #0x03                                   	\t\n\
        		mov     r2, r2, LSL #14         // 16KB stacks per core 	\t\n\
        		ldr     r1, =__boot_stack__                             	\t\n\
        		sub     r1, r1, r2              // skip other cores' stacks 	\t\n\
        		mov     sp, r1                                                  \t\n\
                                                                                	\t\n\
        		b       _start                                                  \t");
}
