// EPOS ARM PrimeCell PL061 GPIO Mediator Declarations

#ifndef __pl061_h
#define __pl061_h

#include <architecture/cpu.h>
#define __ic_common_only__
#include <machine/ic.h>
#undef __ic_common_only__
#define __gpio_common_only__
#include <machine/gpio.h>
#undef __gpio_common_only__

__BEGIN_SYS

class PL061: public GPIO_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::TIMERx_BASE) GPTM".

private:
    typedef CPU::Reg32 Reg32;
    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    // Registers offsets from BASE (i.e. this)
    enum {                              // Description                          Type    Value after reset
        DATA            = 0x000,        // Data                                 rw      0x00000000
        DIR             = 0x400,        // Direction                            rw      0x00000000
        IS              = 0x404,        // Interrupt Sense                      rw      0x00000000
        IBE             = 0x408,        // Interrupt Both Edges                 rw      0x00000000
        IEV             = 0x40c,        // Interrupt Event                      rw      0x00000000
        IM              = 0x410,        // Interrupt Mask                       rw      0x00000000
        GRIS            = 0x414,        // Raw Interrupt Status                 ro      0x00000000
        MIS             = 0x418,        // Masked Interrupt Status              ro      0x00000000
        ICR             = 0x41c,        // Interrupt Clear                      w1c     0x00000000
        AFSEL           = 0x420,        // Alternate Function Select            rw      -
        DR2R            = 0x500,        // 2-mA Drive Select                    rw      0x000000ff
        DR4R            = 0x504,        // 4-mA Drive Select                    rw      0x00000000
        DR8R            = 0x508,        // 8-mA Drive Select                    rw      0x00000000
        ODR             = 0x50c,        // Open Drain Select                    rw      0x00000000
        PUR             = 0x510,        // Pull-Up Select                       rw      0x000000ff
        PDR             = 0x514,        // Pull-Down Select                     rw      0x00000000
        SLR             = 0x518,        // Slew Rate Control Select             rw      0x00000000
        DEN             = 0x51c,        // Digital Enable                       rw      0x000000ff
        P_EDGE_CTRL     = 0x704,        // Power-up Interrupt Edge Control      rw      0x00000000
        PI_IEN          = 0x710,        // Power-up Interrupt Enable            rw      0x00000000
        IRQ_DETECT_ACK  = 0x718,        // Power-up Interrupt Status/Clear      rw      0x00000000
        PeriphID4       = 0xfd0,        // Peripheral Identification 4          ro      0x00000000
        PeriphID5       = 0xfd4,        // Peripheral Identification 5          ro      0x00000000
        PeriphID6       = 0xfd8,        // Peripheral Identification 6          ro      0x00000000
        PeriphID7       = 0xfdc,        // Peripheral Identification 7          ro      0x00000000
        PeriphID0       = 0xfe0,        // Peripheral Identification 0          ro      0x00000061
        PeriphID1       = 0xfe4,        // Peripheral Identification 1          ro      0x00000000
        PeriphID2       = 0xfe8,        // Peripheral Identification 2          ro      0x00000018
        PeriphID3       = 0xfec,        // Peripheral Identification 3          ro      0x00000001
        PCellID0        = 0xff0,        // PrimeCell Identification 0           ro      0x0000000d
        PCellID1        = 0xff4,        // PrimeCell Identification 1           ro      0x000000f0
        PCellID2        = 0xff8,        // PrimeCell Identification 2           ro      0x00000005
        PCellID3        = 0xffc         // PrimeCell Identification 3           ro      0x000000b1
    };

    // MUX functions
    enum Function {
        FUN_GPIO,
        FUN_ALTERNATE
    };

    // Pin codes
    enum {
        PIN0 = 1 <<  0,
        PIN1 = 1 <<  1,
        PIN2 = 1 <<  2,
        PIN3 = 1 <<  3,
        PIN4 = 1 <<  4,
        PIN5 = 1 <<  5,
        PIN6 = 1 <<  6,
        PIN7 = 1 <<  7
    };

    // Useful Bits in the Alternate Function Select Register
    enum {                              // Description                  Type    Value after reset
        AFSEL_ALTP0     = 1 <<  0,      // Pin 0 (0 -> GPIO | 1 -> Alt) rw      0
        AFSEL_ALTP1     = 1 <<  1,      // Pin 1 (0 -> GPIO | 1 -> Alt) rw      0
        AFSEL_ALTP2     = 1 <<  2,      // Pin 2 (0 -> GPIO | 1 -> Alt) rw      0
        AFSEL_ALTP3     = 1 <<  3,      // Pin 3 (0 -> GPIO | 1 -> Alt) rw      0
        AFSEL_ALTP4     = 1 <<  4,      // Pin 4 (0 -> GPIO | 1 -> Alt) rw      0
        AFSEL_ALTP5     = 1 <<  5,      // Pin 5 (0 -> GPIO | 1 -> Alt) rw      0
        AFSEL_ALTP6     = 1 <<  6,      // Pin 6 (0 -> GPIO | 1 -> Alt) rw      0
        AFSEL_ALTP7     = 1 <<  7       // Pin 7 (0 -> GPIO | 1 -> Alt) rw      0
    };

    // Useful Bits in the Digital Enable Register
    enum {                              // Description                    Type    Value after reset
        DEN_DIGP0       = 1 <<  0,      // Pin 0 (1 -> Digital Enable)    rw      1
        DEN_DIGP1       = 1 <<  1,      // Pin 1 (1 -> Digital Enable)    rw      1
        DEN_DIGP2       = 1 <<  2,      // Pin 2 (1 -> Digital Enable)    rw      1
        DEN_DIGP3       = 1 <<  3,      // Pin 3 (1 -> Digital Enable)    rw      1
        DEN_DIGP4       = 1 <<  4,      // Pin 4 (1 -> Digital Enable)    rw      1
        DEN_DIGP5       = 1 <<  5,      // Pin 5 (1 -> Digital Enable)    rw      1
        DEN_DIGP6       = 1 <<  6,      // Pin 6 (1 -> Digital Enable)    rw      1
        DEN_DIGP7       = 1 <<  7       // Pin 7 (1 -> Digital Enable)    rw      1
    };

