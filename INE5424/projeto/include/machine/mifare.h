// Identification cards - Contactless integrated circuit(s) cards - Proximity cards standard (ISO/IEC 14443-3)

#ifndef __mifare_h
#define __mifare_h

#include <architecture/cpu.h>

__BEGIN_SYS

class MIFARE
{
    typedef CPU::Reg8 Reg8;

public:
    // MIFARE constants
    enum MIFARE_Misc {
        MF_ACK       = 0xA,
        MF_KEY_SIZE  = 6,
    };

    // Commands sent to the MIFARE PICC (proximity integrated circuit card).
    // The commands used by the PCD (reader) to manage communication
    // with several PICCs (cards) (ISO 14443-3, Type A, section 6.4)
    enum PICC_Command {
        REQA              = 0x26,    // Request command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollisionor selection.
        WUPA              = 0x52,    // Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY and prepare foranticollision or selection.
        CT                = 0x88,    // Cascade Tag. Used during anti collision.
        SEL_CL1           = 0x93,    // Anti collision/Select, Cascade Level 1
        SEL_CL2           = 0x95,    // Anti collision/Select, Cascade Level 2
        SEL_CL3           = 0x97,    // Anti collision/Select, Cascade Level 3
        HLTA              = 0x50,    // Halt command, Type A. Instructs an ACTIVE PICC to go to state HALT.
        // MIFARE Classic commands (http://www.mouser.com/ds/2/302/MF1S503x-89574.pdf, Section 9.1)
        // Use MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
        MF_AUTH_KEY_A     = 0x60,   // Perform authentication with Key A
        MF_AUTH_KEY_B     = 0x61,   // Perform authentication with Key B
        MF_READ           = 0x30,   // Reads one 16 byte block from the authenticated sector of the PICC.
        MF_WRITE          = 0xA0,   // Writes one 16 byte block to the authenticated sector of the PICC.
        MF_DECREMENT      = 0xC0,    // Decrements the contents of a block and stores the result in the internal data register.
        MF_INCREMENT      = 0xC1,    // Increments the contents of a block and stores the result in the internal data register.
        MF_RESTORE        = 0xC2,    // Reads the contents of a block into the internal data register.
        MF_TRANSFER       = 0xB0,     // Writes the contents of the internal data register to a block.
        // MIFARE Ultralight commands (http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 7.6)
        // The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
        PICC_CMD_UL_WRITE = 0xA2    // Writes one 4 byte page to the PICC.
    };

    // PICC types
    enum PICC_Type {
        MIFARE_UL    = 0x00, // MIFARE Ultralight or Ultralight C
        NOT_COMPLETE = 0x04, // SAK of an incomplete UID
        MIFARE_1K    = 0x08, // MIFARE Classic - 1KB
        MIFARE_MINI  = 0x09, // MIFARE Classic - 320 bytes
        MIFARE_PLUS  = 0x11, // MIFARE Plus
        MIFARE_4K    = 0x18, // MIFARE Classic - 4KB
        ISO_14443_4  = 0x20, // PICC compliant with ISO/IEC 14443-4
        ISO_18092    = 0x40, // PICC compliant with ISO/IEC 18092
    };

    class UID
    {
    public:
        static const unsigned int SIZE_MAX = 10;

        UID() : _size(0) {}
        UID(const void * id, unsigned int size = SIZE_MAX, Reg8 sak = 0) {
            uid(id, size);
            _uid_sak[_size] = sak;
        }
        UID(unsigned int v) : _size(4) {
            unsigned int i;
            for(i = 0; i < 4; i++)
                _uid_sak[i] = reinterpret_cast<Reg8*>(&v)[i];
            for(; i < SIZE_MAX + 1; i++)
                _uid_sak[i] = 0;
        }
        UID(unsigned long long v) : _size(10) { // ID size must be 4, 7, or 10
            unsigned int i;
            for(i = 0; i < 8; i++)
                _uid_sak[i] = reinterpret_cast<Reg8*>(&v)[i];
            for(; i < SIZE_MAX + 1; i++)
                _uid_sak[i] = 0;
        }

        const Reg8 * uid() const { return _uid_sak; }
        Reg8 * uid() { return _uid_sak; }

        void sak(Reg8 s) { _uid_sak[_size] = s; }
        PICC_Type type() const { return static_cast<PICC_Type>(_uid_sak[_size] & 0x7F); }

        Reg8 size() const { return _size; }
        void size(unsigned int s) { assert(_size <= SIZE_MAX); _size = s; }

        void uid(const void * u, unsigned int s) { size(s); uid(u); }
        void uid(const void * u) {
            for(unsigned int i = 0; i < SIZE_MAX; i++)
                _uid_sak[i] = (i < _size) ? reinterpret_cast<const Reg8*>(u)[i] : 0;
        }

        operator unsigned int() const {
            unsigned long long ret = 0;
            for(unsigned int i = 0; i < SIZE_MAX; i++)
                ret ^= _uid_sak[i] << ((i % sizeof(unsigned int)) * 8);
            return ret;
        }

        friend Debug & operator<<(Debug & db, const UID & u) {
            db << "{s=" << u._size << ",u=";
            db << hex << u._uid_sak[0];
            for(unsigned int i = 1; i < u._size; i++)
                db << "," << hex << u._uid_sak[i];
            db << ",sa=" << u._uid_sak[u._size] << "}";
            return db;
        }

        friend OStream & operator<<(OStream & os, const UID & u) {
            os << "{s=" << u._size << ",u=";
            os << hex << u._uid_sak[0];
            for(unsigned int i = 1; i < u._size; i++)
                os << "," << hex << u._uid_sak[i];
            os << ",sa=" << u._uid_sak[u._size] << "}";
            return os;
        }

    private:
        Reg8 _size;
        Reg8 _uid_sak[SIZE_MAX + 1];
    }__attribute__((packed));

    class Key
    {
    public:
        static const unsigned int KEY_SIZE = 6;

        enum Key_Type {
            TYPE_A,
            TYPE_B,
        };

        Key() {}
        Key(const void * key, Key_Type type) : _type(type) {
            memcpy(_key, key, KEY_SIZE);
        }

        static unsigned int size() { return KEY_SIZE; }
        const Reg8 * key() const { return _key; }
        const Key_Type type() const { return _type; }

        void key(const void * k) { memcpy(_key, k, KEY_SIZE); }

        friend Debug & operator<<(Debug & db, const Key & k) {
            db << "{t=" << (k._type == TYPE_A ? 'A' : 'B') << ",k=[";
            db << hex << k._key[0];
            for(unsigned int i = 1; i < KEY_SIZE; i++)
                db << "," << hex << k._key[i];
            db << "]}";
            return db;
        }

        friend OStream & operator<<(OStream & os, const Key & k) {
            os << "{t=" << (k._type == TYPE_A ? 'A' : 'B') << ",k=[";
            os << hex << k._key[0];
            for(unsigned int i = 1; i < KEY_SIZE; i++)
                os << "," << hex << k._key[i];
            os << "]}";
            return os;
        }

    private:
        Key_Type _type;
        Reg8 _key[KEY_SIZE];
    }__attribute__((packed));
};

__END_SYS

#endif
