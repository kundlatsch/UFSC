// EPOS IC Mediator Common Package

#ifndef __ic_h
#define __ic_h

#include <system/config.h>

__BEGIN_SYS

class IC_Common
{
public:
    typedef unsigned int Interrupt_Id;
    typedef void (* Interrupt_Handler)(Interrupt_Id);

    static const unsigned int UNSUPPORTED_INTERRUTP = ~1;

    enum {
        INT_SYS_TIMER   = UNSUPPORTED_INTERRUTP,
        INT_USER_TIMER0 = UNSUPPORTED_INTERRUTP,
        INT_USER_TIMER1 = UNSUPPORTED_INTERRUTP,
        INT_USER_TIMER2 = UNSUPPORTED_INTERRUTP,
        INT_USER_TIMER3 = UNSUPPORTED_INTERRUTP,
        INT_USER_TIMER4 = UNSUPPORTED_INTERRUTP,
        INT_USER_TIMER5 = UNSUPPORTED_INTERRUTP,
        INT_USER_TIMER6 = UNSUPPORTED_INTERRUTP,
        INT_USER_TIMER7 = UNSUPPORTED_INTERRUTP,
        INT_UART0       = UNSUPPORTED_INTERRUTP,
        INT_UART1       = UNSUPPORTED_INTERRUTP,
        INT_UART2       = UNSUPPORTED_INTERRUTP,
        INT_UART3       = UNSUPPORTED_INTERRUTP,
        INT_NIC0        = UNSUPPORTED_INTERRUTP,
        INT_NIC0_RX     = UNSUPPORTED_INTERRUTP,
        INT_NIC0_TX     = UNSUPPORTED_INTERRUTP,
        INT_NIC0_ERR    = UNSUPPORTED_INTERRUTP,
        INT_NIC0_TIMER  = UNSUPPORTED_INTERRUTP,
        INT_USB0        = UNSUPPORTED_INTERRUTP,
        INT_SPI0        = UNSUPPORTED_INTERRUTP,
        INT_I2C0        = UNSUPPORTED_INTERRUTP,
        INT_GPIOA       = UNSUPPORTED_INTERRUTP,
        INT_GPIOB       = UNSUPPORTED_INTERRUTP,
        INT_GPIOC       = UNSUPPORTED_INTERRUTP,
        INT_GPIOD       = UNSUPPORTED_INTERRUTP,
        INT_GPIOE       = UNSUPPORTED_INTERRUTP,
        INT_GPIOF       = UNSUPPORTED_INTERRUTP,
        INT_ADC0        = UNSUPPORTED_INTERRUTP,
        INT_RESCHEDULER = UNSUPPORTED_INTERRUTP,
        LAST_INT        = UNSUPPORTED_INTERRUTP
    };

protected:
    IC_Common() {}

public:
    static Interrupt_Handler int_vector(Interrupt_Id id);
    static void int_vector(Interrupt_Id id, const Interrupt_Handler & handler);

    static void enable();
    static void enable(Interrupt_Id id);
    static void disable();
    static void disable(Interrupt_Id id);

    static Interrupt_Id irq2int(Interrupt_Id id);       // Offset IRQs as seen by the bus to INTs seen by the CPU (if needed)
    static Interrupt_Id int2irq(Interrupt_Id irq);      // Offset INTs as seen by the CPU to IRQs seen by the bus (if needed)

    static void ipi(unsigned int cpu, Interrupt_Id id); // Inter-processor Interrupt
};

__END_SYS

#endif

#if defined(__IC_H) && !defined(__ic_common_only__)
#include __IC_H
#endif
