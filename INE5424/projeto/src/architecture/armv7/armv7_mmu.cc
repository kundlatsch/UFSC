// EPOS ARMv7 MMU Mediator Implementation

#include <architecture/armv7/armv7_mmu.h>

__BEGIN_SYS

ARMv7_MMU::List ARMv7_MMU::_free[colorful * COLORS + 1];
ARMv7_MMU::Page_Directory * ARMv7_MMU::_master;

__END_SYS
