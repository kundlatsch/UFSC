// EPOS Cortex-M USB Mediator Implementation

#include <machine/ic.h>
#include <machine/usb.h>

#ifdef __USB_H

__USING_SYS

const char * USB_Engine::_send_buffer = reinterpret_cast<const char *>(0);
unsigned int USB_Engine::_send_buffer_size = 0;
CPU::Reg32 USB_Engine::_cif;
CPU::Reg32 USB_Engine::_iif;
CPU::Reg32 USB_Engine::_oif;
volatile USB_2_0::STATE USB_Engine::_state;
volatile bool USB_Engine::_ready_to_put = false;
volatile bool USB_Engine::_ready_to_put_next = false;
volatile bool USB_Engine::_locked = false;

char USB_Engine::get()
{
    lock();
    input();

    while(!(usb(CSOL) & CSOL_OUTPKTRDY));
    unsigned int sz = (usb(CNTH) << 8) | usb(CNT0_CNTL);
    char ret = usb(F4);
    if(sz == 1)
        usb(CSOL) &= ~CSOL_OUTPKTRDY;

    unlock();
    return ret;
}

int USB_Engine::read(char * out, unsigned int max_size)
{
    lock();
    input();
    int cont = 15;
    while(!(usb(CSOL) & CSOL_OUTPKTRDY) && cont) cont--;
    unsigned int i;
    unsigned int sz = (usb(CNTH) << 8) | usb(CNT0_CNTL);
    for(i = 0; (i<sz) && (i<max_size); i++)
        out[i] = usb(F4);
    if(i>=sz)
        usb(CSOL) &= ~CSOL_OUTPKTRDY;

    unlock();
    return i;
}

void USB_Engine::put(char c)
{
    if(Traits<USB>::blocking)
        while(!ready_to_put());
    else
        if(!ready_to_put())
            return;

    lock();
    output();

    while(usb(CS0_CSIL) & CSIL_INPKTRDY);
    usb(F3) = c;

    flush();
    unlock();
}

int USB_Engine::write(const char * c, unsigned int size)
{
    if(Traits<USB>::blocking) {
        while(!ready_to_put());
    } else {
        if(!ready_to_put())
            return 0;
    }

    lock();
    output();
    while(usb(CS0_CSIL) & CSIL_INPKTRDY);
    for(unsigned int i = 0; (i < _max_packet_ep3) and (i < size); i++)
        usb(F3) = c[i];
    flush();
    unlock();

    return size;
}

bool USB_Engine::handle_ep0(const USB_2_0::Request::Device_Request & data)
{
    switch(data.bRequest)
    {
    case SET_ADDRESS:
        if(auto d = data.morph<Request::Set_Address>())
        {
            db<USB>(TRC) << *d << endl;
            usb(ADDR) = d->device_address;
            _state = USB_2_0::STATE::ADDRESS;
            return true;
        }
        break;

    case GET_DESCRIPTOR:
        if(auto d = data.morph<Request::Get_Descriptor>())
        {
            db<USB>(TRC) << *d << endl;
            if(d->descriptor_type == DESC_DEVICE)
            {
                _send_buffer = reinterpret_cast<const char *>(&_device_descriptor);
                _send_buffer_size = (sizeof _device_descriptor) > (d->descriptor_length) ? (d->descriptor_length) : (sizeof _device_descriptor);
                return true;
            }

            // DESC_DEVICE_QUALIFIER not supported

            if(d->descriptor_type == DESC_CONFIGURATION)
            {
                _send_buffer = reinterpret_cast<const char *>(&_config);
                _send_buffer_size = (sizeof _config) > (d->descriptor_length) ? (d->descriptor_length) : (sizeof _config);
                return true;
            }
        }
        break;

    case SET_CONFIGURATION:
        if(auto d = data.morph<Request::Set_Configuration>())
        {
            db<USB>(TRC) << *d << endl;
            bool ret = (d->configuration_number == 1);
            if(ret) {
                _state = USB_2_0::STATE::CONFIGURED;
            }
            return ret;
        }
        break;

    case CDC::GET_LINE_CODING:
        if(auto d = data.morph<CDC::Request::Get_Line_Coding>())
        {
            static CDC::Request::Get_Line_Coding::Data_Format data;// = new CDC::Request::Get_Line_Coding::Data_Format();
            data.dwDTERate = 1/Traits<UART>::DEF_BAUD_RATE;
            switch(Traits<UART>::DEF_STOP_BITS)
            {
            case 1: data.bCharFormat = 0; break;
            case 2: data.bCharFormat = 2; break;
            default: return false;
            }
            data.bParityType = Traits<UART>::DEF_PARITY;
            data.bDataBits = Traits<UART>::DEF_DATA_BITS;

            _send_buffer = reinterpret_cast<const char *>(&data);
            _send_buffer_size = (sizeof (data)) > (d->size_of_data) ? (d->size_of_data) : (sizeof (data));
            return true;
        }
        break;

    case CDC::SET_LINE_CODING:
        return true;

    case CDC::SET_CONTROL_LINE_STATE:
        if(auto d = data.morph<CDC::Request::Set_Control_Line_State>())
        {
            db<USB>(TRC) << *d << endl;
            _ready_to_put_next = d->DTE_present;
            return true;
        }
        break;

    default:
        break;
    }

    return false;
}

