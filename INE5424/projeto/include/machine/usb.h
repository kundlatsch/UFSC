// EPOS USB Mediator Common Package

#ifndef __usb_h
#define __usb_h

#include <system/config.h>
#include <architecture/cpu.h>

__BEGIN_SYS

class USB_Common
{
public:

protected:
    USB_Common() {}

public:
    char get();
    void put(char c);

    int read(char * data, unsigned int max_size);
    int write(const char * data, unsigned int size);

    void flush();
    bool ready_to_get();
    bool ready_to_put();

    void int_enable(bool receive = true, bool transmit = true, bool error = true);
    void int_disable(bool receive = true, bool transmit = true, bool error = true);
};


class USB_2_0: public USB_Common
{
public:
    enum STATE
    {
        ATTACHED,
        POWERED,
        DEFAULT,
        ADDRESS,
        CONFIGURED,
        SUSPENDED,
    };

private:
    typedef CPU::Reg16 Reg16;
    static void swap(char & a, char & b) { a ^= b; b ^= a; a ^= b; }

protected:
    enum DATA_TRANSFER_DIRECTION
    {
        HOST_TO_DEVICE = 0,
        DEVICE_TO_HOST = 1,
    };
    enum TYPE
    {
        STANDARD = 0,
        CLASS = 1,
        VENDOR = 2,
        //RESERVED = 3,
    };
    enum RECIPIENT
    {
        DEVICE = 0,
        INTERFACE = 1,
        ENDPOINT = 2,
        OTHER = 3,
        //RESERVED = 4...31
    };
    enum BREQUEST
    {
        GET_STATUS        = 0,
        CLEAR_FEATURE     = 1,
        //RESERVED          = 2,
        SET_FEATURE       = 3,
        //RESERVED         = 4,
        SET_ADDRESS       = 5,
        GET_DESCRIPTOR    = 6,
        SET_DESCRIPTOR    = 7,
        GET_CONFIGURATION = 8,
        SET_CONFIGURATION = 9,
        GET_INTERFACE     = 10,
        SET_INTERFACE     = 11,
        SYNCH_FRAME       = 12,
    };
    enum DESCRIPTOR_TYPE
    {
        DESC_DEVICE                    = 0x01,
        DESC_CONFIGURATION             = 0x02,
        DESC_STRING                    = 0x03,
        DESC_INTERFACE                 = 0x04,
        DESC_ENDPOINT                  = 0x05,
        DESC_DEVICE_QUALIFIER          = 0x06,
        DESC_OTHER_SPEED_CONFIGURATION = 0x07,
        DESC_INTERFACE_POWER           = 0x08,

        DESC_CLASS_SPECIFIC_INTERFACE  = 0x24,
        DESC_CLASS_SPECIFIC_ENDPOINT   = 0x25,
    };
    enum ENDPOINT_TYPE
    {
        EP_ATTR_CTRL = 0x00,
        EP_ATTR_ISO  = 0x01,
        EP_ATTR_BULK = 0x02,
        EP_ATTR_INT  = 0x03,
    };


public:
    class Descriptor
    {
    public:
        struct Device
        {
            unsigned bLength : 8;            // Descriptor length
            unsigned bDescriptorType : 8;    // Descriptor type (DESC_DEVICE)
            unsigned bcdUSB  : 16;           // USB version (2.0)
            unsigned bDeviceClass : 8;       // Device class
            unsigned bDeviceSubClass : 8;    // Device subclass
            unsigned bDeviceProtocol : 8;    // Device protocol
            unsigned bMaxPacketSize0 : 8;    // Max packet size for endpoint 0
            unsigned idVendor : 16;          // Vendor ID
            unsigned idProduct : 16;         // Product ID
            unsigned bcdDevice : 16;         // Device release number
            unsigned iManufacturer : 8;      // Manufacturer string index
            unsigned iProduct : 8;           // Product string index
            unsigned iSerialNumber : 8;      // Serial number string index
            unsigned bNumConfigurations : 8; // Number of possible configurations

