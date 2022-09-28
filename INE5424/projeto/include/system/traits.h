#ifndef __traits_types_h
#define __traits_types_h

__BEGIN_UTIL

// Utilities
template<unsigned int KEY_SIZE> class SWAES;
template<typename T, unsigned int LENGHT> class Array;
class Bitmaps;
class Ciphers;
class CRC;
class Debug;
class ELF;
class Handler;
class Lists;
class Heaps;
class Observers;
class OStream;
class Predictors;
class Queues;
class Random;
class Spin;
class SREC;
class Vectors;
template<typename> class Scheduler;

__END_UTIL

__BEGIN_SYS

// System parts
class Build;
class Boot;
class Setup;
class Init;
class Utility;

// Architecture Hardware Mediators
class CPU;
class TSC;
class MMU;
class FPU;
class PMU;

// Machine Hardware Mediators
class Machine;
class PCI;
class IC;
class Timer;
class RTC;
class UART;
class SPI;
class RS485;
class USB;
class EEPROM;
class Display;
class Serial_Display;
class Keyboard;
class Serial_Keyboard;
class Scratchpad;
class Watchdog;
class GPIO;
class I2C;
class ADC;
class FPGA;
template<unsigned int KEY_SIZE> class HWAES;
class Ethernet;
class IEEE802_15_4;
class Modem;
template<typename Family> class NIC;
class PCNet32;
class RTL8139;
class C905;
class E100;
class M95;
class IEEE802_15_4_NIC;
class Ethernet_NIC;

// Transducer Mediators (i.e. sensors and actuators)
class Transducers;
class Dummy_Transducer;
class Accelerometer;
class Gyroscope;
class Thermometer;
class Alternate_Thermometer;
class Hygrometer;
class Alternate_Hygrometer;
class CO2_Sensor;
class Pluviometer;
class Pressure_Sensor;
class Keypad;

// API Components
class System;
class Application;
class Shared_Memory;

class Thread;
class Active;
class Periodic_Thread;
class RT_Thread;
class Task;
class Priority;
class FCFS;
class RR;
class RM;
class DM;
class EDF;
class GRR;
class Fixed_CPU;
class CPU_Affinity;
class GEDF;
class PEDF;
class CEDF;
class PRM;
class EA_PEDF;

class Address_Space;
class Segment;

class Synchronizer;
class Mutex;
class Semaphore;
class Condition;

class Time;
class Clock;
class Chronometer;
class Alarm;
class Delay;

template<typename T> class Clerk;
class Monitor;

class Network;
class ELP;
class TSTPOE;
class TSTP;
template<typename NIC, typename Network, unsigned int HTYPE> class ARP;
class IP;
class ICMP;
class UDP;
class TCP;
class DHCP;
class HTTP;
class IPC;
template<typename Channel, bool connectionless = Channel::connectionless> class Link;
template<typename Channel, bool connectionless = Channel::connectionless> class Port;

class SmartData;
template<typename Transducer, typename Network = TSTP> class Responsive_SmartData;
template<typename Transducer, typename Network = TSTP> class Interested_SmartData;

// Framework
class Framework;
template<typename Component> class Handle;
template<typename Component, bool remote> class Stub;
template<typename Component> class Proxy;
template<typename Component> class Adapter;
template<typename Component> class Scenario;
class Agent;

// Aspects
class Aspect;
template<typename Component> class Authenticated;
template<typename Component> class Shared;
template<typename Component> class Remote;

// Configuration Tokens
struct Traits_Tokens
{
    // EPOS software architecture (aka mode)
    enum {LIBRARY, BUILTIN, KERNEL};

    // CPU hardware architectures
    enum {AVR8, H8, ARMv4, ARMv7, ARMv8, IA32, X86_64, SPARCv8, PPC32, RV32, RV64};

    // Machines
    enum {eMote1, eMote2, STK500, RCX, Cortex, PC, Leon, Virtex, RISCV};

