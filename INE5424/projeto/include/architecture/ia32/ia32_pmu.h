// EPOS IA32 PMU Mediator Declarations

#ifndef __ia32_pmu_h
#define __ia32_pmu_h

#include <architecture/cpu.h>
#include <architecture/pmu.h>
#include <machine/ic.h>
#include <utility/handler.h>

__BEGIN_SYS

class Intel_PMU_V1: public PMU_Common
{
protected:
    typedef CPU::Reg8  Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg32 Reg32;
    typedef CPU::Reg64 Reg64;
    typedef CPU::Log_Addr Log_Addr;
    typedef IC::Interrupt_Id Interrupt_Id;

    static const unsigned int CHANNELS = 3;
    static const unsigned int FIXED = 0;
    static const unsigned int EVENTS = 7;

public:
    // Architectural PM Version 1 Section 30.2.1.1
    // MAR address range between 0x40000000 to 0x400000FF
    enum {
        MSR_BASE = 0x40000000
    };

    // Performance Monitoring Counters - used as input to the rdpmc instruction
    enum {
        PMC0 = 0x0000, // PERFCTR0
        PMC1 = 0x0001, // PERFCTR1
        PMC2 = 0x0002,
        PMC3 = 0x0003,
        PMC4 = 0x0004,
        PMC5 = 0x0005,
        PMC6 = 0x0006,
        PMC7 = 0x0007,
    };

    // Memory-mapped PMU registers - Appendix B - MSRs
    enum {
        PMC_BASE_ADDR = 0x00c1,
        MPERF         = 0x00e7,
        APERF         = 0x00e8,
        EVTSEL0       = 0x0186,
        EVTSEL1       = 0x0187,
        EVTSEL2       = 0x0188,
        EVTSEL3       = 0x0189,
        STATUS        = 0x0198,
        CTL           = 0x0199,
        FIXED_CTR0    = 0x0309,
        FIXED_CTR1    = 0x030a,
        FIXED_CTR2    = 0x030b,
        CAPABILITIES  = 0x0345,
        FIXED_CTR_CTL = 0x038d,
        GLOBAL_STATUS = 0x038e,
        GLOBAL_CTRL   = 0x038f,
        GLOBAL_OVF    = 0x0390,
        PEBS_ENABLE   = 0x03F1
    };

    // Flags
    enum {
        PCE     = 1 << 8,
        USR     = 1 << 16,
        OS      = 1 << 17,
        EDGE    = 1 << 18,
        PC      = 1 << 19,
        INT     = 1 << 20,
        LOGICAL = 1 << 21,
        ENABLE  = 1 << 22,
        INV     = 1 << 23, // invert the CMASK
    };

    // Predefined architectural performance events
    enum {
        // Event                         Select  UMask
        UNHALTED_CORE_CYCLES            = 0x3c | (0x00 << 8),
        INSTRUCTIONS_RETIRED            = 0xc0 | (0x00 << 8),
        UNHALTED_REFERENCE_CYCLES       = 0x3c | (0x01 << 8),
        LLC_REFERENCES                  = 0x2e | (0x4f << 8),
        LLC_MISSES                      = 0x2e | (0x41 << 8),
        BRANCH_INSTRUCTIONS_RETIRED     = 0xc4 | (0x00 << 8),
        BRANCH_MISSES_RETIRED           = 0xc5 | (0x00 << 8)
    };

public:
    Intel_PMU_V1() {}

    static void config(Channel channel, Event event, Flags flags = NONE) {
        assert((channel < CHANNELS) && (event < EVENTS));
        db<PMU>(TRC) << "PMU::config(c=" << channel << ",e=" << event << ",f=" << flags << ")" << endl;
        wrmsr(EVTSEL0 + channel, _events[event] | USR | OS | ENABLE | flags); // implicitly start counting due to flag ENABLE
    }

    static Count read(Channel channel) {
        db<PMU>(TRC) << "PMU::read(c=" << channel << ")" << endl;
        return rdpmc(channel);
    }

    static void write(Channel channel, Count count) {
        db<PMU>(TRC) << "PMU::write(ch=" << channel << ",ct=" << count << ")" << endl;
        wrmsr(PMC_BASE_ADDR + channel, count);
    }

    static void start(Channel channel) {
        db<PMU>(TRC) << "PMU::start(c=" << channel << ")" << endl;
        wrmsr(EVTSEL0 + channel, (rdmsr(EVTSEL0 + channel) | ENABLE));
    }

    static void stop(Channel channel) {
        db<PMU>(TRC) << "PMU::stop(c=" << channel << ")" << endl;
        wrmsr(EVTSEL0 + channel, (rdmsr(EVTSEL0 + channel) & ~ENABLE));
    }

