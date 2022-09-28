// EPOS EPOSMoteIII (ARM Cortex-M3) I2C Mediator Declarations

// TODO: it looks like this mediator only implements Master operation

#ifndef __emote3_i2c_h
#define __emote3_i2c_h

#include <architecture/cpu.h>
#include <machine/cortex/engine/pl061.h>
#define __i2c_common_only__
#include <machine/i2c.h>
#undef __i2c_common_only__
#include "emote3_sysctrl.h"
#include "emote3_ioctrl.h"
#include <system/memory_map.h>

__BEGIN_SYS

class CC2538_I2C: public I2C_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::ADC0_BASE) eMote3_ADC"

private:
    typedef CPU::Reg32 Reg32;

public:
    // Registers offsets from BASE (i.e. this)
    enum {
        I2CM_SA         = 0x00,
        I2CM_CTRL       = 0x04,
        I2CM_STAT       = I2CM_CTRL,
        I2CM_DR         = 0x08,
        I2CM_TPR        = 0x0c,
        I2CM_IMR        = 0x10,
        I2CM_RIS        = 0x14,
        I2CM_MIS        = 0x18,
        I2CM_ICR        = 0x1c,
        I2CM_CR         = 0x20,
    };

    // I2C slave offsets
    enum {                           // Description
        I2CS_OAR        = 0x00,      // Own Address
        I2CS_CTRL       = 0x04,      // Control and Status
        I2CS_STAT       = I2CS_CTRL, // Control and Status
        I2CS_DR         = 0x08,      // Data
        I2CS_IMR        = 0x0c,      // Interrupt Mask
        I2CS_RIS        = 0x10,      // Raw Interrupt Status
        I2CS_MIS        = 0x14,      // Masked Interrupt Status
        I2CS_ICR        = 0x18,      // Interrupt Clear
    };

    // Unified I2C Master-Slave offsets
    enum {
        I2C_SA         = 0x00,
        I2C_OAR        = I2C_SA,
        I2C_CTRL       = 0x04,
        I2C_STAT       = I2C_CTRL,
        I2C_DR         = 0x08,
        I2C_TPR        = 0x0c,
        I2C_IMR        = 0x10,
        I2C_RIS        = 0x14,
        I2C_MIS        = 0x18,
        I2C_ICR        = 0x1c,
        I2C_CR         = 0x20,
    };

    // Useful bits in the I2CM_SA register
    enum {
        I2C_SA_RS              = 0x01,
    };

    // Useful bits in the I2CM_CTRL register
    enum {
        I2C_CTRL_ACK   = 1 << 3,
        I2C_CTRL_STOP  = 1 << 2,
        I2C_CTRL_START = 1 << 1,
        I2C_CTRL_RUN   = 1 << 0,
    };

    // Useful bits in the I2CM_STAT register
    enum {                        // Description (type)
        I2C_STAT_BUSBSY = 1 << 6, // Bus Busy (RO)
        I2C_STAT_IDLE   = 1 << 5, // I2C Idle (RO)
        I2C_STAT_ARBLST = 1 << 4, // Arbitration Lost (RO)
        I2C_STAT_DATACK = 1 << 3, // Acknowledge Data (RO)
        I2C_STAT_ADRACK = 1 << 2, // Acknowledge Address (RO)
        I2C_STAT_ERROR  = 1 << 1, // Error (RO)
        I2C_STAT_BUSY   = 1 << 0, // I2C Busy (RO)
    };

    // Useful bits in the I2CM_IMR register
    enum {                  // Description (type)
        I2C_IMR_IM = 0x01, // Interrupt Mask (RW)
    };

    // Useful bits in the I2CM_RIS register
    enum {                   // Description (type)
        I2C_RIS_BIT = 0x01, // Raw Interrupt Status (RO)
    };

    // Useful bits in the I2CM_MIS register
    enum {                   // Description (type)
        I2C_MIS_MIS = 0x01, // Masked Interrupt Status (RO)
    };

    // Useful bits in the I2CM_ICR register
    enum {                  // Description (type)
        I2C_ICR_IC = 0x01, // Interrupt Clear (WO)
    };

    // Useful bits in the I2CM_CR register
    enum {                     // Description (type)
        I2C_CR_SFE  = 1 << 5, // I2C Slave Function Enable (RW)
        I2C_CR_MFE  = 1 << 4, // I2C Master Function Enable (RW)
        I2C_CR_LPBK = 1 << 0, // I2C Loopback (RW)
    };

    // Useful bits in the I2CS_STAT register
    enum {                      // Description (type)
        I2C_STAT_FBR  = 1 << 2, // First Byte Received (RO)
        I2C_STAT_TREQ = 1 << 1, // Transmit Request (RO)
        I2C_STAT_RREQ = 1 << 0, // Receive Request (RO)
    };

    // Useful bits in the I2CS_CTRL register
    enum {                   // Description (type)
        I2C_CTRL_DA = 0x01, // Device Active (WO)
    };

    // Useful bits in the I2CS_IMR register
    enum {                         // Description (type)
        I2C_IMR_STOPIM  = 1 << 2, // Stop Condition Interrupt Mask (RO)
        I2C_IMR_STARTIM = 1 << 1, // Start Condition Interrupt Mask (RO)
        I2C_IMR_DATAIM  = 1 << 0, // Data Interrupt Mask (RW)
    };

    // Useful bits in the I2CS_RIS register
    enum {                          // Description (type)
        I2C_RIS_STOPRIS  = 1 << 2, // Stop Condition Raw Interrupt Status (RO)
        I2C_RIS_STARTRIS = 1 << 1, // Start Condition Raw Interrupt Status (RO)
        I2C_RIS_DATARIS  = 1 << 0, // Data Interrupt Status (RO)
    };

    // Useful bits in the I2CS_MIS register
    enum {                          // Description (type)
        I2C_MIS_STOPMIS  = 1 << 2, // Stop Condition Masked Interrupt Status (RO)
        I2C_MIS_STARTMIS = 1 << 1, // Start Condition Masked Interrupt Status (RO)
        I2C_MIS_DATAMIS  = 1 << 0, // Data Masked Interrupt Status (RO)
    };

    // Useful bits in the I2CS_ICR register
    enum {                          // Description (type)
        I2C_ICR_STOPIC   = 1 << 2, // Stop Condition Interrupt Clear (WO)
        I2C_ICR_STARTIC  = 1 << 1, // Start Condition Interrupt Clear (WO)
        I2C_ICR_DATAIC   = 1 << 0, // Data Interrupt Clear (WO)
    };