            char & operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
            const char & operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };
        } __attribute__((packed));

        struct Configuration
        {
            unsigned bLength : 8;             // Descriptor length
            unsigned bDescriptorType : 8;     // Descriptor type (DESC_DEVICE)
            unsigned wTotalLength  : 16;      // Total length of data returned for this configuration
            unsigned bNumInterfaces : 8;      // Number of interfaces supported by this configuration
            unsigned bConfigurationValue : 8; // Value to use as an argument to the SetConfiguration() request to select this configuration
            unsigned iConfiguration : 8;      // Index of string descriptor describing this configuration
            unsigned bmAttributes : 8;        // Configuration characteristics
            unsigned bMaxPower : 8;           // Maximum power consumption from the bus in this specific configuration. Expressed in 2 mA units

            char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
            const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };
        } __attribute__((packed));

        struct Interface
        {
            unsigned bLength : 8;             // Descriptor length
            unsigned bDescriptorType : 8;     // Descriptor type (DESC_DEVICE)
            unsigned bInterfaceNumber : 8;    // Number of this interface
            unsigned bAlternateSetting : 8;   // Number Value used to select this alternate setting
            unsigned bNumEndpoints : 8;       // Number of endpoints used by this interface (excluding endpoint zero)
            unsigned bInterfaceClass : 8;     // Class code (assigned by the USB-IF)
            unsigned bInterfaceSubClass : 8;  // Subclass code (assigned by the USB-IF)
            unsigned bInterfaceProtocol : 8;  // Protocol code (assigned by the USB)
            unsigned iInterface : 8;          // Index of string descriptor describing this interface

            char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
            const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };
        } __attribute__((packed));

        struct Endpoint
        {
            unsigned bLength : 8;          // Descriptor length
            unsigned bDescriptorType : 8;  // Descriptor type (DESC_ENDPOINT)
            unsigned bEndpointAddress : 8; // Encoded address of the endpoint on the USB device described by this descriptor
            unsigned bmAttributes : 8;     // Endpoint attributes
            unsigned wMaxPacketSize : 16;  // Maximum packet size this endpoint is capable of sending or receiving at once
            unsigned bInterval : 8;        // Interval for polling endpoint for data transfers. Expressed in frames or microframes

            char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
            const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };
        } __attribute__((packed));

        //struct String //TODO
        //{
        //} __attribute__((packed));
    };

    class Request
    {
    public:
        struct Device_Request
        {
            union
            {
                unsigned bmRequestType : 8;
                struct
                {
                    unsigned data_transfer_direction : 1;
                    unsigned type : 2;
                    unsigned recipient : 5;
                } __attribute__((packed));
            } __attribute__((packed));
            unsigned bRequest : 8;
            unsigned wValue : 16;
            unsigned wIndex : 16;
            unsigned wLength : 16;

            char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
            const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };

            template<typename T>
            const T* morph() const { return reinterpret_cast<const T*>(T::check(*this) ? this : 0); }

            template<typename T>
            T* morph() { return reinterpret_cast<T*>(T::check(*this) ? this : 0); }

            friend OStream & operator<<(OStream & db, const Device_Request & d) {
                db << "Device_Request:{bmRequestType=" << d.bmRequestType << ",bRequest=" << d.bRequest << ",wValue=" << d.wValue << ",wIndex=" << d.wIndex << ",wLength=" << d.wLength << "}";
                return db;
            }
        } __attribute__((packed));

        struct Set_Address
        {
            union
            {
                unsigned bmRequestType : 8;
                struct
                {
                    unsigned data_transfer_direction : 1;
                    unsigned type : 2;
                    unsigned recipient : 5;
                } __attribute__((packed));
            } __attribute__((packed));
            unsigned bRequest : 8;
            unsigned device_address : 16;
            unsigned wIndex : 16;
            unsigned wLength : 16;

            char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
            const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };

            static bool check(const Device_Request & data) { return (data.bRequest == SET_ADDRESS) && (data.bmRequestType == 0); }

            friend OStream & operator<<(OStream & db, const Set_Address & d) {
                db << "Set_Address:{bmRequestType=" << d.bmRequestType << ",bRequest=" << d.bRequest << ",device_address=" << d.device_address << ",wIndex=" << d.wIndex << ",wLength=" << d.wLength << "}";
                return db;
            }
        } __attribute__((packed));

        struct Get_Descriptor
        {
            union
            {
                unsigned bmRequestType : 8;
                struct
                {
                    unsigned data_transfer_direction : 1;
                    unsigned type : 2;
                    unsigned recipient : 5;
                } __attribute__((packed));
            } __attribute__((packed));
            unsigned bRequest : 8;
            unsigned descriptor_index : 8;
            unsigned descriptor_type : 8;
            unsigned language_id : 16;
            unsigned descriptor_length : 16;

            char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
            const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };

            static bool check(const Device_Request & data) { return (data.bRequest == GET_DESCRIPTOR) && (data.bmRequestType == 128); }

            friend OStream & operator<<(OStream & db, const Get_Descriptor & d) {
                db << "Get_Descriptor:{bmRequestType=" << d.bmRequestType << ",bRequest=" << d.bRequest << ",descriptor_index=" << d.descriptor_index << ",descriptor_type=" << d.descriptor_type << ",language_id=" << d.language_id << ",descriptor_length=" << d.descriptor_length << "}";
                return db;
            }
        } __attribute__((packed));

        struct Set_Configuration
        {
            union
            {
                unsigned bmRequestType : 8;
                struct
                {
                    unsigned data_transfer_direction : 1;
                    unsigned type : 2;
                    unsigned recipient : 5;
                } __attribute__((packed));
            } __attribute__((packed));
            unsigned bRequest : 8;
            unsigned configuration_number : 16;
            unsigned wIndex : 16;
            unsigned wLength : 16;

            char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
            const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };

            static bool check(const Device_Request & data) { return (data.bRequest == SET_CONFIGURATION) && (data.bmRequestType == 0) && (data.wIndex == 0) && (data.wLength == 0); }

            friend OStream & operator<<(OStream & db, const Set_Configuration & d) {
                db << "Set_Configuration:{bmRequestType=" << d.bmRequestType << ",bRequest=" << d.bRequest << ",configuration_number=" << d.configuration_number << ",wIndex=" << d.wIndex << ",wLength=" << d.wLength << "}";
                return db;
            }
        } __attribute__((packed));
    };

    // Definitions for Communications Device Class
    class CDC
    {
    public:
        enum DESCRIPTOR_SUBTYPE
        {
            DESC_HEADER        = 0, // Header function descriptor
            DESC_CALL_MGMT     = 1, // Call management function descriptor
            DESC_ABS_CTRL_MGMT = 2, // Abstract control management function descriptor
            DESC_UNION_IF      = 6, // Union interface function descriptor
        };
        enum BREQUEST
        {
            SEND_ENCAPSULATED_COMMAND                    = 0x00,
            GET_ENCAPSULATED_RESPONSE                    = 0x01,
            SET_COMM_FEATURE                             = 0x02,
            GET_COMM_FEATURE                             = 0x03,
            CLEAR_COMM_FEATURE                           = 0x04,
            SET_AUX_LINE_STATE                           = 0x10,
            SET_HOOK_STATE                               = 0x11,
            PULSE_SETUP                                  = 0x12,
            SEND_PULSE                                   = 0x13,
            SET_PULSE_TIME                               = 0x14,
            RING_AUX_JACK                                = 0x15,
            SET_LINE_CODING                              = 0x20,
            GET_LINE_CODING                              = 0x21,
            SET_CONTROL_LINE_STATE                       = 0x22,
            SEND_BREAK                                   = 0x23,
            SET_RINGER_PARMS                             = 0x30,
            GET_RINGER_PARMS                             = 0x31,
            SET_OPERATION_PARMS                          = 0x32,
            GET_OPERATION_PARMS                          = 0x33,
            SET_LINE_PARMS                               = 0x34,
            GET_LINE_PARMS                               = 0x35,
            DIAL_DIGITS                                  = 0x36,
            SET_UNIT_PARAMETER                           = 0x37,
            GET_UNIT_PARAMETER                           = 0x38,
            CLEAR_UNIT_PARAMETER                         = 0x39,
            GET_PROFILE                                  = 0x3A,
            SET_ETHERNET_MULTICAST_FILTERS               = 0x40,
            SET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER = 0x41,
            GET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER = 0x42,
            SET_ETHERNET_PACKET_FILTER                   = 0x43,
            GET_ETHERNET_STATISTIC                       = 0x44,
            SET_ATM_DATA_FORMAT                          = 0x50,
            GET_ATM_DEVICE_STATISTICS                    = 0x51,
            SET_ATM_DEFAULT_VC                           = 0x52,
            GET_ATM_VC_STATISTICS                        = 0x53,
            GET_NTB_PARAMETERS                           = 0x80,
            GET_NET_ADDRESS                              = 0x81,
            SET_NET_ADDRESS                              = 0x82,
            GET_NTB_FORMAT                               = 0x83,
            SET_NTB_FORMAT                               = 0x84,
            GET_NTB_INPUT_SIZE                           = 0x85,
            SET_NTB_INPUT_SIZE                           = 0x86,
            GET_MAX_DATAGRAM_SIZE                        = 0x87,
            SET_MAX_DATAGRAM_SIZE                        = 0x88,
            GET_CRC_MODE                                 = 0x89,
            SET_CRC_MODE                                 = 0x8A,
        };

        class Functional_Descriptor
        {
        public:
            /// USB CDC header functional descriptor
            struct Header
            {
                unsigned bFunctionLength : 8;    // Descriptor length
                unsigned bDescriptorType : 8;    // Descriptor type (DESC_ENDPOINT)
                unsigned bDescriptorSubType : 8; // Descriptor subtype
                unsigned bcdCDC : 16;            // CDC specification release number

                char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
                const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };
            } __attribute__((packed));

            /// USB CDC call management functional descriptor
            struct Call_Management
            {
                unsigned bFunctionLength : 8;    // Descriptor length
                unsigned bDescriptorType : 8;    // Descriptor type (DESC_ENDPOINT)
                unsigned bDescriptorSubType : 8; // Descriptor subtype
                unsigned bmCapabilities : 8;     // Capabilities supported
                unsigned bDataInterface : 8;     // Interface number of Data Class interface optionally used for call management

                char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
                const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };
            } __attribute__((packed));

            /// USB CDC abstract control management functional descriptor
            struct Abstract_Control_Management
            {
                unsigned bFunctionLength : 8;    // Descriptor length
                unsigned bDescriptorType : 8;    // Descriptor type (DESC_ENDPOINT)
                unsigned bDescriptorSubType : 8; // Descriptor subtype
                unsigned bmCapabilities : 8;     // Capabilities supported

                char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
                const char& operator[](unsigned int idx) const  { return (reinterpret_cast<const char*>(this))[idx]; };
            } __attribute__((packed));

            /// USB CDC union interface functional descriptor
            struct Union_Interface
            {
                unsigned bFunctionLength : 8;    // Descriptor length
                unsigned bDescriptorType : 8;    // Descriptor type (DESC_ENDPOINT)
                unsigned bDescriptorSubType : 8; // Descriptor subtype
                unsigned bMasterInterface : 8;   // Interface number of the Communication or Data Class interface, designated as the master or controlling interface for the union
                unsigned bSlaveInterface0 : 8;   // Interface number of first slave or associated interface in the union

                char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
                const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };
            } __attribute__((packed));
        };

        class Request
        {
        public:
            struct Get_Line_Coding
            {
                union
                {
                    unsigned bmRequestType : 8;
                    struct
                    {
                        unsigned data_transfer_direction : 1;
                        unsigned type : 2;
                        unsigned recipient : 5;
                    } __attribute__((packed));
                } __attribute__((packed));
                unsigned bRequest : 8;
                unsigned wValue : 16;
                unsigned interface_number : 16;
                unsigned size_of_data : 16;

                struct Data_Format
                {
                    unsigned dwDTERate : 32;  // Data terminal rate, in bits per second.
                    unsigned bCharFormat : 8; // Stop bits
                    // 0 - 1 Stop bit
                    // 1 - 1.5 Stop bits
                    // 2 - 2 Stop bits
                    unsigned bParityType : 8; //0-None 1-Odd 2-Even 3-Mark 4-Space
                    unsigned bDataBits : 8;   // 5,6,7,8 or 16
                }__attribute__((packed));

                char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
                const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };

                static bool check(const USB_2_0::Request::Device_Request & data) { return (data.bRequest == GET_LINE_CODING) && (data.bmRequestType == 161) && (data.wValue == 0); }

                friend OStream & operator<<(OStream & db, const Get_Line_Coding & d) {
                    db << "Get_Line_Coding:{bmRequestType=" << d.bmRequestType << ",bRequest=" << d.bRequest << ",wValue=" << d.wValue << ",interface_number=" << d.interface_number << ",size_of_data=" << d.size_of_data << "}";
                    return db;
                }
            } __attribute__((packed));


            struct Set_Control_Line_State
            {
                union
                {
                    unsigned bmRequestType : 8;
                    struct
                    {
                        unsigned data_transfer_direction : 1;
                        unsigned type : 2;
                        unsigned recipient : 5;
                    } __attribute__((packed));
                } __attribute__((packed));
                unsigned bRequest : 8;
                unsigned DTE_present : 1;
                unsigned carrier_control : 1;
                unsigned reserved : 14;
                unsigned interface_number : 16;
                unsigned wLength : 16;

                char& operator[](unsigned int idx) { return (reinterpret_cast<char*>(this))[idx]; };
                const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };

                static bool check(const USB_2_0::Request::Device_Request & data) { return (data.bRequest == SET_CONTROL_LINE_STATE) && (data.bmRequestType == 33) && (data.wLength == 0); }

                friend OStream & operator<<(OStream & db, const Set_Control_Line_State & d) {
                    db << "Set_Control_Line_State:{bmRequestType=" << d.bmRequestType << ",bRequest=" << d.bRequest << ",reserved=" << d.reserved << ",carrier_control=" << d.carrier_control << ",DTE_present=" << d.DTE_present << ",interface_number=" << d.interface_number << ",wLength=" << d.wLength << "}";
                    return db;
                }
            } __attribute__((packed));
        };
    };

    struct Full_Config
    {
        Descriptor::Configuration _configuration_descriptor;

        Descriptor::Interface _interface0_descriptor;
        CDC::Functional_Descriptor::Header _cdc_header_descriptor;
        CDC::Functional_Descriptor::Abstract_Control_Management _cdc_acm_descriptor;
        CDC::Functional_Descriptor::Union_Interface _cdc_ui_descriptor;
        CDC::Functional_Descriptor::Call_Management _cdc_cm_descriptor;
        Descriptor::Endpoint _endpoint0_descriptor;

        Descriptor::Interface _interface1_descriptor;
        Descriptor::Endpoint _endpoint1_descriptor;
        Descriptor::Endpoint _endpoint2_descriptor;

        const char& operator[](unsigned int idx) const { return (reinterpret_cast<const char*>(this))[idx]; };
    } __attribute__((packed));
};

__END_SYS

#endif

#if defined(__USB_H) && !defined(__usb_common_only__)
#include __USB_H
#endif