    static void reset(Channel channel) {
        db<PMU>(TRC) << "PMU::reset(c=" << channel << ")" << endl;
        wrmsr(EVTSEL0 + channel, 0);
    }

protected:
    static Reg64 rdmsr(Reg32 msr) { return CPU::rdmsr(msr); }
    static void wrmsr(Reg32 msr, Reg64 val) { CPU::wrmsr(msr, val); }
    static Reg64 rdpmc(Reg32 counter) {
        Reg64 value;
        ASM("rdpmc" : "=A" (value) : "c" (counter));
        return value;
    }

protected:
    static constexpr Event _events[EVENTS] = {
        // Architecture              // API
        INSTRUCTIONS_RETIRED,        // INSTRUCTION
        UNHALTED_REFERENCE_CYCLES,   // DVS_CLOCK
        UNHALTED_CORE_CYCLES,        // CLOCK
        BRANCH_INSTRUCTIONS_RETIRED, // BRANCH
        BRANCH_MISSES_RETIRED,       // BRANCH_MISS
        LLC_REFERENCES,              // L3_HIT
        LLC_MISSES,                  // L3_MISS
    };
};

class Intel_PMU_V2: public Intel_PMU_V1
{
protected:
    static const unsigned int CHANNELS = 5;
    static const unsigned int FIXED = 3;

public:
    // Meaningful bits in FIXED_CTR_CTRL MSR
    enum {
        CRT0_ENABLE_SYS    = 0,
        CRT0_ENABLE_USR    = 1,
        CRT0_ENABLE_INT    = 3,
        CRT1_ENABLE_SYS    = 4,
        CRT1_ENABLE_USR    = 5,
        CRT1_ENABLE_INT    = 7,
        CRT2_ENABLE_SYS    = 8,
        CRT2_ENABLE_USR    = 9,
        CRT2_ENABLE_INT    = 11
    };

    // Meaningful bits in GLOBAL_CTRL MSR
    enum {
        PMC0_ENABLE        = 0,
        PMC1_ENABLE        = 1,
        CRT0_ENABLE        = 32,
        CRT1_ENABLE        = 33,
        CRT2_ENABLE        = 34
    };

    // Meaningful bits in GLOBAL_STATUS and GLOBAL_OVF_CTRL MSR
    enum {
        PMC0_OVERFLOW      = 0,
        PMC1_OVERFLOW      = 1,
        CRT0_OVERFLOW      = 32,
        CRT1_OVERFLOW      = 33,
        CRT2_OVERFLOW      = 34,
        FIXED_CTR0_OVF     = (1LLU << 0x20),        //TODO: confirm
        FIXED_CTR1_OVF     = (1LLU << 0x21),        //TODO: confirm
        FIXED_CTR2_OVF     = (1LLU << 0x22),        //TODO: confirm
        COND_CHGD          = (1LLU << 0x3F)         //TODO: confirm
    };

    // Meaningful bits in FIXED_CRTs
    enum {
            // FIXED_CTR_CTRL0
            FIXED_CTR0_OS           = 0x01,
            FIXED_CTR0_USER         = 0x02,
            FIXED_CTR0_ALL          = 0x03,
            FIXED_CTR0_PMI          = (0x01 << 3),
            // FIXED_CTR_CTRL1
            FIXED_CTR1_OS           = (0x01 << 4),
            FIXED_CTR1_USER         = (0x02 << 4),
            FIXED_CTR1_ALL          = (0x03 << 4),
            FIXED_CTR1_PMI          = (0x01 << 7),
            // FIXED_CTR_CTRL2
            FIXED_CTR2_OS           = (0x01 << 8),
            FIXED_CTR2_USER         = (0x02 << 8),
            FIXED_CTR2_ALL          = (0x03 << 8),
            FIXED_CTR2_PMI          = (0x01 << 11),

            FIXED_CTR0_ENABLE       = (0x1LLU << 32),
            FIXED_CTR1_ENABLE       = (0x1LLU << 33),
            FIXED_CTR2_ENABLE       = (0x1LLU << 34),
    };

public:
    Intel_PMU_V2() {}

    static Reg64 global_overflow_status(void) { 
        db<Intel_PMU_V1>(TRC) << "Intel_PMU_V1::overflow()" << endl; 
        return rdmsr(GLOBAL_STATUS); 
    }

    static bool cond_changed(void) {
        db<Intel_PMU_V1>(TRC) << "Intel_PMU_V1::cond_changed()" << endl;
        return ((rdmsr(GLOBAL_STATUS) & COND_CHGD) != 0);
    }

    static void clear_cond_changed(void) {
        wrmsr(GLOBAL_OVF, COND_CHGD);
    }

protected:
    static Handler * _handlers[CHANNELS - FIXED];
};

// TODO: Refactoring stoped at V2. Someone with a real machine must continue the procedure following the model
class Intel_PMU_V3: public Intel_PMU_V2
{
public:
    // flags
    enum {
        // PERFEVTSEL MSR any thread bit figure 18-16
        ANY_THREAD = (0x01 << 21),
    };

    // FIXED_CTR_CTL MSR any thread bit
    enum {
        ANY_THREAD_FIXED0 = (0x01 << 2),
        ANY_THREAD_FIXED1 = (0x01 << 6),
        ANY_THREAD_FIXED2 = (0x01 << 10),
    };

public:
    Intel_PMU_V3() {}
};

