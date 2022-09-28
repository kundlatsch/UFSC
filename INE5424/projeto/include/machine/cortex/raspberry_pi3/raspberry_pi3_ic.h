// EPOS Paspberry Pi3 (ARM Cortex-A53) IC Mediator Declarations

#ifndef __raspberry_pi3_ic_h
#define __raspberry_pi3_ic_h

#include <machine/cortex/engine/cortex_a53/bcm_mailbox.h>
#include <system/memory_map.h>

__BEGIN_SYS

class IC_Engine: public BCM_IC_Common
{
public:
    // Interrupts
    static const unsigned int INTS = Traits<IC>::INTS;
    enum {
        INT_SYS_TIMER           = Traits<Machine>::emulated ? CORE0_MAILBOX_TIMER_IRQ : SYSTEM_TIMER_MATCH1,
        INT_USER_TIMER0         = SYSTEM_TIMER_MATCH3,
        INT_USER_TIMER1         = SYSTEM_TIMER_MATCH3,
        INT_USER_TIMER2         = SYSTEM_TIMER_MATCH3,
        INT_USER_TIMER3         = SYSTEM_TIMER_MATCH3,
        INT_GPIOA               = GPIO_INT0,
        INT_GPIOB               = GPIO_INT1,
        INT_GPIOC               = GPIO_INT2,
        INT_GPIOD               = GPIO_INT3,
        INT_UART0               = UART_AUX_INT,
        INT_UART1               = UART_INT0,
        INT_USB0                = USB_CONTROLLER,
        INT_SPI                 = SPI_INT,
        INT_RESCHEDULER         = CORE0_MAILBOX0_IRQ,
        LAST_INT                = BCM_IC_Common::LAST_INT
    };

public:
    static void enable() {
        mbox()->enable();
    }

    static void enable(unsigned int i) {
        assert(i <= INTS);
        switch(i / 32){
        case 0 /* */:
        case 1 /* */:
        case 2 /* IRQ */:       irq()->enable(i);         break;
        default /* Mailbox */:  mbox()->enable(i);        break;
        }
    }

    static void disable() {
        //mbox()->disable();
    }

    static void disable(unsigned int i) {
        assert(i <= INTS);
        switch(i / 32){
        case 0 /* */:
        case 1 /* */:
        case 2 /* IRQ */:       irq()->disable(i);        break;
        default /* Mailbox */:  mbox()->disable(i);       break;
        }
    }

    static Interrupt_Id int_id() {
        Interrupt_Id id = mbox()->int_id(); // check mailbox first
        if(id == LAST_INT) // if it wasn't the mailbox that triggered the interrupt, then check irq
            id = irq()->int_id();
        return id;
    }

    static Interrupt_Id irq2int(Interrupt_Id id) { return id; }
    static Interrupt_Id int2irq(Interrupt_Id irq) { return irq; }

    static void ipi(unsigned int cpu, Interrupt_Id id) { mbox()->ipi(cpu, id); }

    static void mailbox_eoi(Interrupt_Id id) {mbox()->eoi(id); }

    static void init() { mbox()->init(); }; // irq doesn't need initialization

private:
    static BCM_IRQ * irq() { return reinterpret_cast<BCM_IRQ *>(Memory_Map::IC_BASE); }
    static BCM_Mailbox * mbox() { return reinterpret_cast<BCM_Mailbox *>(Memory_Map::MBOX_CTRL_BASE); }
};

__END_SYS

#endif
