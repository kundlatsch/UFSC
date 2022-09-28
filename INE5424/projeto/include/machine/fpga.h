// EPOS FPGA Mediator Common Package

#ifndef __fpga_h
#define __fpga_h

#include <system/config.h>

__BEGIN_SYS

class FPGA_Common
{
protected:
    FPGA_Common() {}

public:
    static void reset();
    static void int_handler(IC::Interrupt_Id interrupt);
};

__END_SYS

#endif

#if defined(__FPGA_H) && !defined(__fpga_common_only__)
#include __FPGA_H
#endif