class Intel_Sandy_Bridge_PMU: public Intel_PMU_V3
{
protected:
    static const unsigned int CHANNELS = 7;
    static const unsigned int FIXED = 3;
    static const unsigned int EVENTS = 213;

public:
    // Layout of IA32_PEBS_ENABLE MSR figure 18-28
    enum {
        PS_EN                = (0x01LLU << 63),
        LL_EN_PMC3           = (0x01LLU << 35),
        LL_EN_PMC2           = (0x01LLU << 34),
        LL_EN_PMC1           = (0x01LLU << 33),
        LL_EN_PMC0           = (0x01LLU << 32),
        PEBS_EN_PMC3         = (0x01LLU << 3),
        PEBS_EN_PMC2         = (0x01LLU << 2),
        PEBS_EN_PMC1         = (0x01LLU << 1),
        PEBS_EN_PMC0         = 0x01LLU,
        RESET_PEBS           = 0x00LLU
    };

    // the two uncore MSR addresses
    enum {
        OFFCORE_RSP_0        = 0x1a6,
        OFFCORE_RSP_1        = 0x1a7,
        RESET_VALUE          = 0x00000000
    };

    // Request type fields bits for OFFCORE_RSP_x
    // bits 15:0
    enum {
        DMND_DATA_RD   = 0x0000000001LLU, // bit 0
        DMND_RFO       = 0x0000000002LLU, // bit 1
        DMND_IFETCH    = 0x0000000004LLU, // bit 2
        WB             = 0x0000000008LLU, // bit 3
        PF_DATA_RD     = 0x0000000010LLU, // bit 4
        PF_RFO         = 0x0000000020LLU, // bit 5
        PF_IFETCH      = 0x0000000040LLU, // bit 6
        PF_LLC_DATA_RD = 0x0000000080LLU, // bit 7
        PF_LLC_RFO     = 0x0000000100LLU, // bit 8
        PF_LLC_IFETCH  = 0x0000000200LLU, // bit 9
        BUS_LOCKS      = 0x0000000400LLU, // bit 10
        STRM_ST        = 0x0000000800LLU, // bit 11
        // bits 12-14 are reserved
        OTHER          = 0x0000008000LLU  // bit 15
    };

    // Response supplier and snoop info field bits for OFFCORE_RSP_x
    // bits 30:16
    enum {
      ANY            = 0x0000010000LLU, // bit 16 RSPNS_SUPPLIER
      NO_SUPP        = 0x0000020000LLU, // bit 17 RSPNS_SUPPLIER
      LLC_HITM       = 0x0000040000LLU, // bit 18 RSPNS_SUPPLIER
      LLC_HITE       = 0x0000080000LLU, // bit 19 RSPNS_SUPPLIER
      LLC_HITS       = 0x0000100000LLU, // bit 20 RSPNS_SUPPLIER
      LLC_HITF       = 0x0000200000LLU, // bit 21 RSPNS_SUPPLIER
      LOCAL          = 0x0000400000LLU, // bit 22 RSPNS_SUPPLIER
      // bits 23-30 are reserved RSPNS_SUPPLIER
      SNPI_NONE      = 0x0080000000LLU, // bit 31 RSPNS_SNOOP
      SNP_NOT_NEEDED = 0x0100000000LLU, // bit 32 RSPNS_SNOOP
      SNP_MISS       = 0x0200000000LLU, // bit 33 RSPNS_SNOOP
      HIT_NO_FWD     = 0x0400000000LLU, // bit 34 RSPNS_SNOOP
      HIT_FWD        = 0x0800000000LLU, // bit 35 RSPNS_SNOOP
      HITM           = 0x1000000000LLU, // bit 36 RSPNS_SNOOP
      NON_DRAM       = 0x2000000000LLU, // bit 37 RSPNS_SNOOP
    };

    //off-core response event enconding (table 18-24)
    enum {
        PMC0_UNCORE = 0xb7 | (0x1 << 8), //requires OFFCORE_RSP_0
        PMC3_UNCORE = 0xbb | (0x1 << 8), //requires OFFCORE_RSP_1
    };

    enum{
        PMC_MASK = (0xffffffff >> (32 - CHANNELS - FIXED))
    };

    // List of all supported events - Section 19.3 - Table 19-3
    enum {
        LD_BLOCKS_DATA_UNKNOWN                          = 0x03 | (0x01 << 8),
        LD_BLOCKS_STORE_FORWARD                         = 0x03 | (0x02 << 8),
        LD_BLOCKS_NO_SR                                 = 0x03 | (0x8 << 8),
        LD_BLOCKS_ALL_BLOCK                             = 0x03 | (0x10 << 8),

        MISALIGN_MEM_REF_LOADS                          = 0x05 | (0x01 << 8),
        MISALIGN_MEM_REF_STORES                         = 0x05 | (0x02 << 8),

        LD_BLOCKS_PARTIAL_ADDRESS_ALIAS                 = 0x07 | (0x01 << 8),
        LD_BLOCKS_PARTIAL_ALL_STA_BLCOK                 = 0x07 | (0x08 << 8),