void USB_Engine::handle_int(IC::Interrupt_Id interrupt)
{
    Reg32 index = endpoint(); // Save old index

    Reg32 flags;
    if((flags = _cif)) {
        _cif = 0;
        if(flags & INT_RESET)
            reset();
        db<USB>(TRC) << "USB::CIF = " << flags << endl;
    }

    if((flags = _iif)) {
        _iif = 0;
        if(flags & (1 << 0)) { // Endpoint 0 interrupt
            if((not _ready_to_put) and _ready_to_put_next) {
                _ready_to_put = true;
                put(' '); // For some reason the first put() is not shown in minicom
            }

            control();
            if(usb(CS0_CSIL) & CS0_OUTPKTRDY) { // Data present
                db<USB>(TRC) << "Endpoint 0 command received:";
                // Read command from endpoint 0 FIFO
                USB_2_0::Request::Device_Request data;
                Reg8 fifocnt = usb(CNT0_CNTL);
                for(unsigned int i=0; (i < 8) && (i < fifocnt); i++) {
                    data[i] = usb(F0);
                    db<USB>(TRC) << " " << (int) data[i];
                }
                db<USB>(TRC) << endl;

                db<USB>(TRC) << data << endl;

                if(handle_ep0(data)) {
                    if(_send_buffer)
                        usb(CS0_CSIL) |= CS0_CLROUTPKTRDY;
                    else
                        usb(CS0_CSIL) |= CS0_CLROUTPKTRDY | CS0_DATAEND;
                    db<USB>(TRC) << "command processed" << endl;
                } else {
                    // Signal that the command could not be executed
                    usb(CS0_CSIL) |= CS0_CLROUTPKTRDY | CS0_SENDSTALL;
                    db<USB>(WRN) << "USB::int_handler: command NOT processed" << endl;
                }
            }

            // Send part of the requested buffer that fits in the FIFO
            if(_send_buffer) {
                for(unsigned int i=0; (_send_buffer_size > 0) && (i<_max_packet_ep0); i++, _send_buffer++, _send_buffer_size--)
                    usb(F0) = *_send_buffer;

                if(_send_buffer_size == 0) {
                    // Signal that packet is ready and no further data is expected for this request
                    usb(CS0_CSIL) |= CS0_INPKTRDY | CS0_DATAEND;
                    _send_buffer = reinterpret_cast<const char *>(0);
                } else
                    // Signal that packet is ready
                    usb(CS0_CSIL) |= CS0_INPKTRDY;
            }
        }

        db<USB>(TRC) << "IIF = " << flags << endl;
    }

    if((flags = _oif)) {
        _oif = 0;
        db<USB>(TRC) << "OIF = " << flags << endl;
    }

    endpoint(index); // Restore old index
}

#endif
