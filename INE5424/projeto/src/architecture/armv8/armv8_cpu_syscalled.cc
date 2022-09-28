// EPOS ARMV8 CPU System Call Entry Implementation

#include <architecture/armv8/armv8_cpu.h>

extern "C" { void _sysexec(); }

__BEGIN_SYS

void CPU::syscalled() {
      ASM("str lr, [sp, #-8]!   \n"
          "str x0, [sp, #-8]!   \n"
          "bl _sysexec          \n" 
          "ldr x0, [sp, #8]!    \n"
          "ldr lr, [sp, #8]!    \n"
      );
}

__END_SYS