        DTLB_LOAD_MISSES_MISS_CAUSES_A_WALK             = 0x08 | (0x01 << 8),
        DTLB_LOAD_MISSES_MISS_WALK_COMPLETED            = 0x08 | (0x02 << 8),
        DTLB_LOAD_MISSES_MISS_WALK_DURATION             = 0x08 | (0x04 << 8),
        DTLB_LOAD_MISSES_MISS_STLB_HIT                  = 0x08 | (0x10 << 8),

        INT_MISC_RECOVERY_CYCLES                        = 0x0d | (0x03 << 8), // set edge to count occurences
        INT_MISC_RAT_STALL_CYCLES                       = 0x0d | (0x40 << 8),

        UOPS_ISSUED_ANY                                 = 0x0e | (0x01 << 8), //set cmmask = 1, inv = 1 to count stalled cycles

        FP_COMP_OPS_EXE_X87                             = 0x10 | (0x01 << 8),
        FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE            = 0x10 | (0x10 << 8),
        FP_COMP_OPS_EXE_SSE_FP_SCALAR_SINGLE            = 0x10 | (0x20 << 8),
        FP_COMP_OPS_EXE_SSE_PACKED_SINGLE               = 0x10 | (0x40 << 8),
        FP_COMP_OPS_EXE_SSE_SCALAR_DOUBLE               = 0x10 | (0x80 << 8),

        SIMD_FP_256_PACKED_SINGLE                       = 0x11 | (0x01 << 8),
        SIMD_FP_256_PACKED_DOUBLE                       = 0x11 | (0x02 << 8),

        ARITH_FPU_DIV_ACTIVE                            = 0x12 | (0x01 << 8),

        INSTS_WRITTEN_TO_IQ_INSTS                       = 0x17 | (0x01 << 8),

        L2_RQSTS_DEMAND_DATA_RD_HIT                     = 0x24 | (0x01 << 8),
        L2_RQSTS_ALL_DEMAND_DATA_RD                     = 0x24 | (0x03 << 8),
        L2_RQSTS_RFO_HITS                               = 0x24 | (0x04 << 8),
        L2_RQSTS_RFO_MISS                               = 0x24 | (0x08 << 8),
        L2_RQSTS_ALL_RFO                                = 0x24 | (0x0c << 8),
        L2_RQSTS_CODE_RD_HIT                            = 0x24 | (0x10 << 8),
        L2_RQSTS_CODE_RD_MISS                           = 0x24 | (0x20 << 8),
        L2_RQSTS_ALL_CODE_RD                            = 0x24 | (0x30 << 8),
        L2_RQSTS_PF_HIT                                 = 0x24 | (0x40 << 8),
        L2_RQSTS_PF_MISS                                = 0x24 | (0x80 << 8),
        L2_RQSTS_ALL_PF                                 = 0x24 | (0xc0 << 8),

        L2_STORE_LOCK_RQSTS_MISS                        = 0x27 | (0x01 << 8),
        L2_STORE_LOCK_RQSTS_HIT_E                       = 0x27 | (0x04 << 8),
        L2_STORE_LOCK_RQSTS_HIT_M                       = 0x27 | (0x08 << 8),
        L2_STORE_LOCK_RQSTS_ALL                         = 0x27 | (0x0f << 8),

        L2_L1D_WB_RQSTS_HIT_E                           = 0x28 | (0x04 << 8),
        L2_L1D_WB_RQSTS_HIT_M                           = 0x28 | (0x08 << 8),

        LONGEST_LAT_CACHE_REFERENCE                     = 0x2e | (0x4f << 8), //table 19-1 architectural event
        LONGEST_LAT_CACHE_MISS                          = 0x2e | (0x41 << 8), //table 19-1 architectural event

        CPU_CLK_UNHALTED_THREAD_P                       = 0x3c | (0x00 << 8), //table 19-1 architectural event
        CPU_CLK_THREAD_UNHALTED_REF_XCLK                = 0x3c | (0x01 << 8), //table 1901 architectural event

        L1D_PEND_MISS_PENDING                           = 0x48 | (0x01 << 8), //counter 2 only - set cmask = 1 to count cycles

        DTLB_STORE_MISSES_MISS_CAUSES_A_WALK            = 0x49 | (0x01 << 8),
        DTLB_STORE_MISSES_WALK_COMPLETED                = 0x49 | (0x02 << 8),
        DTLB_STORE_MISSES_WALK_DURATION                 = 0x49 | (0x04 << 8),
        DTLB_STORE_MISSES_TLB_HIT                       = 0x49 | (0x10 << 8),

        LOAD_HIT_PRE_SW_PF                              = 0x4c | (0x01 << 8),
        LOAD_HIT_PREHW_PF                               = 0x4c | (0x02 << 8),

        HW_PRE_REQ_DL1_MISS                             = 0x4e | (0x02 << 8),

        L1D_REPLACEMENT                                 = 0x51 | (0x01 << 8),
        L1D_ALLOCATED_IN_M                              = 0x51 | (0x02 << 8),
        L1D_EVICTION                                    = 0x51 | (0x04 << 8),
        L1D_ALL_M_REPLACEMENT                           = 0x51 | (0x08 << 8),