public:
    void config(const Role & role) {
        if(role == MASTER) {
            i2c(I2C_CR) = I2C_CR_MFE;
            i2c(I2C_TPR) = 0x3; // 400kHz, assuming a system clock of 32MHz
        } else
            i2c(I2C_CR) = I2C_CR_SFE;
    }

    bool get(char slave_address, char * data, bool stop = true) {
        // Specify the slave address and that the next operation is a read (last bit = 1)
        i2c(I2C_SA) = (slave_address << 1) | 0x01;
        return get_byte(data, I2C_CTRL_RUN | I2C_CTRL_START | (stop ? I2C_CTRL_STOP : 0));

    }

    bool put(unsigned char slave_address, char data, bool stop = true) {
        // Specify the slave address and that the next operation is a write (last bit = 0)
        i2c(I2C_SA) = (slave_address << 1);
        return put_byte(data, I2C_CTRL_RUN | I2C_CTRL_START | (stop ? I2C_CTRL_STOP : 0));
    }

    bool read(char slave_address, char * data, unsigned int size, bool stop) {
        unsigned int i;
        bool ret = true;
        // Specify the slave address and that the next operation is a read (last bit = 1)
        i2c(I2C_SA) = (slave_address << 1) | 0x01;
        for(i = 0; i < size; i++, data++) {
            if(i == 0)
                ret = get_byte(data, I2C_CTRL_START | I2C_CTRL_RUN | I2C_CTRL_ACK);
            else if(i + 1 == size)
                ret = get_byte(data, I2C_CTRL_RUN | (stop ? I2C_CTRL_STOP : 0));
            else
                ret = get_byte(data, I2C_CTRL_RUN | I2C_CTRL_ACK);
            if(!ret)
                return false;
        }
        return ret;
    }

    bool write(unsigned char slave_address, const char * data, unsigned int size, bool stop) {
        bool ret = true;
        // Specify the slave address and that the next operation is a write (last bit = 0)
        i2c(I2C_SA) = slave_address << 1;
        ret = put_byte(data[0], I2C_CTRL_RUN | I2C_CTRL_START);
        for(unsigned int i = 1; !ret && (i < size - 1); i++)
            ret = put_byte(data[i], I2C_CTRL_RUN);
        if(!ret)
            ret = put_byte(data[size-1], I2C_CTRL_RUN  | I2C_CTRL_STOP);
        return ret;
    }

    bool ready_to_get() { return ready_to_put(); }
    bool ready_to_put() { return !(i2c(I2C_STAT) & I2C_STAT_BUSY); }