    // Machine models
    enum {Unique, Legacy_PC, eMote3, LM3S811, Zynq, Realview_PBX, Raspberry_Pi3, SiFive_E, SiFive_U};

    // Architecture endianness
    enum {LITTLE, BIG};

    // Serial display engines
    enum {UART, USB};

    // Life span multipliers
    enum {FOREVER = 0, SECOND = 1, MINUTE = 60, HOUR = 3600, DAY = 86400, WEEK = 604800, MONTH = 2592000, YEAR = 31536000};

    // IP configuration strategies
    enum {STATIC, MAC, INFO, RARP, DHCP};

    // SmartData predictors
    enum :unsigned char {NONE, LVP, DBP};

    // Monitor events (Transducers)
    enum Transducer_Event {
        CPU_TEMPERATURE,
        CPU_VOLTAGE,
    };

   // Monitor events (System)
    enum System_Event {
        ELAPSED_TIME,
        DEADLINE_MISSES,
        CPU_EXECUTION_TIME,
        THREAD_EXECUTION_TIME,
        RUNNING_THREAD,
    };

    // Monitor events (PMU)
    enum PMU_Event {
        // SB == Sandy_Bridge; CA = Cortex-A; CA53 = Cortex-A53; CA9 = Cortex-A9;
        COMMITED_INSTRUCTIONS                           = 0,
        CPU_CYCLES                                      = 2,
        BRANCHES                                        = 3,
        BRANCH_MISSES                                   = 4,
        L1_CACHE_HITS                                   = 5, // L1D ACCESS in ARM
        L2_CACHE_HITS                                   = 6, // L2D ACCESS in ARM
        L3_CACHE_MISSES                                 = 8, // not present in ARM using DATA MEMORY ACCESS
        LAST_LEVEL_CACHE_MISSES                         = L3_CACHE_MISSES, // DATA_MEMORY_ACCESS in ARM
        CACHE_MISSES                                    = LAST_LEVEL_CACHE_MISSES,
        L1I_CACHE_MISS                                  = 9,
        L1I_TLB_MISS                                    = 10,
        PREDICTABLE_BRANCHES                            = 11,
        L1D_WRITEBACK                                   = 12,
        L2D_WRITEBACK                                   = 13,
        // BRANCH_HITS                                 = -1, // not in ARM neither Intel