        PARTIAL_RAT_STALLS_FLAGS_MERGE_UOP              = 0x59 | (0x20 << 8),
        PARTIAL_RAT_STALLS_SLOW_LEA_WINDOW              = 0x59 | (0x40 << 8),
        PARTIAL_RAT_STALLS_MUL_SINGLE_UOP               = 0x59 | (0x80 << 8),

        RESOURCE_STALLS2_ALL_FL_EMPTY                   = 0x5b | (0x0c << 8),
        RESOURCE_STALLS2_ALL_PRF_CONTROL                = 0x5b | (0x0f << 8),
        RESOURCE_STALLS2_BOB_FULL                       = 0x5b | (0x40 << 8),
        RESOURCE_STALLS2_OOO_RSRC                       = 0x5b | (0x4f << 8),

        CPL_CYCLES_RING0                                = 0x5c | (0x01 << 8), //use edge to count transition
        CPL_CYCLES_RING123                              = 0x5c | (0x02 << 8),

        RS_EVENTS_EMPTY_CYCLES                          = 0x5e | (0x01 << 8),

        OFFCORE_REQUESTS_OUTSTANDING_DEMAND_DATA_RD     = 0x60 | (0x01 << 8),
        OFFCORE_REQUESTS_OUTSTANDING_DEMAND_RFO         = 0x60 | (0x04 << 8),
        OFFCORE_REQUESTS_OUTSTANDING_ALL_DATA_RD        = 0x60 | (0x08 << 8),

        LOCK_CYCLES_SPLIT_LOCK_UC_LOCK_DURATION         = 0x63 | (0x01 << 8),
        LOCK_CYCLES_CACHE_LOCK_DURATION                 = 0x63 | (0x02 << 8),

        IDQ_EMPTY                                       = 0x79 | (0x02 << 8),
        IDQ_MITE_UOPS = 0x79 | (0x04 << 8),
        IDQ_DSB_UOPS = 0x79 | (0x08 << 8),
        IDQ_MS_DSB_UOPS = 0x79 | (0x10 << 8),
        IDQ_MS_MITE_UOPS = 0x79 | (0x20 << 8),
        IDQ_MS_UOPS = 0x79 | (0x30 << 8),

        ICACHE_MISSES = 0x80 | (0x02 << 8),

        ITLB_MISSES_MISS_CAUSES_A_WALK = 0x85 | (0x01 << 8),
        ITLB_MISSES_WALK_COMPLETED = 0x85 | (0x02 << 8),
        ITLB_MISSES_WALK_DURATION = 0x85 | (0x04 << 8),
        ITLB_MISSES_STLB_HIT = 0x85 | (0x10 << 8),

        ILD_STALL_LCP = 0x87 | (0x01 << 8),
        ILD_STALL_IQ_FULL = 0x87 | (0x04 << 8),

        BR_INST_EXEC_COND = 0x88 | (0x01 << 8),
        BR_INST_EXEC_DIRECT_JMP = 0x88 | (0x02 << 8),
        BR_INST_EXEC_INDIRECT_JMP_NON_CALL_RET = 0x88 | (0x04 << 8),
        BR_INST_EXEC_RETURN_NEAR = 0x88 | (0x08 << 8),
        BR_INST_EXEC_DIRECT_NEAR_CALL = 0x88 | (0x10 << 8),
        BR_INST_EXEC_INDIRECT_NEAR_CALL = 0x88 | (0x20 << 8),
        BR_INST_EXEC_NON_TAKEN = 0x88 | (0x40 << 8),
        BR_INST_EXEC_TAKEN = 0x88 | (0x80 << 8),
        BR_INST_EXEC_ALL_BRANCHES = 0x88 | (0xff << 8),

        BR_MISP_EXEC_COND = 0x89 | (0x01 << 8),
        BR_MISP_EXEC_INDIRECT_JMP_NON_CALL_RET = 0x89 | (0x04 << 8),
        BR_MISP_EXEC_RETURN_NEAR = 0x89 | (0x08 << 8),
        BR_MISP_EXEC_DIRECT_NEAR_CALL = 0x89 | (0x10 << 8),
        BR_MISP_EXEC_INDIRECT_NEAR_CALL = 0x89 | (0x20 << 8),
        BR_MISP_EXEC_NON_TAKEN = 0x89 | (0x40 << 8),
        BR_MISP_EXEC_TAKEN = 0x89 | (0x80 << 8),
        BR_MISP_EXEC_ALL_BRANCHES = 0x89 | (0xff << 8),

        IDQ_UOPS_NOT_DELIVERED_CORE = 0x9c | (0x01 << 8),

