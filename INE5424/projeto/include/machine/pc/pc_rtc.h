// EPOS PC Real-Time Clock Mediator Declarations

#ifndef __pc_rtc_h
#define __pc_rtc_h

#include <architecture/cpu.h>
#include <machine/rtc.h>

__BEGIN_SYS

// Motorola MC146818 RTC (features an alarm and a batery-backup RAM (CMOS) too)
class MC146818
{
private:
    typedef CPU::IO_Port IO_Port;
    typedef CPU::Reg8 Reg8;

    static const unsigned int CMOS_SIZE = 114;

public:
    // I/O ports
    enum {
        ADDR		= 0x70,
        DATA		= 0x71
    };

    // Register Addresses
    typedef Reg8 Address;
    enum {
        SECONDS		    = 0x00,
        ALARM_S		    = 0x01,
        MINUTES		    = 0x02,
        ALARM_M		    = 0x03,
        HOURS		    = 0x04,
        ALARM_H		    = 0x05,
        WEEK_DAY	    = 0x06, // 1 = Sunday
        DAY		        = 0x07,
        MONTH		    = 0x08,
        YEAR		    = 0x09,
        REG_A    	    = 0x0a, // Control/Status Register A
        REG_B		    = 0x0b, // Control/Status Register B
        REG_C		    = 0x0c, // Control/Status Register C
        REG_D		    = 0x0d, // Control/Status Register D
        CMOS     	    = 0x0e,
        // Layout used by BIOS
        CMOS_DIAGNOSTIC	= CMOS + 0, // POST diagnostic
        CMOS_POWER_DONW	= CMOS + 1, // Status of system power-down
        CMOS_FLOPPY	    = CMOS + 2,
        CMOS_DISK1	    = CMOS + 3,
        CMOS_DISK2	    = CMOS + 4,
                       // CMOS + 5  is reserved
        CMOS_CONFIG	    = CMOS + 6,
        CMOS_MEM_LSB	= CMOS + 7,
        CMOS_MEM_MSB	= CMOS + 8,
        CMOS_EX_MEM_LSB	= CMOS + 9,
        CMOS_EX_MEM_MSB	= CMOS + 10,
        	           // CMOS + 11  until
        	           // CMOS + 12  is reserved
        CMOS_CHK_LSB	= CMOS + 13,
        CMOS_CHK_MSB	= CMOS + 14,
        CMOS_OT_MEM_LSB	= CMOS + 15,
        CMOS_OT_MEM_HSB	= CMOS + 16,
        CENTURY		    = CMOS + 17
        	           // CMOS + 18  until
        	           // CMOS + 49  is reserved
    };

    // Control/Status register A
    enum {
        INT_FREQ_MASK	= 0x0f, // Interrupt frequency
        TIME_FREQ_MASK	= 0x70, // Time frequency ?
        UPDATED		    = 0x80, // Enable/disable time updates
        DEF_REG_A	    = 0x26  // BIOS defaults (INT=1024Hz, TIME=32,768KHz)
    };

    // Control/Status register B
    enum {
        DST		    = 0x01, // Daylight savings time
        HOUR_24		= 0x02, // Hour format 24/12
        BINARY		= 0x04, // Binary or BCD
        INT_MASK	= 0x70, // Interrupt masks
        INT_SECOND	= 0x10, // Every time update (second)
        INT_ALARM	= 0x20, // When alarm time is reached
  	    INT_FREQ	= 0x40, // Periodic (freq in CRTL A)
        ENABLE		= 0x80, // Enable/disable time updates
        DEF_REG_B	= (ENABLE | BINARY | HOUR_24)
    };

    // Status register C
    enum {
        INT_STAT_MASK	= 0x70, // What caused an interrupt?
        INT_STAT_SECOND	= 0x10, // Time update (second)
        INT_STAT_ALARM	= 0x20, // Alarm
      	INT_STAT_FREQ	= 0x40  // Periodic
    };

    // Status register D
    enum {
    	BATERY = 0x80 // 0 -> low
    };

public:
    MC146818() {}

    static unsigned char reg(Address a) {
        CPU::out8(ADDR, a);
        return bcd2bin(CPU::in8(DATA));
    }
    static void reg(Address a, Reg8 v) {
        CPU::out8(ADDR, a);
        CPU::out8(DATA, bin2bcd(v));
    }

    static unsigned char cmos_read(Address a) {
        CPU::out8(ADDR, CMOS + a);
        return CPU::in8(DATA);
    }
    static void cmos_write(Address a, Reg8 v) {
        CPU::out8(ADDR, CMOS + a);
        CPU::out8(DATA, v);
    }

    static unsigned int cmos_size() { return CMOS_SIZE; }

private:
    static int bcd2bin(int bcd) { return (bcd >> 4) * 10 + (bcd & 0x0f); }
    static int bin2bcd(int bin) { return ((bin / 10) << 4) | (bin % 10); }
};

class RTC: public RTC_Common, private MC146818
{
private:
    static const unsigned int EPOCH_YEAR = Traits<RTC>::EPOCH_YEAR;
    static const unsigned int EPOCH_DAYS = Traits<RTC>::EPOCH_DAYS;

public:
    RTC() {}

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { return date().to_offset(EPOCH_DAYS); }
};

__END_SYS

#endif