        // ARM Cortex-A specific events
        IMMEDIATE_BRANCHES_CA                           = 1, // NOT Intel
        DATA_MEMORY_ACCESS_CA                           = LAST_LEVEL_CACHE_MISSES,
        L1_CACHE_MISSES_CA                              = 7,  // L1D REFILL in ARM
        L2_CACHE_MISSES_CA                              = 14, // L2D REFILL in ARM
        UNALIGNED_LOAD_STORES_CA,
        L1I_HIT_CA, // L1I_ACCESS
        L1D_TLB_MISS_CA, // L1D_TLB_REFILL
        EXCEPTION_TAKEN_CA,
        BUS_ACCESS_CA,
        LOCAL_MEMORY_ERROR_CA,
        INSTRUCTION_SPECULATIVELY_EXECUTED_CA,
        BUS_CYCLE_CA,
        CHAIN_CA,
        // Cortex-A9 specific events
        JAVA_BYTECODE_EXECUTE_CA9,
        SOFTWARE_JAVA_BYTECODE_EXECUTED_CA9,
        JAZELLE_BACKWARDS_BRANCHES_EXECUTED_CA9,
        COHERENT_LINEFILL_MISS_CA9,
        COHERENT_LINEFILL_HIT_CA9,
        ICA9CHE_DEPENDENT_STALL_CYCLES_CA9,
        DCA9CHE_DEPENDENT_STALL_CYCLES_CA9,
        MAIN_TLB_MISS_STALL_CYCLES_CA9,
        STREX_PASSED_CA9,
        STREX_FAILED_CA9,
        DATA_EVICTION_CA9,
        ISSUE_DOESNT_DISPATCH_CA9,
        ISSUE_EMPTY_CA9,
        ISSUE_CORE_RENAMING_CA9,
        PREDICTABLE_FUNCTION_RETURNS_CA9,
        MAIN_EXECUTION_UNIT_RETURNS_CA9,
        SECOND_EXECUTION_UNIT_RETURNS_CA9,
        LOAD_STORE_INSTRUCTIONS_CA9,
        FLOATING_POINT_INSTRUCTIONS_CA9,
        NEON_INSTRUCTIONS_CA9,
        PROCESSOR_STALL_PLD_CA9,
        PROCESSOR_STALL_WRITE_MEMORY_CA9,
        PROCESSOR_STALL_ITLB_MISS_CA9,
        PROCESSOR_STALL_DTLB_MISS_CA9,
        PROCESSOR_STALL_IUTLB_MISS_CA9,
        PROCESSOR_STALL_DUTLB_MISS_CA9,
        PROCESSOR_STALL_DMB_CA9,
        INTEGER_CLOCK_ENABLED_CA9,
        DATA_ENGINE_CLOCK_ENABLED_CA9,
        ISB_INSTRUCTIONS_CA9,
        DSB_INSTRUCTIONS_CA9,
        DMB_INSTRUCTIONS_CA9,
        EXTERNAL_INTERRUPTS_CA9,
        PLE_CA9CHE_LINE_REQUEST_COMPLETED_CA9,
        PLE_CA9CHE_LINE_REQUEST_SKIPPED_CA9,
        PLE_FIFO_FLUSH_CA9,
        PLE_REQUEST_COMPLETED_CA9,
        PLE_FIFO_OVERFLOW_CA9,
        PLE_REQUEST_PROGRAMMED_CA9,
        // ARM Cortex-A53 specific events
        BUS_ACCESS_LD_CA53,
        BUS_ACCESS_ST_CA53,
        BR_INDIRECT_SPEC_CA53,
        EXC_IRQ_CA53,
        EXC_FIQ_CA53,
        EXTERNAL_MEM_REQUEST_CA53,
        EXTERNAL_MEM_REQUEST_NON_CA53CHEABLE_CA53,
        PREFETCH_LINEFILL_CA53,
        ICA53CHE_THROTTLE_CA53,
        ENTER_READ_ALLOC_MODE_CA53,
        READ_ALLOC_MODE_CA53,
        PRE_DECODE_ERROR_CA53,
        DATA_WRITE_STALL_ST_BUFFER_FULL_CA53,
        SCU_SNOOPED_DATA_FROM_OTHER_CPU_CA53,
        CONDITIONAL_BRANCH_EXECUTED_CA53,
        IND_BR_MISP_CA53,
        IND_BR_MISP_ADDRESS_MISCOMPARE_CA53,
        CONDITIONAL_BRANCH_MISP_CA53,
        L1_ICA53CHE_MEM_ERROR_CA53,
        L1_DCA53CHE_MEM_ERROR_CA53,
        TLB_MEM_ERROR_CA53,
        EMPTY_DPU_IQ_NOT_GUILTY_CA53,
        EMPTY_DPU_IQ_ICA53CHE_MISS_CA53,
        EMPTY_DPU_IQ_IMICRO_TLB_MISS_CA53,
        EMPTY_DPU_IQ_PRE_DECODE_ERROR_CA53,
        INTERLOCK_CYCLE_NOT_GUILTY_CA53,
        INTERLOCK_CYCLE_LD_ST_WAIT_AGU_ADDRESS_CA53,
        INTERLOCK_CYCLE_ADV_SIMD_FP_INST_CA53,
        INTERLOCK_CYCLE_WR_STAGE_STALL_BC_MISS_CA53,
        INTERLOCK_CYCLE_WR_STAGE_STALL_BC_STR_CA53,
        // ARMv8 does not have Cortex A9 events.
        BUS_ACCESS_LD_CA53_v8 = 24,
        BUS_ACCESS_ST_CA53_v8,
        BR_INDIRECT_SPEC_CA53_v8,
        EXC_IRQ_CA53_v8,
        EXC_FIQ_CA53_v8,
        EXTERNAL_MEM_REQUEST_CA53_v8,
        EXTERNAL_MEM_REQUEST_NON_CA53CHEABLE_CA53_v8,
        PREFETCH_LINEFILL_CA53_v8,
        ICA53CHE_THROTTLE_CA53_v8,
        ENTER_READ_ALLOC_MODE_CA53_v8,
        READ_ALLOC_MODE_CA53_v8,
        PRE_DECODE_ERROR_CA53_v8,
        DATA_WRITE_STALL_ST_BUFFER_FULL_CA53_v8,
        SCU_SNOOPED_DATA_FROM_OTHER_CPU_CA53_v8,
        CONDITIONAL_BRANCH_EXECUTED_CA53_v8,
        IND_BR_MISP_CA53_v8,
        IND_BR_MISP_ADDRESS_MISCOMPARE_CA53_v8,
        CONDITIONAL_BRANCH_MISP_CA53_v8,
        L1_ICA53CHE_MEM_ERROR_CA53_v8,
        L1_DCA53CHE_MEM_ERROR_CA53_v8,
        TLB_MEM_ERROR_CA53_v8,
        EMPTY_DPU_IQ_NOT_GUILTY_CA53_v8,
        EMPTY_DPU_IQ_ICA53CHE_MISS_CA53_v8,
        EMPTY_DPU_IQ_IMICRO_TLB_MISS_CA53_v8,
        EMPTY_DPU_IQ_PRE_DECODE_ERROR_CA53_v8,
        INTERLOCK_CYCLE_NOT_GUILTY_CA53_v8,
        INTERLOCK_CYCLE_LD_ST_WAIT_AGU_ADDRESS_CA53_v8,
        INTERLOCK_CYCLE_ADV_SIMD_FP_INST_CA53_v8,
        INTERLOCK_CYCLE_WR_STAGE_STALL_BC_MISS_CA53_v8,
        INTERLOCK_CYCLE_WR_STAGE_STALL_BC_STR_CA53_v8,

