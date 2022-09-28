// EPOS ARMV8 CPU System Call Entry Implementation

#include <architecture/armv8/armv8_cpu.h>

__BEGIN_SYS

void CPU::syscall(void * msg)
{
    ASM(
        //Salvando contexto
        "str x0, [sp, #-8]!   \n"
        "mov x0, %0     \n" 
        "SVC 0x0    \n"
        "ldr lr, [sp, #8]!   \n"
        "" :: "r"(msg)
    );
}

__END_SYS