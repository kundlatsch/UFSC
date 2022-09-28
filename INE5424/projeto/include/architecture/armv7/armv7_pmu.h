// EPOS ARMv7 PMU Mediator Declarations

#ifndef __armv7_pmu_h
#define __armv7_pmu_h

#include <architecture/cpu.h>
#define __common_only__
#include <architecture/pmu.h>
#undef __common_only__

__BEGIN_SYS

class ARMv7_A_PMU: public PMU_Common
{
private:
    typedef CPU::Reg32 Reg32;

protected:
    static const unsigned int CHANNELS = 6;
    static const unsigned int FIXED = 0;
    static const unsigned int EVENTS = 93; // 54 if A53

public:
    // Useful bits in the PMCR register
    enum {                      // Description                          Type    Value after reset
        PMCR_E = 1 << 0,        // Enable all counters                  r/w
        PMCR_P = 1 << 1,        // Reset event counters                 r/w
        PMCR_C = 1 << 2,        // Cycle counter reset                  r/w
        PMCR_D = 1 << 3,        // Enable cycle counter prescale (1/64) r/w
        PMCR_X = 1 << 4,        // Export events                        r/w
    };

    // Useful bits in the PMCNTENSET register
    enum {                      // Description                          Type    Value after reset
        PMCNTENSET_C = 1 << 31, // Cycle counter enable                 r/w
    };

    // Useful bits in the PMOVSR register
    enum {                      // Description                          Type    Value after reset
        PMOVSR_C = 1 << 31,     // Cycle counter overflow clear         r/w
    };