        UOPS_DISPATCHED_PORT_PORT_0 = 0xa1 | (0x01 << 8),
        UOPS_DISPATCHED_PORT_PORT_1 = 0xa1 | (0x02 << 8),
        UOPS_DISPATCHED_PORT_PORT_2_LD = 0xa1 | (0x04 << 8),
        UOPS_DISPATCHED_PORT_PORT_2_STA = 0xa1 | (0x08 << 8),
        UOPS_DISPATCHED_PORT_PORT_2 = 0xa1 | (0x0c << 8),
        UOPS_DISPATCHED_PORT_PORT_3_LD = 0xa1 | (0x10 << 8),
        UOPS_DISPATCHED_PORT_PORT_3_STA = 0xa1 | (0x20 << 8),
        UOPS_DISPATCHED_PORT_PORT_3 = 0xa1 | (0x30 << 8),
        UOPS_DISPATCHED_PORT_PORT_4 = 0xa1 | (0x40 << 8),
        UOPS_DISPATCHED_PORT_PORT_5 = 0xa1 | (0x80 << 8),

        RESOURCE_STALLS_ANY = 0xa2 | (0x01 << 8),
        RESOURCE_STALLS_LB = 0xa2 | (0x02 << 8),
        RESOURCE_STALLS_RS = 0xa2 | (0x04 << 8),
        RESOURCE_STALLS_SB = 0xa2 | (0x08 << 8),
        RESOURCE_STALLS_ROB = 0xa2 | (0x10 << 8),
        RESOURCE_STALLS_FCSW = 0xa2 | (0x20 << 8),
        RESOURCE_STALLS_MXCSR = 0xa2 | (0x40 << 8),
        RESOURCE_STALLS_OTHER = 0xa2 | (0x80 << 8),

        DSB2MITE_SWITCHES_COUNT = 0xab | (0x01 << 8),
        DSB2MITE_SWITCHES_PENALTY_CYCLES = 0xab | (0x02 << 8),

        DSB_FILL_OTHER_CANCEL = 0xac | (0x02 << 8),
        DSB_FILL_EXCEED_DSB_LINES = 0xac | (0x04 << 8),
        DSB_FILL_ALL_CANCEL = 0xac | (0x08 << 8),

        ITLB_ITLB_FLUSH = 0xae | (0x01 << 8),

        OFFCORE_REQUESTS_DEMAND_DATA_RD = 0xb0 | (0x01 << 8),
        OFFCORE_REQUESTS_DEMAND_RFO = 0xb0 | (0x04 << 8),
        OFFCORE_REQUESTS_ALL_DATA_RD = 0xb0 | (0x08 << 8),

        UOPS_DISPATCHED_THREAD = 0xb1 | (0x01 << 8),
        UOPS_DISPATCHED_CORE = 0xb1 | (0x02 << 8),

        OFFCORE_REQUESTS_BUFFER_SQ_FULL = 0xb2 | (0x01 << 8),

        AGU_BYPASS_CANCEL_COUNT = 0xb6 | (0x01 << 8),

        OFF_CORE_RESPONSE_0 = 0xb7 | (0x01 << 8),

        OFF_CORE_RESPONSE_1 = 0xbb | (0x01 << 8),

        TLB_FLUSH_DTLB_THREAD = 0xbd | (0x01 << 8),
        TLB_FLUSH_STLB_ANY = 0xbd | (0x20 << 8),

        L1D_BLOCKS_BANK_CONFLICT_CYCLES = 0xbf | (0x05 << 8),

        INST_RETIRED_ANY_P = 0xc0 | (0x00 << 8), //table 19-1 architectural event
        INST_RETIRED_PREC_DIST = 0xc0 | (0x01 << 8), //PMC1 only; must quiesce other PMCs

        OTHER_ASSISTS_ITLB_MISS_RETIRED = 0xc1 | (0x02 << 8),
        OTHER_ASSISTS_AVX_STORE = 0xc1 | (0x08 << 8),
        OTHER_ASSISTS_AVX_TO_SSE = 0xc1 | (0x10 << 8),
        OTHER_ASSISTS_SSE_TO_AVX = 0xc1 | (0x20 << 8),

        UOPS_RETIRED_ALL = 0xc2 | (0x01 << 8),
        UOPS_RETIRED_RETIRE_SLOTS = 0xc2 | (0x02 << 8),

        MACHINE_CLEARS_MEMORY_ORDERING = 0xc3 | (0x02 << 8),
        MACHINE_CLEARS_SMC = 0xc3 | (0x04 << 8),
        MACHINE_CLEARS_MASKMOV = 0xc3 | (0x20 << 8),

        BR_INST_RETIRED_ALL_BRANCHES_ARCH = 0xc4 | (0x00 << 8), //table 19-1
        BR_INST_RETIRED_CONDITIONAL = 0xc4 | (0x01 << 8),
        BR_INST_RETIRED_NEAR_CALL = 0xc4 | (0x02 << 8),
        BR_INST_RETIRED_ALL_BRANCHES = 0xc4 | (0x04 << 8),
        BR_INST_RETIRED_NEAR_RETURN = 0xc4 | (0x08 << 8),
        BR_INST_RETIRED_NOT_TAKEN = 0xc4 | (0x10 << 8),
        BR_INST_RETIRED_NEAR_TAKEN = 0xc4 | (0x20 << 8),
        BR_INST_RETIRED_FAR_BRANCH = 0xc4 | (0x40 << 8),

