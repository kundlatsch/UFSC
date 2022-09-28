// EPOS EPOSMote III (ARM Cortex-M3) USB Mediator Declarations

#ifndef __emote3_usb_h
#define __emote3_usb_h

#include <architecture/cpu.h>
#include <machine/usb.h>
#include <machine/ic.h>
#include <machine/cortex/engine/pl061.h>

__BEGIN_SYS

class USB_Engine: public USB_2_0
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::USB0_BASE) USB_Engine"

private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg32 Reg32;

public:
    // Maximum packet sizes for each end-point
    static const unsigned int _max_packet_ep0 = 32;
    static const unsigned int _max_packet_ep1 = 32;
    static const unsigned int _max_packet_ep2 = 64;
    static const unsigned int _max_packet_ep3 = 128;
    static const unsigned int _max_packet_ep4 = 256;
    static const unsigned int _max_packet_ep5 = 512;

    // Registers offsets from BASE (i.e. this)
    enum {
    //  Name        Offset   Type  Width   Reset Value    Physical Address
        ADDR      = 0x00, //   RW   32     0x0000 0000    0x4008 9000
        POW       = 0x04, //   RW   32     0x0000 0000    0x4008 9004
        IIF       = 0x08, //   RO   32     0x0000 0000    0x4008 9008
        OIF       = 0x10, //   RO   32     0x0000 0000    0x4008 9010
        CIF       = 0x18, //   RO   32     0x0000 0000    0x4008 9018
        IIE       = 0x1C, //   RW   32     0x0000 003F    0x4008 901C
        OIE       = 0x24, //   RW   32     0x0000 003E    0x4008 9024
        CIE       = 0x2C, //   RW   32     0x0000 0006    0x4008 902C
        FRML      = 0x30, //   RO   32     0x0000 0000    0x4008 9030
        FRMH      = 0x34, //   RO   32     0x0000 0000    0x4008 9034
        INDEX     = 0x38, //   RW   32     0x0000 0000    0x4008 9038
        CTRL      = 0x3C, //   RW   32     0x0000 0000    0x4008 903C
        MAXI      = 0x40, //   RW   32     0x0000 0000    0x4008 9040
        CS0_CSIL  = 0x44, //   RW   32     0x0000 0000    0x4008 9044
        CSIH      = 0x48, //   RW   32     0x0000 0020    0x4008 9048
        MAXO      = 0x4C, //   RW   32     0x0000 0000    0x4008 904C
        CSOL      = 0x50, //   RW   32     0x0000 0000    0x4008 9050
        CSOH      = 0x54, //   RW   32     0x0000 0000    0x4008 9054
        CNT0_CNTL = 0x58, //   RO   32     0x0000 0000    0x4008 9058
        CNTH      = 0x5C, //   RO   32     0x0000 0000    0x4008 905C
        F0        = 0x80, //   RW   32     0x0000 0000    0x4008 9080
        F1        = 0x88, //   RW   32     0x0000 0000    0x4008 9088
        F2        = 0x90, //   RW   32     0x0000 0000    0x4008 9090
        F3        = 0x98, //   RW   32     0x0000 0000    0x4008 9098
        F4        = 0xA0, //   RW   32     0x0000 0000    0x4008 90A0
        F5        = 0xA8, //   RW   32     0x0000 0000    0x4008 90A8
    };

    // Useful bits in CTRL
    enum {
      //Name        Offset     Description                                                        Type Reset Value
        PLLLOCKED = 1 << 7, // PLL lock status. The PLL is locked when USB_CTRL.PLLLOCKED is 1.     RO 0
        PLLEN     = 1 << 1, // 48 MHz USB PLL enable When this bit is set, the 48 MHz PLL is        RW 0
                            // started. Software must avoid access to other USB registers before
                            // the PLL has locked; that is, USB_CTRL.PLLLOCKED is 1. This bit
                            // can be set only when USB_CTRL.USBEN is 1.  The PLL must be
                            // disabled before entering PM1 when suspended, and must be re-
                            // enabled when resuming operation.
        USBEN     = 1 << 0, // USB enable. The USB controller is reset when this bit is cleared     RW 0
    };

    // Useful bits in POW
    enum {
      //Name        Offset      Description                                                        Type Reset Value
        ISOWAITSOF = 1 << 7, // For isochronous mode IN endpoints:                                   RW 0
                             // When set, the USB controller will wait for an SOF token from the
                             // time USB_CSIL.INPKTRDY is set before sending the packet. If an
                             // IN token is received before an SOF token, then a zero length data
                             // packet will be sent.
       RST         = 1 << 3, // Indicates that reset signaling is present on the bus                 RO 0
       RESUME      = 1 << 2, // Drives resume signaling for remote wakeup
                             // According to the USB Specification, the resume signal must be held   RW 0
                             // active for at least 1 ms and no more than 15 ms. It is recommended
                             // to keep this bit set for approximately 10 ms.
       SUSPEND     = 1 << 1, // Indicates entry into suspend mode                                    RO 0
                             // Suspend mode must be enabled by setting
                             // USB_POW.SUSPENDEN
                             // Software clears this bit by reading the USB_CIF register or by
                             // asserting USB_POW.RESUME
       SUSPENDEN   = 1 << 0, // Enables detection of and entry into suspend mode.                    RW 0
    };

    // Common to CIE, CIF registers
    enum {
      //Name          Offset     Description
        INT_SOF     = 1 << 3, // Start-of-Frame interrupt flag/enable
        INT_RESET   = 1 << 2, // Reset interrupt flag/enable
        INT_RESUME  = 1 << 1, // Resume interrupt flag/enable
        INT_SUSPEND = 1 << 0, // Suspend interrupt flag/enable
    };

    // Useful bits in CS0
    enum {
      //Name              Offset     Description                                                     Type Reset
       CS0_CLROUTPKTRDY = 1 << 6, // Software sets this bit to clear the USB_CS0.OUTPKTRDY bit. It is  RW 0
                                  // cleared automatically.
       CS0_SENDSTALL    = 1 << 5, // Software sets this bit to terminate the current transaction with  RW 0
                                  // a STALL handshake. The bit is cleared automatically when the STALL
                                  // handshake has been transmitted.
       CS0_SETUPEND     = 1 << 4, // This bit is set when a control transaction ends before the        RO 0
                                  // USB_CS0.DATAEND bit has been set. An interrupt is generated
                                  // and the FIFO flushed at this time. Software clears this bit by
                                  // setting USB_CS0.CLRSETUPEND.
       CS0_DATAEND      = 1 << 3, // This bit is used to signal the end of the data stage, and must    RW 0
                                  // be set:
                                  // 1. When the last data packet is loaded and USB_CS0.INPKTRDY is
                                  // set.
                                  // 2. When the last data packet is unloaded and
                                  // USB_CS0.CLROUTPKTRDY is set.
                                  // 3. When USB_CS0.INPKTRDY is set to send a zero-length packet.
                                  // The USB controller clears this bit automatically.
       CS0_SENTSTALL    = 1 << 2, // This bit is set when a STALL handshake is sent. An interrupt is    RW 0
                                  // generated is generated when this bit is set. Software must clear
                                  // this bit.
       CS0_INPKTRDY     = 1 << 1, // Software sets this bit after loading a data packet into the        RW 0
                                  // endpoint 0 FIFO. It is cleared automatically when the data packet
                                  // has been transmitted. An interrupt is generated when the bit is cleared.
       CS0_OUTPKTRDY    = 1 << 0, // Endpoint 0 data packet received                                    RO 0
                                  // An interrupt request (EP0) is generated if the interrupt is enabled.
                                  // Software must read the endpoint 0 FIFO empty, and clear this bit by
                                  // setting USB_CS0.CLROUTPKTRDY
    };

    // Useful bits in CSOL
    enum {
      //Name              Offset     Description                                                     Type Reset
        CSOL_CLRDATATOG  = 1 << 7, // Software sets this bit to reset the endpoint data toggle to 0. RW 0
        CSOL_SENTSTALL   = 1 << 6, // This bit is set when a STALL handshake is transmitted. An interrupt is generated when this bit is set. Software should clear this bit. RW 0
        CSOL_SENDSTALL   = 1 << 5, // For bulk/interrupt mode OUT endpoints: Software sets this bit to issue a STALL handshake.  Software clears this bit to terminate the stall condition. RW 0
        CSOL_FLUSHPACKET = 1 << 4, // Software sets this bit to flush the next packet to be read from the endpoint OUT FIFO.  Note: If the FIFO contains two packets, USB_CSOL.FLUSHPACKET will need to be set twice to completely clear the FIFO. RW 0
        CSOL_DATAERROR   = 1 << 3, // For isochronous mode OUT endpoints: This bit is set when USB_CSOL.OUTPKTRDY is set if the data packet has a CRC or bit-stuff error. It is cleared automatically when USB_CSOL.OUTPKTRDY is cleared. RO 0
        CSOL_OVERRUN     = 1 << 2, // For isochronous mode OUT endpoints: This bit is set when an OUT packet cannot be loaded into the OUT endpoint FIFO. Firmware should clear this bit. RW 0
        CSOL_FIFOFULL    = 1 << 1, // This bit is set when no more packets can be loaded into the OUT endpoint FIFO. RO 0
        CSOL_OUTPKTRDY   = 1 << 0, // This bit is set when a data packet has been received. Software should clear this bit when the packet has been unloaded from the OUT endpoint FIFO. An interrupt is generated when the bit is set. RW 0
    };

    // Useful bits in CSIH
    enum {
      //Name           Offset     Description                                                          Type Reset
        AUTISET      = 1 << 7, // If set by software, the CSIL.INPKTRDY bit is automatically set when    RW 0
                               // a data packet of maximum size (specified by USBMAXI) is loaded into
                               // the IN endpoint FIFO. If a packet of less than the maximum packet
                               // size is loaded, then CSIL.INPKTRDY will have to be set manually.
        ISO          = 1 << 6, // Selects IN endpoint type: 0: Bulk/interrupt 1: Isochronous             RW 0
        FORCEDATATOG = 1 << 3, // Software sets this bit to force the IN endpoint's data toggle to       RW 0
                               // switch after each data packet is sent regardless of whether an ACK
                               // was received. This can be used by interrupt IN endpoints which are
                               // used to communicate rate feedback for isochronous endpoints.
        INDBLBUF     = 1 << 0, // IN endpoint FIFO double-buffering enable.                              RW 0
    };

    // Useful bits in CSIL
    enum {
      //Name              Offset     Description                                                         Type Reset
        CSIL_CLRDATATOG  = 1 << 6, // Software sets this bit to reset the IN endpoint data toggle to 0.    RW 0
        CSIL_SENTSTALL   = 1 << 5, // For bulk/interrupt mode IN endpoints: This bit is set when a STALL   RW 0
                                   // handshake is transmitted. The FIFO is flushed and the INPKTRDY bit
                                   // cleared. Software should clear this bit.
        CSIL_SENDSTALL   = 1 << 4, // For bulk/interrupt mode IN endpoints: Software sets this bit to      RO 0
                                   // issue a STALL handshake. Software clears this bit to terminate the
                                   // stall condition.
        CSIL_FLUSHPACKET = 1 << 3, // Software sets this bit to flush the next packet to be transmitted    RW 0
                                   // from the IN endpoint FIFO. The FIFO pointer is reset and the
                                   // INPKTRDY bit is cleared.  Note: If the FIFO contains two packets,
                                   // FLUSHPACKET will need to be set twice to completely clear the FIFO.
        CSIL_UNDERRUN    = 1 << 2, // In isochronous mode, this bit is set when a zero length data packet  RW 0
                                   // is sent after receiving an IN token with USB_CSIL.INPKTRDY not set.
                                   // In bulk/interrupt mode, this bit is set when a NAK is returned in
                                   // response to an IN token. Software should clear this bit.
        CSIL_PKTPRESENT  = 1 << 1, // This bit is set when there is at least one packet in the IN endpoint RW 0
                                   // FIFO.
        CSIL_INPKTRDY    = 1 << 0, // IN endpoint {1-5} packet transfer pending Software sets this bit     RO 0
                                   // after loading a data packet into the FIFO. It is cleared automati-
                                   // cally when a data packet has been transmitted. An interrupt is gene-
                                   // rated (if enabled) when the bit is cleared.  When using double-buff-
                                   // ering, the bit is cleared immediately if the other FIFO is empty.
    };

