// EPOS ARMv8 PMU Events Declaration
/*
#include <architecture/pmu.h>

__BEGIN_SYS

const ARMv8_A_PMU::Reg32 ARMv8_A_PMU::_events[ARMv8_A_PMU::EVENTS] = {
    ARMv8_A_PMU::INSTRUCTIONS_ARCHITECTURALLY_EXECUTED,  // 0
    ARMv8_A_PMU::IMMEDIATE_BRANCH,                       // 1
    ARMv8_A_PMU::CYCLE,                                  // 2
    ARMv8_A_PMU::BRANCHES_ARCHITECTURALLY_EXECUTED,      // 3
    ARMv8_A_PMU::MISPREDICTED_BRANCH,                    // 4
    ARMv8_A_PMU::L1D_ACCESS,                             // 5
    ARMv8_A_PMU::L2D_ACCESS,                             // 6
    ARMv8_A_PMU::L1D_REFILL,                             // 7
    ARMv8_A_PMU::DATA_MEMORY_ACCESS,                     // 8 (LLC MISS)
    ARMv8_A_PMU::L1I_REFILL,                             // 9
    ARMv8_A_PMU::L1I_TLB_REFILL,                         // 10
    ARMv8_A_PMU::PREDICTABLE_BRANCH_EXECUTED,            // 11
    ARMv8_A_PMU::L1D_WRITEBACK,                          // 12
    ARMv8_A_PMU::L2D_WRITEBACK,                          // 13
    ARMv8_A_PMU::L2D_REFILL,                             // 14
    ARMv8_A_PMU::UNALIGNED_LOAD_STORE,                   // 15
    ARMv8_A_PMU::L1I_ACCESS,                             // 16
    ARMv8_A_PMU::L1D_TLB_REFILL,                         // 17
    ARMv8_A_PMU::EXCEPTION_TAKEN,                        // 18
    ARMv8_A_PMU::BUS_ACCESS,                             // 19
    ARMv8_A_PMU::LOCAL_MEMORY_ERROR,                     // 20
    ARMv8_A_PMU::INSTRUCTION_SPECULATIVELY_EXECUTED,     // 21
    ARMv8_A_PMU::BUS_CYCLE,                              // 22
    ARMv8_A_PMU::CHAIN,                                  // 23
    ARMv8_A_PMU::// ARM Cortex-A53 specific events (24-62 are Cortex-A9 events)
    ARMv8_A_PMU::BUS_ACCESS_LD,                          // 63 --> 24
    ARMv8_A_PMU::BUS_ACCESS_ST,                          // 64 --> 25
    ARMv8_A_PMU::BR_INDIRECT_SPEC,                       // 65 --> 26
    ARMv8_A_PMU::EXC_IRQ,                                // 66 --> 27
    ARMv8_A_PMU::EXC_FIQ,                                // 67 --> 28
    ARMv8_A_PMU::EXTERNAL_MEM_REQUEST,                   // 68 --> 29
    ARMv8_A_PMU::EXTERNAL_MEM_REQUEST_NON_CACHEABLE,     // 69 --> 30
    ARMv8_A_PMU::PREFETCH_LINEFILL,                      // 70 --> 31
    ARMv8_A_PMU::ICACHE_THROTTLE,                        // 71 --> 32
    ARMv8_A_PMU::ENTER_READ_ALLOC_MODE,                  // 72 --> 33
    ARMv8_A_PMU::READ_ALLOC_MODE,                        // 73 --> 34
    ARMv8_A_PMU::PRE_DECODE_ERROR,                       // 74 --> 35
    ARMv8_A_PMU::DATA_WRITE_STALL_ST_BUFFER_FULL,        // 75 --> 36
    ARMv8_A_PMU::SCU_SNOOPED_DATA_FROM_OTHER_CPU,        // 76 --> 37
    ARMv8_A_PMU::CONDITIONAL_BRANCH_EXECUTED,            // 77 --> 38
    ARMv8_A_PMU::IND_BR_MISP,                            // 78 --> 39
    ARMv8_A_PMU::IND_BR_MISP_ADDRESS_MISCOMPARE,         // 79 --> 40
    ARMv8_A_PMU::CONDITIONAL_BRANCH_MISP,                // 80 --> 41
    ARMv8_A_PMU::L1_ICACHE_MEM_ERROR,                    // 81 --> 42
    ARMv8_A_PMU::L1_DCACHE_MEM_ERROR,                    // 82 --> 43
    ARMv8_A_PMU::TLB_MEM_ERROR,                          // 83 --> 44
    ARMv8_A_PMU::EMPTY_DPU_IQ_NOT_GUILTY,                // 84 --> 45
    ARMv8_A_PMU::EMPTY_DPU_IQ_ICACHE_MISS,               // 85 --> 46
    ARMv8_A_PMU::EMPTY_DPU_IQ_IMICRO_TLB_MISS,           // 86 --> 47
    ARMv8_A_PMU::EMPTY_DPU_IQ_PRE_DECODE_ERROR,          // 87 --> 48
    ARMv8_A_PMU::INTERLOCK_CYCLE_NOT_GUILTY,             // 88 --> 49
    ARMv8_A_PMU::INTERLOCK_CYCLE_LD_ST_WAIT_AGU_ADDRESS, // 89 --> 50
    ARMv8_A_PMU::INTERLOCK_CYCLE_ADV_SIMD_FP_INST,       // 90 --> 51
    ARMv8_A_PMU::INTERLOCK_CYCLE_WR_STAGE_STALL_BC_MISS, // 91 --> 52
    ARMv8_A_PMU::INTERLOCK_CYCLE_WR_STAGE_STALL_BC_STR   // 92 --> 53
};

__END_SYS
*/