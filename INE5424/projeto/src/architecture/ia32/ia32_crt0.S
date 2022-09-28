        .file        "ia32_crt0.S"

        .section .text
        .align        4
        .globl        _start
        .type        _start,@function
_start:
        // Temporary stack(s) for INIT were created and configure by SETUP
        // BSS was cleared by SETUP
        call        _init

        .align      4
        .globl      __epos_app_entry
        .type   __epos_app_entry,@function
__epos_app_entry:
        // Main's stack was allocated by Thread::init()
        call        main
        push        %eax        // save main's return value to be used by exit()
        call        _fini
        call        _exit