public:
    char get();
    void put(char c);

    int read(char * data, unsigned int max_size);
    int write(const char * data, unsigned int size);

    void flush() { usb(CS0_CSIL) |= CSIL_INPKTRDY; }

    bool ready_to_get() {
        if(!configured())
            return false;
        else {
            lock();
            input();
            bool ret = usb(CSOL) & CSOL_OUTPKTRDY;
            unlock();
            return ret;
        }
    }

    bool ready_to_put() { return _ready_to_put; }

    void int_enable(bool receive = true, bool transmit = true, bool error = true) { IC::disable(IC::INT_USB0); }
    void int_disable(bool receive = true, bool transmit = true, bool error = true);

    void reset();

    void eoi() {
        // USB interrupt flags are cleared when read
        _cif |= usb(CIF);
        _iif |= usb(IIF);
        _oif |= usb(OIF);
    }

    void power(const Power_Mode & mode) {
        PL061 * pl061 = new(reinterpret_cast<void *>(Memory_Map::GPIOC_BASE)) PL061;

        switch(mode) {
        case ENROLL:
            break;
        case DISMISS:
            break;
        case SAME:
            break;
        case FULL: {
            // Set D+ USB pull-up resistor, which is controlled by GPIO pin C2 in eMote3
            pl061->select_pin_function(PL061::PIN2, PL061::FUN_GPIO);
            pl061->direction(PL061::PIN2, PL061::OUT);
            pl061->set(PL061::PIN2, true);
        } break;
        case LIGHT:
        case SLEEP:
            break;
        case OFF:
            disable();
            pl061->clear(PL061::PIN2);
            break;
        }
    }

    void handle_int(IC::Interrupt_Id interrupt);

    void init();

