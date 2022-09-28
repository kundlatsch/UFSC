// EPOS RISC-V 32 Program Starter

        .file "rv32_crt0.S"

        .section .text
        .align  4
        .global _start
        .type   _start, function
_start:
        // Temporary stack(s) for INIT were created and configure by SETUP
        // BSS was cleared by SETUP
        call      _init

        .align  4
        .globl  __epos_app_entry
        .type   __epos_app_entry, function
__epos_app_entry:
        // Main's stack was allocated by Thread::init()
        call    main
        addi    sp, sp, -4
        sw      a0, 0(sp)   // save main's return value to be used by exit()
        call    _fini
        lw      a0, 0(sp)
        call    _exit