        BR_MISP_RETIRED_ALL_BRANCHES_ARCH = 0xc5 | (0x00 << 8), //table 19-1
        BR_MISP_RETIRED_CONDITIONAL = 0xc5 | (0x01 << 8),
        BR_MISP_RETIRED_NEAR_CALL = 0xc5 | (0x02 << 8),
        BR_MISP_RETIRED_ALL_BRANCHES = 0xc5 | (0x04 << 8),
        BR_MISP_RETIRED_NOT_TAKEN = 0xc5 | (0x10 << 8),
        BR_MISP_RETIRED_TAKEN = 0xc5 | (0x20 << 8),

        FP_ASSIST_X87_OUTPUT = 0xca | (0x02 << 8),
        FP_ASSIST_X87_INPUT = 0xca | (0x04 << 8),
        FP_ASSIST_SIMD_OUTPUT = 0xca | (0x08 << 8),
        FP_ASSIST_SIMD_INPUT = 0xca | (0x10 << 8),
        FP_ASSIST_ANY = 0xca | (0x1e << 8),

        ROB_MISC_EVENTS_LBR_INSERTS = 0xcc | (0x20 << 8),

        MEM_TRANS_RETIRED_LOAD_LATENCY = 0xcd | (0x01 << 8), //specify threshold in MSR 0x3F6
        MEM_TRANS_RETIRED_PRECISE_STORE = 0xcd | (0x02 << 8), //see section 18.8.4.3

        MEM_UOP_RETIRED_LOADS = 0xd0 | (0x01 << 8),
        MEM_UOP_RETIRED_STORES = 0xd0 | (0x02 << 8),
        MEM_UOP_RETIRED_STLB_MISS = 0xd0 | (0x10 << 8),
        MEM_UOP_RETIRED_LOCK = 0xd0 | (0x20 << 8),
        MEM_UOP_RETIRED_SPLIT = 0xd0 | (0x40 << 8),
        MEM_UOP_RETIRED_ALL = 0xd0 | (0x80 << 8),

        MEM_UOPS_RETIRED_ALL_LOADS = 0xD0 | (0x81 << 8), // Supports PEBS. PMC0-3 only regardless HTT.

        MEM_LOAD_UOPS_RETIRED_L1_HIT = 0xd1 | (0x01 << 8),
        MEM_LOAD_UOPS_RETIRED_L2_HIT = 0xd1 | (0x02 << 8),
        MEM_LOAD_UOPS_RETIRED_L3_HIT = 0xd1 | (0x04 << 8),
        MEM_LOAD_UOPS_RETIRED_HIT_LFB = 0xd1 | (0x40 << 8),

        XSNP_MISS = 0xd2 | (0x01 << 8),
        XSNP_HIT  = 0xd2 | (0x02 << 8),
        XSNP_HITM = 0xd2 | (0x04 << 8),
        XSNP_NONE = 0xd2 | (0x08 << 8),

        MEM_LOAD_UOPS_MISC_RETIRED_LLC_MISS  = 0xd4 | (0x02 << 8),

        L2_TRANS_DEMAND_DATA_RD = 0xf0 | (0x01 << 8),
        L2_TRANS_RFO = 0xf0 | (0x02 << 8),
        L2_TRANS_CODE_RD = 0xf0 | (0x04 << 8),
        L2_TRANS_ALL_PF = 0xf0 | (0x08 << 8),
        L2_TRANS_L1D_WB = 0xf0 | (0x10 << 8),
        L2_TRANS_L2_FILL = 0xf0 | (0x20 << 8),
        L2_TRANS_L2_WB = 0xf0 | (0x40 << 8),
        L2_TRANS_ALL_REQ_UESTS = 0xf0 | (0x80 << 8),

        L2_LINES_IN_I = 0xf1 | (0x01 << 8),
        L2_LINES_IN_S = 0xf1 | (0x02 << 8),
        L2_LINES_IN_E = 0xf1 | (0x04 << 8),
        L2_LINES_IN_ALL = 0xf1 | (0x07 << 8),

        L2_LINES_OUT_DEMAND_CLEAN = 0xf2 | (0x01 << 8),
        L2_LINES_OUT_DEMAND_DIRTY = 0xf2 | (0x02 << 8),
        L2_LINES_OUT_DEMAND_PF_CLEAN = 0xf2 | (0x04 << 8),
        L2_LINES_OUT_DEMAND_PF_DIRTY = 0xf2 | (0x08 << 8),
        L2_LINES_OUT_DEMAND_DIRTY_ALL = 0xf2 | (0x0a << 8),

        SQ_MISC_SPLIT_LOCK = 0xF4 | (010 << 8)
    };

public:
    Intel_Sandy_Bridge_PMU() {}