    // Predefined architectural performance events
    enum {
        // Event
        L1I_REFILL                            = 0x01,
        L1I_TLB_REFILL                        = 0x02,
        L1D_REFILL                            = 0x03,
        L1D_ACCESS                            = 0x04,
        L1D_TLB_REFILL                        = 0x05,
        INSTRUCTIONS_ARCHITECTURALLY_EXECUTED = 0x08,
        EXCEPTION_TAKEN                       = 0x09,
        BRANCHES_ARCHITECTURALLY_EXECUTED     = 0x0c,
        IMMEDIATE_BRANCH                      = 0X0d,
        UNALIGNED_LOAD_STORE                  = 0X0f,
        MISPREDICTED_BRANCH                   = 0x10,
        CYCLE                                 = 0x11,
        PREDICTABLE_BRANCH_EXECUTED           = 0x12,
        DATA_MEMORY_ACCESS                    = 0x13,
        L1I_ACCESS                            = 0x14,
        L1D_WRITEBACK                         = 0x15,
        L2D_ACCESS                            = 0x16,
        L2D_REFILL                            = 0x17,
        L2D_WRITEBACK                         = 0x18,
        BUS_ACCESS                            = 0x19,
        LOCAL_MEMORY_ERROR                    = 0x1a,
        INSTRUCTION_SPECULATIVELY_EXECUTED    = 0x1b,
        BUS_CYCLE                             = 0x1d,
        CHAIN                                 = 0x1e,
        // Cortex-A9 specific events
        JAVA_BYTECODE_EXECUTE                 = 0x40,
        SOFTWARE_JAVA_BYTECODE_EXECUTED       = 0x41,
        JAZELLE_BACKWARDS_BRANCHES_EXECUTED   = 0x42,
        COHERENT_LINEFILL_MISS                = 0x50,
        COHERENT_LINEFILL_HIT                 = 0x51,
        ICACHE_DEPENDENT_STALL_CYCLES         = 0x60,
        DCACHE_DEPENDENT_STALL_CYCLES         = 0x61,
        MAIN_TLB_MISS_STALL_CYCLES            = 0x62,
        STREX_PASSED                          = 0x63,
        STREX_FAILED                          = 0x64,
        DATA_EVICTION                         = 0x65,
        ISSUE_DOESNT_DISPATCH                 = 0x66,
        ISSUE_EMPTY                           = 0x67,
        ISSUE_CORE_RENAMING                   = 0x68,
        PREDICTABLE_FUNCTION_RETURNS          = 0x6e,
        MAIN_EXECUTION_UNIT_RETURNS           = 0x70,
        SECOND_EXECUTION_UNIT_RETURNS         = 0x71,
        LOAD_STORE_INSTRUCTIONS               = 0x72,
        FLOATING_POINT_INSTRUCTIONS           = 0x73,
        NEON_INSTRUCTIONS                     = 0x74,
        PROCESSOR_STALL_PLD                   = 0x80,
        PROCESSOR_STALL_WRITE_MEMORY          = 0x81,
        PROCESSOR_STALL_ITLB_MISS             = 0x82,
        PROCESSOR_STALL_DTLB_MISS             = 0x83,
        PROCESSOR_STALL_IUTLB_MISS            = 0x84,
        PROCESSOR_STALL_DUTLB_MISS            = 0x85,
        PROCESSOR_STALL_DMB                   = 0x86,
        INTEGER_CLOCK_ENABLED                 = 0x8a,
        DATA_ENGINE_CLOCK_ENABLED             = 0x8b,
        ISB_INSTRUCTIONS                      = 0x90,
        DSB_INSTRUCTIONS                      = 0x91,
        DMB_INSTRUCTIONS                      = 0x92,
        EXTERNAL_INTERRUPTS                   = 0x93,
        PLE_CACHE_LINE_REQUEST_COMPLETED      = 0xa0,
        PLE_CACHE_LINE_REQUEST_SKIPPED        = 0xa1,
        PLE_FIFO_FLUSH                        = 0xa2,
        PLE_REQUEST_COMPLETED                 = 0xa3,
        PLE_FIFO_OVERFLOW                     = 0xa4,
        PLE_REQUEST_PROGRAMMED                = 0xa5,
        // Cortex A-53 specific events
        BUS_ACCESS_LD                         = 0x60,
        BUS_ACCESS_ST                         = 0x61,
        BR_INDIRECT_SPEC                      = 0x7a,
        EXC_IRQ                               = 0x86,
        EXC_FIQ                               = 0x87,
        EXTERNAL_MEM_REQUEST                  = 0xc0,
        EXTERNAL_MEM_REQUEST_NON_CACHEABLE    = 0xc1,
        PREFETCH_LINEFILL                     = 0xc2,
        ICACHE_THROTTLE                       = 0xc3,
        ENTER_READ_ALLOC_MODE                 = 0xc4,
        READ_ALLOC_MODE                       = 0xc5,
        PRE_DECODE_ERROR                      = 0xc6,
        DATA_WRITE_STALL_ST_BUFFER_FULL       = 0xc7,
        SCU_SNOOPED_DATA_FROM_OTHER_CPU       = 0xc8,
        CONDITIONAL_BRANCH_EXECUTED           = 0xc9,
        IND_BR_MISP                           = 0xca,
        IND_BR_MISP_ADDRESS_MISCOMPARE        = 0xcb,
        CONDITIONAL_BRANCH_MISP               = 0xcc,
        L1_ICACHE_MEM_ERROR                   = 0xd0,
        L1_DCACHE_MEM_ERROR                   = 0xd1,
        TLB_MEM_ERROR                         = 0xd2,
        EMPTY_DPU_IQ_NOT_GUILTY               = 0xe0,
        EMPTY_DPU_IQ_ICACHE_MISS              = 0xe1,
        EMPTY_DPU_IQ_IMICRO_TLB_MISS          = 0xe2,
        EMPTY_DPU_IQ_PRE_DECODE_ERROR         = 0xe3,
        INTERLOCK_CYCLE_NOT_GUILTY            = 0xe4,
        INTERLOCK_CYCLE_LD_ST_WAIT_AGU_ADDRESS= 0xe5,
        INTERLOCK_CYCLE_ADV_SIMD_FP_INST      = 0xe6,
        INTERLOCK_CYCLE_WR_STAGE_STALL_BC_MISS= 0xe7,
        INTERLOCK_CYCLE_WR_STAGE_STALL_BC_STR = 0xe8,
    };

public:
    ARMv7_A_PMU() {}