public:
    unsigned char get() { return gpio(DATA + (0xff << 2)); }
    bool get(Pin mask) { return gpio(DATA + (mask << 2)); }
    void set() { gpio(DATA) = 0xff; }
    void set(Pin mask, bool value = true) { gpio(DATA + (mask << 2)) = value * 0xff; }
    void clear() { gpio(DATA) = 0; }
    void clear(Pin mask) { set(mask, false); }

    void direction(Pin mask, Direction dir) {
        switch(dir) {
            case OUT:
                gpio(DIR) |= mask;
                break;
            case IN:
            case INOUT:
                gpio(DIR) &= ~mask;
                break;
        }
    }

    void int_enable(Pin mask) { gpio(IM) |= mask; }
    void int_disable(Pin mask) { gpio(IM) &= ~mask; }
    void int_enable(Port port, Pin mask, Level level, bool power_up = false, Level power_up_level = HIGH);
    void int_enable(Port port, Pin mask, Edge edge, bool power_up = false, Edge power_up_edge = RISING) {
        gpio(IS) &= ~mask; // Set interrupt to edge-triggered

        switch(edge) {
        case RISING:
            gpio(IBE) &= ~mask; // Interrupt on single edge, defined by IEV
            gpio(IEV) |= mask;
            break;
        case FALLING:
            gpio(IBE) &= ~mask; // Interrupt on single edge, defined by IEV
            gpio(IEV) &= ~mask;
            break;
        case BOTH:
            gpio(IBE) |= mask;
            break;
        case NONE:
            break;
        }

        if(Traits<GPIO>::supports_power_up && power_up) {
            assert(power_up_edge != BOTH);
            if(power_up_edge == FALLING)
                gpio(P_EDGE_CTRL) |= (mask << (8 * port)); // this is a bit weird: each port has an P_EDGE_CTRL, but the bits for each port are different
            else if (power_up_edge == RISING)
                gpio(P_EDGE_CTRL) &= ~(mask << (8 * port));
            gpio(PI_IEN) |= (mask << (8 * port));
        }
    }

    void select_pin_function(Pin mask, Function fun) {
        if(fun == FUN_ALTERNATE) {
            gpio(AFSEL) |= mask;
        } else
            gpio(AFSEL) &= ~mask;
    }

    void pull(Pin mask, Pull p) {
        switch(p) {
            case UP:
                gpio(PUR) &= mask;
                break;
            case DOWN:
                gpio(PDR) &= mask;
                break;
            case FLOATING:
                gpio(ODR) &= mask;
                break;
        }
    }

    void clear_interrupts(Port port, Pin mask) {
        gpio(ICR) = mask;

        // There is something weird going on here.
        // The manual says: "There is a self-clearing function to this register that generates a
        // reset pulse to clear any interrupt which has its corresponding bit set to 1."
        // But this is not happening!
        // Also, clearing only the bit that is set or replacing the statement below with
        // regs[irq_number](IRQ_DETECT_ACK) = 0;
        // does not work!
        gpio(IRQ_DETECT_ACK) &=  ~(mask << (8 * port));
    }

    unsigned int pending_regular_interrupts() { return gpio(MIS); }
    unsigned int pending_powerup_interrupts() { return gpio(IRQ_DETECT_ACK); }

private:
    volatile Reg32 & gpio(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
