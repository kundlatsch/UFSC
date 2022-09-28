// EPOS RISC-V 32 Architecture Metainfo
#ifndef __rv32_traits_h
#define __rv32_traits_h

#include <system/config.h>

__BEGIN_SYS

template<> struct Traits<CPU>: public Traits<Build>
{
    enum {LITTLE, BIG};
    static const unsigned int ENDIANESS         = LITTLE;
    static const unsigned int WORD_SIZE         = 32;
    static const unsigned int CLOCK             = 50000000;
    static const bool unaligned_memory_access   = false;
};

template<> struct Traits<MMU>: public Traits<Build>
{
    static const bool colorful = false;
    static const unsigned int COLORS = 1;
};

template<> struct Traits<FPU>: public Traits<Build>
{
    static const bool enabled = false;
    static const bool user_save = true;
};

template<> struct Traits<TSC>: public Traits<Build>
{
    static const bool enabled = true;
};

__END_SYS

#endif
