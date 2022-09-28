// EPOS IA32 CPU Mediator Implementation

#include <architecture/ia32/ia32_cpu.h>
#include <architecture/ia32/ia32_mmu.h>
#include <machine/ic.h>
#include <system/memory_map.h>

__BEGIN_SYS

volatile unsigned int CPU::_cores;
Hertz CPU::_cpu_clock;
Hertz CPU::_cpu_current_clock;
Hertz CPU::_bus_clock;

void CPU::Context::save() volatile
{
    // Save the running thread context into its own stack (mostly for debugging)
    ASM("        push    %ebp                                            \n"
        "        mov     %esp, %ebp                                      \n"
        "        mov     8(%ebp), %esp   # sp = this                     \n"
        "        add     $40, %esp       # sp += sizeof(Context)         \n"
        "        push    4(%ebp)         # push eip                      \n"
        "        pushf                                                   \n"
        "        push    %eax                                            \n"
        "        push    %ecx                                            \n"
        "        push    %edx                                            \n"
        "        push    %ebx                                            \n"
        "        push    %ebp            # push esp                      \n"
        "        push    (%ebp)          # push ebp                      \n"
        "        push    %esi                                            \n"
        "        push    %edi                                            \n"
        "        mov     %ebp, %esp                                      \n"
        "        pop     %ebp                                            \n");
}

void CPU::Context::load() const volatile
{
    // Reload Segment Registers with user-level selectors
if(Traits<System>::multitask)
    ASM("        mov     %0, %%ds                                        \n"
        "        mov     %0, %%es                                        \n"
        "        mov     %0, %%fs                                        \n"
        "        mov     %0, %%gs                                        \n" : : "r"(SEL_APP_DATA));

    // The thread's context in on its stack
    ASM("        mov     4(%esp), %esp         # sp = this               \n");

    // Adjust the user-level stack pointer in the dummy TSS (what for?)
    ASM("        pop     %0                                              \n" : "=m"(reinterpret_cast<TSS *>(Memory_Map::TSS0 + CPU::id() * sizeof(MMU::Page))->esp) : );

    // Adjust the system-level stack pointer in the dummy TSS (that will be used by system calls and interrupts) for this Thread
if(Traits<System>::multitask)
    ASM("        mov     %%esp, %%eax                                    \n"
        "        add     $52, %%eax                                      \n"
        "        movl    %%eax, %0                                       \n" : "=m"(reinterpret_cast<TSS *>(Memory_Map::TSS0 + CPU::id() * sizeof(MMU::Page))->esp0) : : "eax");

    // Perform a possibly cross-level return (from kernel to user-level)
    // Stack contents depend on the CPL in CS, either ss, esp, eflags, cs, eip (for cross-level)
    // or eflags, cs, eip (for same-level)
    ASM("        popa                                                    \n"
        "        iret                                                    \n");
}

void CPU::switch_context(Context * volatile * o, Context * volatile n)
{
    // Recover the return address from the stack and save the previously running thread context ("o") into its stack
    // PUSHA saves an extra SP (which is always "this"), but saves several instruction fetches
    ASM("        pop     %%esi                   # eip                   \n"
        "        pushf                                                   \n"
        "        push    %%cs                                            \n"
        "        push    %%esi                   # eip                   \n"
        "        pusha                                                   \n"
        "        push    %0                                              \n"
        "        mov     48(%%esp), %%eax        # old                   \n"
        "        mov     %%esp, (%%eax)                                  \n" : : "m"(reinterpret_cast<TSS *>(Memory_Map::TSS0 + CPU::id() * sizeof(MMU::Page))->esp));

    // Restore the next thread context ("n") from its stack and updates the user-level stack pointer in the dummy TSS used for mode changes (user/system)
    ASM("        mov     52(%esp), %esp          # new	                 \n");
    ASM("        pop     %0                                              \n" : "=m"(reinterpret_cast<TSS *>(Memory_Map::TSS0 + CPU::id() * sizeof(MMU::Page))->esp) : );

if(Traits<System>::multitask)
    // Adjust the system-level stack pointer in the dummy TSS used by this Thread for syscalls and interrupts
    ASM("        mov     %%esp, %%eax                                    \n"
        "        add     $52, %%eax                                      \n"
        "        movl    %%eax, %0                                       \n" : "=m"(reinterpret_cast<TSS *>(Memory_Map::TSS0 + CPU::id() * sizeof(MMU::Page))->esp0) : : "eax");

    // IRET pops FLAGS, CS, and IP
    ASM("        popa                                                    \n"
        "        iret                                                    \n");
}

__END_SYS