        // Intel Sandy Bridge specific events
        UNHALTED_REFERENCE_CYCLES_SB                    = 1,
        L3_CACHE_HITS_SB                                = 7, // not in ARM
        LAST_LEVEL_CACHE_HITS_SB                        = L3_CACHE_HITS_SB, // not in ARM
        CACHE_HITS_SB                                   = LAST_LEVEL_CACHE_HITS_SB, //not in ARM
        MEM_UOP_RETIRED_ALL_SB                          = 14,
        L1D_EVICTION_SB,
        IDQ_UOPS_NOT_DELIVERED_CORE_SB,
        IDQ_EMPTY_SB,
        BR_MISP_EXEC_RETURN_NEAR_SB,
        FP_COMP_OPS_EXE_X87_SB,
        DTLB_STORE_MISSES_WALK_DURATION_SB,
        DTLB_LOAD_MISSES_MISS_WALK_DURATION_SB,
        BR_INST_EXEC_RETURN_NEAR_SB,
        RESOURCE_STALLS_SB_SB,
        XSNP_HIT_SB,
        BR_MISP_EXEC_INDIRECT_JMP_NON_CALL_RET_SB,
        BR_MISP_RETIRED_CONDITIONAL_SB,
        LD_BLOCKS_DATA_UNKNOWN_SB,
        LD_BLOCKS_STORE_FORWARD_SB,
        LD_BLOCKS_NO_SR_SB,
        LD_BLOCKS_ALL_BLOCK_SB,
        MISALIGN_MEM_REF_LOADS_SB,
        MISALIGN_MEM_REF_STORES_SB,
        LD_BLOCKS_PARTIAL_ADDRESS_ALIAS_SB,
        LD_BLOCKS_PARTIAL_ALL_STA_BLCOK_SB,
        DTLB_LOAD_MISSES_MISS_CAUSES_A_WALK_SB,
        DTLB_LOAD_MISSES_MISS_WALK_COMPLETED_SB,
        DTLB_LOAD_MISSES_MISS_STLB_HIT_SB,
        INT_MISC_RECOVERY_CYCLES_SB,
        INT_MISC_RAT_STALL_CYCLES_SB,
        UOPS_ISSUED_ANY_SB,
        FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE_SB,
        FP_COMP_OPS_EXE_SSE_FP_SCALAR_SINGLE_SB,
        FP_COMP_OPS_EXE_SSE_PACKED_SINGLE_SB,
        FP_COMP_OPS_EXE_SSE_SCALAR_DOUBLE_SB,
        SIMD_FP_256_PACKED_SINGLE_SB,
        SIMD_FP_256_PACKED_DOUBLE_SB,
        ARITH_FPU_DIV_ACTIVE_SB,
        INSTS_WRITTEN_TO_IQ_INSTS_SB,
        L2_RQSTS_DEMAND_DATA_RD_HIT_SB,
        L2_RQSTS_ALL_DEMAND_DATA_RD_SB,
        L2_RQSTS_RFO_HITS_SB,
        L2_RQSTS_RFO_MISS_SB,
        L2_RQSTS_ALL_RFO_SB,
        L2_RQSTS_CODE_RD_HIT_SB,
        L2_RQSTS_CODE_RD_MISS_SB,
        L2_RQSTS_ALL_CODE_RD_SB,
        L2_RQSTS_PF_HIT_SB,
        L2_RQSTS_PF_MISS_SB,
        L2_RQSTS_ALL_PF_SB,
        L2_STORE_LOCK_RQSTS_MISS_SB,
        L2_STORE_LOCK_RQSTS_HIT_E_SB,
        L2_STORE_LOCK_RQSTS_HIT_M_SB,
        L2_STORE_LOCK_RQSTS_ALL_SB,
        L2_L1D_WB_RQSTS_HIT_E_SB,
        L2_L1D_WB_RQSTS_HIT_M_SB,
        L2_TRANS_DEMAND_DATA_RD_SB,
        L2_TRANS_RFO_SB,
        L2_TRANS_CODE_RD_SB,
        L2_TRANS_ALL_PF_SB,
        L2_TRANS_L2_FILL_SB,
        L2_TRANS_ALL_REQ_UESTS_SB,
        L2_LINES_IN_I_SB,
        L2_LINES_IN_S_SB,
        L2_LINES_IN_E_SB,
        L2_LINES_IN_ALL_SB,
        L2_LINES_OUT_DEMAND_CLEAN_SB,
        L2_LINES_OUT_DEMAND_DIRTY_SB,
        L2_LINES_OUT_DEMAND_PF_CLEAN_SB,
        L2_LINES_OUT_DEMAND_PF_DIRTY_SB,
        L2_LINES_OUT_DEMAND_DIRTY_ALL_SB,
        LONGEST_LAT_CACHE_REFERENCE_SB,
        LONGEST_LAT_CACHE_MISS_SB,
        CPU_CLK_UNHALTED_THREAD_P_SB,
        CPU_CLK_THREAD_UNHALTED_REF_XCLK_SB,
        L1D_PEND_MISS_PENDING_SB,
        DTLB_STORE_MISSES_MISS_CAUSES_A_WALK_SB,
        DTLB_STORE_MISSES_WALK_COMPLETED_SB,
        DTLB_STORE_MISSES_TLB_HIT_SB,
        LOAD_HIT_PRE_SW_PF_SB,
        LOAD_HIT_PREHW_PF_SB,
        HW_PRE_REQ_DL1_MISS_SB,
        L1D_REPLACEMENT_SB,
        L1D_ALLOCATED_IN_M_SB,
        L1D_ALL_M_REPLACEMENT_SB,
        PARTIAL_RAT_STALLS_FLAGS_MERGE_UOP_SB,
        PARTIAL_RAT_STALLS_SLOW_LEA_WINDOW_SB,
        PARTIAL_RAT_STALLS_MUL_SINGLE_UOP_SB,
        RESOURCE_STALLS2_ALL_FL_EMPTY_SB,
        RESOURCE_STALLS2_ALL_PRF_CONTROL_SB,
        RESOURCE_STALLS2_BOB_FULL_SB,
        RESOURCE_STALLS2_OOO_RSRC_SB,
        CPL_CYCLES_RING0_SB,
        CPL_CYCLES_RING123_SB,
        RS_EVENTS_EMPTY_CYCLES_SB,
        OFFCORE_REQUESTS_OUTSTANDING_DEMAND_DATA_RD_SB,
        OFFCORE_REQUESTS_OUTSTANDING_DEMAND_RFO_SB,
        OFFCORE_REQUESTS_OUTSTANDING_ALL_DATA_RD_SB,
        LOCK_CYCLES_SPLIT_LOCK_UC_LOCK_DURATION_SB,
        LOCK_CYCLES_CACHE_LOCK_DURATION_SB,
        IDQ_MITE_UOPS_SB,
        IDQ_DSB_UOPS_SB,
        IDQ_MS_DSB_UOPS_SB,
        IDQ_MS_MITE_UOPS_SB,
        IDQ_MS_UOPS_SB,
        ITLB_MISSES_MISS_CAUSES_A_WALK_SB,
        ITLB_MISSES_WALK_COMPLETED_SB,
        ITLB_MISSES_WALK_DURATION_SB,
        ITLB_MISSES_STLB_HIT_SB,
        ILD_STALL_LCP_SB,
        ILD_STALL_IQ_FULL_SB,
        BR_INST_EXEC_DIRECT_JMP_SB,
        BR_INST_EXEC_INDIRECT_JMP_NON_CALL_RET_SB,
        BR_INST_EXEC_DIRECT_NEAR_CALL_SB,
        BR_INST_EXEC_INDIRECT_NEAR_CALL_SB,
        BR_INST_EXEC_NON_TAKEN_SB,
        BR_INST_EXEC_TAKEN_SB,
        BR_INST_EXEC_ALL_BRANCHES_SB,
        BR_MISP_EXEC_COND_SB,
        BR_MISP_EXEC_DIRECT_NEAR_CALL_SB,
        BR_MISP_EXEC_INDIRECT_NEAR_CALL_SB,
        BR_MISP_EXEC_NON_TAKEN_SB,
        BR_MISP_EXEC_TAKEN_SB,
        BR_MISP_EXEC_ALL_BRANCHES_SB,
        UOPS_DISPATCHED_PORT_PORT_0_SB,
        UOPS_DISPATCHED_PORT_PORT_1_SB,
        UOPS_DISPATCHED_PORT_PORT_2_LD_SB,
        UOPS_DISPATCHED_PORT_PORT_2_STA_SB,
        UOPS_DISPATCHED_PORT_PORT_2_SB,
        UOPS_DISPATCHED_PORT_PORT_3_LD_SB,
        UOPS_DISPATCHED_PORT_PORT_3_STA_SB,
        UOPS_DISPATCHED_PORT_PORT_3_SB,
        UOPS_DISPATCHED_PORT_PORT_4_SB,
        UOPS_DISPATCHED_PORT_PORT_5_SB,
        RESOURCE_STALLS_ANY_SB,
        RESOURCE_STALLS_LB_SB,
        RESOURCE_STALLS_RS_SB,
        RESOURCE_STALLS_ROB_SB,
        RESOURCE_STALLS_FCSW_SB,
        RESOURCE_STALLS_MXCSR_SB,
        RESOURCE_STALLS_OTHER_SB,
        DSB2MITE_SWITCHES_COUNT_SB,
        DSB2MITE_SWITCHES_PENALTY_CYCLES_SB,
        DSB_FILL_OTHER_CANCEL_SB,
        DSB_FILL_EXCEED_DSB_LINES_SB,
        DSB_FILL_ALL_CANCEL_SB,
        ITLB_ITLB_FLUSH_SB,
        OFFCORE_REQUESTS_DEMAND_DATA_RD_SB,
        OFFCORE_REQUESTS_DEMAND_RFO_SB,
        OFFCORE_REQUESTS_ALL_DATA_RD_SB,
        UOPS_DISPATCHED_THREAD_SB,
        UOPS_DISPATCHED_CORE_SB,
        OFFCORE_REQUESTS_BUFFER_SQ_FULL_SB,
        AGU_BYPASS_CANCEL_COUNT_SB,
        OFF_CORE_RESPONSE_0_SB,
        OFF_CORE_RESPONSE_1_SB,
        TLB_FLUSH_DTLB_THREAD_SB,
        TLB_FLUSH_STLB_ANY_SB,
        L1D_BLOCKS_BANK_CONFLICT_CYCLES_SB,
        INST_RETIRED_ANY_P_SB,
        INST_RETIRED_PREC_DIST_SB,
        OTHER_ASSISTS_AVX_STORE_SB,
        OTHER_ASSISTS_AVX_TO_SSE_SB,
        OTHER_ASSISTS_SSE_TO_AVX_SB,
        UOPS_RETIRED_ALL_SB,
        UOPS_RETIRED_RETIRE_SLOTS_SB,
        MACHINE_CLEARS_MEMORY_ORDERING_SB,
        MACHINE_CLEARS_SMC_SB,
        MACHINE_CLEARS_MASKMOV_SB,
        BR_INST_RETIRED_ALL_BRANCHES_ARCH_SB,
        BR_INST_RETIRED_CONDITIONAL_SB,
        BR_INST_RETIRED_NEAR_CALL_SB,
        BR_INST_RETIRED_ALL_BRANCHES_SB,
        BR_INST_RETIRED_NEAR_RETURN_SB,
        BR_INST_RETIRED_NOT_TAKEN_SB,
        BR_INST_RETIRED_NEAR_TAKEN_SB,
        BR_INST_RETIRED_FAR_BRANCH_SB,
        BR_MISP_RETIRED_ALL_BRANCHES_ARCH_SB,
        BR_MISP_RETIRED_NEAR_CALL_SB,
        BR_MISP_RETIRED_ALL_BRANCHES_SB,
        BR_MISP_RETIRED_NOT_TAKEN_SB,
        BR_MISP_RETIRED_TAKEN_SB,
        FP_ASSIST_X87_OUTPUT_SB,
        FP_ASSIST_X87_INPUT_SB,
        FP_ASSIST_SIMD_OUTPUT_SB,
        FP_ASSIST_SIMD_INPUT_SB,
        FP_ASSIST_ANY_SB,
        ROB_MISC_EVENTS_LBR_INSERTS_SB,
        MEM_TRANS_RETIRED_LOAD_LATENCY_SB,
        MEM_TRANS_RETIRED_PRECISE_STORE_SB,
        MEM_UOP_RETIRED_LOADS_SB,
        MEM_UOP_RETIRED_STORES_SB,
        MEM_UOP_RETIRED_STLB_MISS_SB,
        MEM_UOP_RETIRED_LOCK_SB,
        MEM_UOP_RETIRED_SPLIT_SB,
        MEM_UOPS_RETIRED_ALL_LOADS_SB,
        MEM_LOAD_UOPS_RETIRED_L3_HIT_SB,
        MEM_LOAD_UOPS_RETIRED_HIT_LFB_SB,
        XSNP_MISS_SB,
        XSNP_HITM_SB,
        XSNP_NONE_SB,
        MEM_LOAD_UOPS_MISC_RETIRED_LLC_MISS_SB,
        SQ_MISC_SPLIT_LOCK_SB
    };
};

template<typename T>
struct Traits {
    // Traits for components that do not declare any
    static const bool enabled = true;
    static const bool monitored = false;
    static const bool debugged = true;
    static const bool hysterically_debugged = false;

    typedef ALIST<> ASPECTS;
};

__END_SYS

#endif
