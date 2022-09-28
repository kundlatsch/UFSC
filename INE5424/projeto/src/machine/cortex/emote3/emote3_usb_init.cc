// EPOS EPOSMote III (ARM Cortex-M3) USB Mediator Declarations

#include <machine.h>
#include <machine/ic.h>
#include <machine/usb.h>
#include <machine/cortex/emote3/emote3_sysctrl.h>

__USING_SYS

#ifdef __USB_H

const USB_2_0::Descriptor::Device USB_Engine::_device_descriptor =
{
    sizeof(Descriptor::Device), // Descriptor length
    DESC_DEVICE,               // Descriptor type
    0x0200,                    // USB version 2.0
    0x02,                      // Device class (CDC)
    0x00,                      // Device subclass
    0x00,                      // Device protocol
    _max_packet_ep0,           // Max packet size for endpoint 0
    0x1d50,                    // Vendor ID (Openmoko)
    0x601c,                    // Product ID (Epos Mote II)
    0x03,                      // Device release number
    0x00,                      // Manufacturer string index
    0x00,                      // Product string index
    0x00,                      // Serial number string index
    0x01,                      // Number of possible configurations
};

const USB_Engine::Full_Config USB_Engine::_config =
{
    //_configuration_descriptor =
    {
        sizeof(Descriptor::Configuration), // Descriptor length
        DESC_CONFIGURATION,              // Descriptor type (DESC_CONFIGURATION)
        sizeof(Full_Config),         // Total length of data returned for this configuration
        0x02,                            // Number of interfaces supported by this configuration
        0x01,                            // Value to use as an argument to the SetConfiguration() request to select this configuration
        0x00,                            // Index of string descriptor describing this configuration
        0xA0,                            // Configuration characteristics (remote wakeup = 1)
        25                               // Maximum power consumption from the bus in this specific configuration. (50mA)
    },
    //_interface0_descriptor =
    {
        sizeof(Descriptor::Interface), // Descriptor length
        DESC_INTERFACE,               // Descriptor type (DESC_INTERFACE)
        0x00,                         // Number of this interface
        0x00,                         // Number Value used to select this alternate setting
        0x01,                         // Number of endpoints used by this interface (excluding endpoint zero)
        0x02,                         // Class code (CDC)
        0x02,                         // Subclass code (Abstract control model)
        0x01,                         // Protocol code (V25TER)
        0x00                          // Index of string descriptor describing this interface
    },
    //_cdc_header_descriptor =
    {
        sizeof(CDC::Functional_Descriptor::Header),
        DESC_CLASS_SPECIFIC_INTERFACE,
        CDC::DESC_HEADER,
        0x0110
    },
    //_cdc_acm_descriptor =
    {
        sizeof(CDC::Functional_Descriptor::Abstract_Control_Management),
        DESC_CLASS_SPECIFIC_INTERFACE,
        CDC::DESC_ABS_CTRL_MGMT,
        0x02
    },
    //_cdc_ui_descriptor =
    {
        sizeof(CDC::Functional_Descriptor::Union_Interface),
        DESC_CLASS_SPECIFIC_INTERFACE,
        CDC::DESC_UNION_IF,
        0x00,
        0x01
    },
    //_cdc_cm_descriptor =
    {
        sizeof(CDC::Functional_Descriptor::Call_Management),
        DESC_CLASS_SPECIFIC_INTERFACE,
        CDC::DESC_CALL_MGMT,
        0x00,
        0x01
    },
    //_endpoint0_descriptor =
    {
        sizeof(Descriptor::Endpoint),// Descriptor length
        DESC_ENDPOINT,               // Descriptor type (DESC_ENDPOINT)
        2 | (1 << 7),                // Encoded address: Endpoint 2, IN
        EP_ATTR_INT,                 // Endpoint attributes (Interrupt endpoint)
        _max_packet_ep2,             // Maximum packet size this endpoint is capable of sending or receiving at once
        0x40                         // Interval for polling endpoint (64 full-speed frames = 64 ms)
    },
    //_interface1_descriptor =
    {
        sizeof(Descriptor::Interface),
        DESC_INTERFACE,
        0x01,
        0x00,
        0x02,
        0x0A,
        0x00,
        0x00,
        0x00
    },
    //_endpoint1_descriptor =
    {
        sizeof(Descriptor::Endpoint),// Descriptor length
        DESC_ENDPOINT,               // Descriptor type (DESC_ENDPOINT)
        3 | (1 << 7),                // Encoded Address: Endpoint 3, IN
        EP_ATTR_BULK,                // Endpoint attributes (Bulk endpoint)
        32,                          // Maximum packet size this endpoint is capable of sending or receiving at once
        0x00                         // Interval (ignored for Bulk operation)
    },
    //_endpoint2_descriptor =
    {
        sizeof(Descriptor::Endpoint),// Descriptor length
        DESC_ENDPOINT,               // Descriptor type (DESC_ENDPOINT)
        4,                           // Encoded address: Endpoint 4, OUT
        EP_ATTR_BULK,                // Endpoint attributes (Bulk endpoint)
        _max_packet_ep4,             // Maximum packet size this endpoint is capable of sending or receiving at once
        0x00                         // Interval (ignored for Bulk operation)
    }
};