    static void config(Channel channel, const Event event, Flags flags = NONE) {
        assert((static_cast<unsigned int>(channel) < CHANNELS) && (static_cast<unsigned int>(event) < EVENTS));
        db<PMU>(TRC) << "PMU::config(c=" << channel << ",e=" << event << ",f=" << flags << ")" << endl;
        pmselr(channel);
        pmxevtyper(_events[event]);
        start(channel);
    }

    static Count read(Channel channel) {
        db<PMU>(TRC) << "PMU::read(c=" << channel << ")" << endl;
        pmselr(channel);
        return pmxevcntr();
    }

    static void write(Channel channel, Count count) {
        db<PMU>(TRC) << "PMU::write(ch=" << channel << ",ct=" << count << ")" << endl;
        pmselr(channel);
        pmxevcntr(count);
    }

    static void start(Channel channel) {
        db<PMU>(TRC) << "PMU::start(c=" << channel << ")" << endl;
        pmcntenset(pmcntenset() | (1 << channel));
    }

    static void stop(Channel channel) {
        db<PMU>(TRC) << "PMU::stop(c=" << channel << ")" << endl;
        pmcntenclr(pmcntenclr() | (1 << channel));
    }

    static void reset(Channel channel) {
        db<PMU>(TRC) << "PMU::reset(c=" << channel << ")" << endl;
        write(channel, 0);
    }

    static void init();

private:
    static void pmcr(Reg32 reg) { ASM("mcr p15, 0, %0, c9, c12, 0\n\t" : : "r"(reg)); }
    static Reg32 pmcr() { Reg32 reg; ASM("mrc p15, 0, %0, c9, c12, 0\n\t" : "=r"(reg) : ); return reg; }

    static void pmcntenset(Reg32 reg) { ASM("mcr p15, 0, %0, c9, c12, 1\n\t" : : "r"(reg)); }
    static Reg32 pmcntenset() { Reg32 reg; ASM("mrc p15, 0, %0, c9, c12, 1\n\t" : "=r"(reg) : ); return reg; }

    static void pmcntenclr(Reg32 reg) { ASM("mcr p15, 0, %0, c9, c12, 2\n\t" : : "r"(reg)); }
    static Reg32 pmcntenclr() { Reg32 reg; ASM("mrc p15, 0, %0, c9, c12, 2\n\t" : "=r"(reg) : ); return reg; }

    static void pmovsr(Reg32 reg) { ASM("mcr p15, 0, %0, c9, c12, 3\n\t" : : "r"(reg)); }
    static Reg32 pmovsr() { Reg32 reg; ASM("mrc p15, 0, %0, c9, c12, 3\n\t" : "=r"(reg) : ); return reg; }

    static void pmselr(Reg32 reg) { ASM("mcr p15, 0, %0, c9, c12, 5\n\t" : : "r"(reg)); }
    static Reg32 pmselr() { Reg32 reg; ASM("mrc p15, 0, %0, c9, c12, 5\n\t" : "=r"(reg) : ); return reg; }

    static void pmxevtyper(const Reg32 reg) { ASM("mcr p15, 0, %0, c9, c13, 1\n\t" : : "r"(reg)); }
    static Reg32 pmxevtyper() { Reg32 reg; ASM("mrc p15, 0, %0, c9, c13, 1\n\t" : "=r"(reg) : ); return reg; }

    static void pmxevcntr(Reg32 reg) { ASM("mcr p15, 0, %0, c9, c13, 2\n\t" : : "r"(reg)); }
    static Reg32 pmxevcntr() { Reg32 reg; ASM("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r"(reg) : ); return reg; }

private:
    static const Event _events[EVENTS];
};


class PMU: private ARMv7_A_PMU
{
    friend class CPU;

private:
    typedef ARMv7_A_PMU Engine;

public:
    using Engine::CHANNELS;
    using Engine::FIXED;
    using Engine::EVENTS;

    using Engine::Event;
    using Engine::Count;
    using Engine::Channel;

public:
    PMU() {}

    using Engine::config;
    using Engine::read;
    using Engine::write;
    using Engine::start;
    using Engine::stop;
    using Engine::reset;

private:
    static void init() { Engine::init(); }
};

__END_SYS

#endif
