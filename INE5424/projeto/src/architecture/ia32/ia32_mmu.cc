// EPOS IA32 MMU Mediator Implementation

#include <architecture/ia32/ia32_mmu.h>

__BEGIN_SYS

// Class attributes
MMU::List MMU::_free[colorful * COLORS + 1];
MMU::Page_Directory * MMU::_master;

__END_SYS