private:
    bool configured() { return state() >= USB_2_0::STATE::CONFIGURED; }
    USB_2_0::STATE state() { return _state; }

    void endpoint(int index) { usb(INDEX) = index; }
    int endpoint() { return usb(INDEX); }
    void control() { endpoint(0); }
    void output() { endpoint(3); }
    void input() { endpoint(4); }

    void lock() {
        _locked = CPU::int_disabled();
        // Concerning the correctness of _locked if a rescheduling happens between these two lines:
        // 1) If _locked was true, no rescheduling happens. OK.
        // 2) If _locked was false and interrupts are still enabled when execution returns, we're OK.
        // 3) It seems like an error if _locked was false and execution comes back with interrupts disabled.
        // So _locked should be correct.
        CPU::int_disable();
    }

    void unlock() {
        if(!_locked)
            CPU::int_enable();
    }

    void enable() { usb(CTRL) |= USBEN; }
    void disable() { usb(CTRL) &= ~USBEN; }

    bool handle_ep0(const USB_2_0::Request::Device_Request & data);

private:
    static volatile bool _ready_to_put; // TODO: isn't it the wrong semantics?
    static volatile bool _ready_to_put_next;
    static volatile bool _locked;
    static volatile USB_2_0::STATE _state;
    static const Full_Config _config;

    static const char * _send_buffer;
    static unsigned int _send_buffer_size;

    static Reg32 _cif;
    static Reg32 _iif;
    static Reg32 _oif;
    static const Descriptor::Device _device_descriptor;

    volatile Reg32 & usb(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(Memory_Map::USB0_BASE)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