// Configurations that need to be done at every USB reset
void USB_Engine::reset()
{
    _ready_to_put = false;
    _ready_to_put_next = false;
    _state = USB_2_0::STATE::DEFAULT;
    _send_buffer = reinterpret_cast<const char *>(0);
    _send_buffer_size = 0;

    // Set up endpoints
    output();
    // The two lines below make the USB automatically signal that a packet is ready every 32 characters.
    usb(MAXI) = 32 / 8; // Endpoint 3, IN.
    usb(MAXO) = 0;
    usb(CS0_CSIL) |= CSIL_CLRDATATOG; // From cc2538 User Guide: When a Bulk IN endpoint is first configured, USB_CSIL.CLRDATATOG should be set.
    // if there are any data packets in the FIFO, they should be flushed. It may be necessary to set this bit twice in succession if double buffering is enabled.
    usb(CS0_CSIL) |= CSIL_FLUSHPACKET;
    usb(CS0_CSIL) |= CSIL_FLUSHPACKET;

    input();
    usb(MAXI) = 0;
    usb(MAXO) = _max_packet_ep4 / 8; // Endpoint 4, OUT
    usb(CSOL) |= CSOL_CLRDATATOG; // From cc2538 User Guide: When a Bulk OUT endpoint is first configured, USB_CSOL.CLRDATATOG should be set.
    // if there are any data packets in the FIFO, they should be flushed
    usb(CSOL) |= CSOL_FLUSHPACKET;

    // Only enable IN interrupts for endpoints 0 and 2
    usb(IIE) = (1 << 0);// | (1 << 2);
    // Only enable OUT interrupts for endpoint 0
    usb(OIE) = (1 << 0);
    // Only enable RESET common interrupt (disable start-of-frame, resume and suspend)
    usb(CIE) = INT_RESET;
}

void USB_Engine::init()
{
    SysCtrl * scr = reinterpret_cast<SysCtrl *>(Memory_Map::SCR_BASE);
    scr->clock_usb();
    power(FULL);

    // Reset USB
    //usb(CTRL) = 0;

    // Enable USB controller
    usb(CTRL) |= USBEN;

    // Enable PLL and wait for it to initialize
    usb(CTRL) |= PLLEN;
    while(!(usb(CTRL) & PLLLOCKED));

    // Do not suspend the device when USB is idle
    usb(POW) &= ~SUSPENDEN;

    reset();

    _state = USB_2_0::STATE::POWERED;


    if (Traits<USB>::wait_to_sync) {
        //Necessary to use USB early on the execution
        bool a = CPU::int_disabled();
        if(a)
            CPU::int_enable();
        Machine::delay(2000000);
        if(a)
            CPU::int_enable();
    }
}

#endif