    static bool config(Channel channel, Event event, Flags flags = NONE) {
        assert((channel < CHANNELS) && (event < EVENTS));
        db<PMU>(TRC) << "PMU::config(c=" << channel << ",e=" << event << ",f=" << flags << ")" << endl;

        if(((channel == 0) && (event != 0)) || ((channel == 1) && (event != 1)) || ((channel == 2) && (event != 2))) {
            db<PMU>(WRN) << "PMU::config: channel " << channel << " is fixed in this architecture and cannot be reconfigured!" << endl;
            return false;
        }

        if(channel >= FIXED) {
            wrmsr(EVTSEL0 + channel - FIXED, _events[event] | USR | OS | ENABLE | flags); // implicitly start counting due to flag ENABLE
        }

        start(channel);

        return true;
    }

    static Count read(Channel channel) {
        assert(channel < CHANNELS);
        db<PMU>(TRC) << "PMU::read(c=" << channel << ")" << endl;
        return channel < FIXED ? rdpmc(channel | (1 << 30)) : rdpmc(channel - FIXED);
    }

    static void write(Channel channel, Count count) {
        db<PMU>(TRC) << "PMU::write(ch=" << channel << ",ct=" << count << ")" << endl;
        if(channel >= FIXED) wrmsr(PMC_BASE_ADDR + channel - FIXED, count);
    }

    static void start(Channel channel) {
        assert(channel < CHANNELS);
        db<PMU>(TRC) << "PMU::start(c=" << channel << ")" << endl;
        if(channel < FIXED) {
            wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) | (3ULL << (CRT0_ENABLE_SYS + channel * 4))));
            wrmsr(GLOBAL_CTRL,   (rdmsr(GLOBAL_CTRL)   | (1ULL << (CRT0_ENABLE + channel))));
        } else {
            wrmsr(GLOBAL_OVF,  rdmsr(GLOBAL_OVF)  | (1ULL << (PMC0_OVERFLOW + channel - FIXED))); // clear OVF flag
            wrmsr(GLOBAL_CTRL, rdmsr(GLOBAL_CTRL) | (1ULL << (PMC0_ENABLE + channel - FIXED)));
        }
    }

    static void stop(Channel channel) {
        assert(channel < CHANNELS);
        db<PMU>(TRC) << "PMU::stop(c=" << channel << ")" << endl;
        if(channel < FIXED) {
            wrmsr(GLOBAL_OVF,   (rdmsr(GLOBAL_OVF)   | (1ULL << (CRT0_OVERFLOW + channel))));
            wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) & ~(3ULL << (CRT0_ENABLE_SYS + channel * 4))));
       } else
            wrmsr(GLOBAL_CTRL, rdmsr(GLOBAL_CTRL) & ~(1ULL << (PMC0_ENABLE + channel - FIXED)));
    }

    static void reset(Channel channel) {
        assert(channel < CHANNELS);
        db<PMU>(TRC) << "PMU::reset(c=" << channel << ")" << endl;
        if(channel < FIXED)
            wrmsr(FIXED_CTR0 + channel, 0);
        else
            wrmsr(EVTSEL0 + channel - FIXED, 0);
    }

    static bool overflow(Channel channel) {
        assert(channel < CHANNELS);
        db<PMU>(TRC) << "PMU::overflow(c=" << channel << ")" << endl;
        return (channel < FIXED) ? (rdmsr(GLOBAL_STATUS) & (1ULL << (CRT0_OVERFLOW + channel))) : (rdmsr(GLOBAL_STATUS) & (1ULL << (channel - FIXED)));
    }

    static bool overflow(void) {
        db<Intel_PMU_V1>(TRC) << "Intel_PMU_V1::overflow()\n";
        return ((rdmsr(GLOBAL_STATUS) & PMC_MASK) != 0);
    }

    static void clear_overflow(Channel channel) {
        assert(channel < CHANNELS);
        wrmsr(GLOBAL_OVF, (PMC_MASK & 1ULL << (channel - FIXED))); //clear OVF flag
    }

    static void handler(Handler * handler, Channel channel) {
        if((channel - FIXED) < CHANNELS)
            _handlers[channel - FIXED] = handler;
        else
            db<Init, Intel_PMU_V1>(WRN) << "Intel_PMU_V1::handler = Bad PMC value, handler not addressed!" << endl;
    }
private:
    static const Event _events[EVENTS];
};

template<int VERSION>
class PMU_Select_Version: public Intel_PMU_V1 {};
template<>
class PMU_Select_Version<Traits<PMU>::V2>: public Intel_PMU_V2 {};
template<>
class PMU_Select_Version<Traits<PMU>::V3>: public Intel_PMU_V3 {};
template<>
class PMU_Select_Version<Traits<PMU>::SANDY_BRIDGE>: public Intel_Sandy_Bridge_PMU {};


class PMU: public PMU_Select_Version<Traits<PMU>::VERSION>
{
    friend class CPU;
    typedef PMU_Select_Version<Traits<PMU>::VERSION> Engine;

public:
    using Engine::CHANNELS;
    using Engine::FIXED;
    using Engine::EVENTS;

public:
    PMU() {}

private:
    static void int_handler(Interrupt_Id i);

    static void init();
};

__END_SYS

#endif