private:
    bool put_byte(char data, int mode) {
        // assumes that slave address already written to I2CMSA
        i2c(I2C_DR) = data;
        i2c(I2C_CTRL) = mode;
        int timeout = 1000;
        while(ready_to_put() && timeout--);
        timeout = 1000;
        while(!ready_to_put() && timeout--);
        return (i2c(I2C_STAT) & (I2C_STAT_ERROR | I2C_STAT_ADRACK | I2C_STAT_DATACK));
    }

    bool get_byte(char * data, int mode) {
        i2c(I2C_CTRL) = mode;
        int timeout = 1000;
        while(!ready_to_get() && timeout--);
        timeout = 1000;
        while(ready_to_get() && timeout--);
        if(i2c(I2C_STAT) & (I2C_STAT_ERROR | I2C_STAT_ADRACK | I2C_STAT_DATACK)) {
            *data = 0;
            return false;
        } else {
            *data = i2c(I2C_DR);
            return true;
        }
    }

    volatile Reg32 & i2c(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

class I2C_Engine: public CC2538_I2C
{
    friend Machine;

private:
    typedef GPIO_Common::Port Port;
    typedef GPIO_Common::Pin Pin;

public:
    I2C_Engine(unsigned int unit, const Role & role) {
        Port sda_port, scl_port;
        Pin  sda_pin,  scl_pin;

        switch(unit) {
        case 0:
            sda_port = Traits<I2C>::Config<0>::SDA_PORT;
            sda_pin  = Traits<I2C>::Config<0>::SDA_PIN;
            scl_port = Traits<I2C>::Config<0>::SCL_PORT;
            scl_pin  = Traits<I2C>::Config<0>::SCL_PIN;
        }

        _i2c = new(reinterpret_cast<void *>(role == MASTER ? Memory_Map::I2C_MASTER_BASE : Memory_Map::I2C_SLAVE_BASE)) CC2538_I2C;

        PL061 * sda = new(reinterpret_cast<void *>(Memory_Map::GPIOA_BASE + (sda_port - 'A') * 0x1000)) PL061;
        PL061 * scl = new(reinterpret_cast<void *>(Memory_Map::GPIOA_BASE + (scl_port - 'A') * 0x1000)) PL061;

        scr()->clock_i2c();

        sda->select_pin_function(1 << sda_pin, PL061::FUN_ALTERNATE);
        sda->direction(1 << sda_pin, PL061::IN);
        scl->select_pin_function(1 << scl_pin, PL061::FUN_ALTERNATE);
        scl->direction(1 << scl_pin, PL061::IN);

        ioc()->enable_i2c((sda_port - 'A'), sda_pin, (scl_port - 'A'), scl_pin);

        _i2c->config(role);
    }

    bool get(char slave_address, char * data, bool stop = true) {
        return _i2c->get(slave_address, data, stop);
    }

    bool put(unsigned char slave_address, char data, bool stop = true) {
        return _i2c->put(slave_address, data, stop);
    }

    bool read(char slave_address, char * data, unsigned int size, bool stop) {
        return _i2c->read(slave_address, data, size, stop);
    }

    bool write(unsigned char slave_address, const char * data, unsigned int size, bool stop) {
        return _i2c->write(slave_address, data, size, stop);
    }

    bool ready_to_get() { return _i2c->ready_to_get(); }
    bool ready_to_put() { return _i2c->ready_to_put(); }

    // The following member functions are not yet implemented. Forwarding them to Base lands in Common and causes a linking
    // error indicating someone needs this functionality. If you see this, just open an issue in GitLab
    void flush() { return _i2c->flush(); }

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true)  { return _i2c->int_enable(receive, transmit, line, modem); }
    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true)  { return _i2c->int_disable(receive, transmit, line, modem); }

    void loopback(bool on = true) { return _i2c->loopback(); }
    void reset() { return _i2c->reset(); }

    static void init() {}

private:
    static SysCtrl * scr() { return reinterpret_cast<SysCtrl *>(Memory_Map::SCR_BASE); }
    static IOCtrl * ioc() { return reinterpret_cast<IOCtrl *>(Memory_Map::IOC_BASE); }

private:
    CC2538_I2C * _i2c;
};

__END_SYS

#endif
