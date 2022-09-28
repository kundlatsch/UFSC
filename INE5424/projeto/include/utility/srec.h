// EPOS SREC Utility Declarations

#ifndef __srec_h
#define	__srec_h 1

#include <system/config.h>

__BEGIN_UTIL

class SREC
{
public:
    SREC() {}

    bool valid() {
      return ((reinterpret_cast<char *>(this)[0] == 'S') &&
              (reinterpret_cast<char *>(this)[1] == '0'));
    }

    void * entry();

    int segments() {
      return 1;
    }

    void * segment_address(int i) {
      return reinterpret_cast<void *>(this->entry());
    }

    int segment_size(int i);
    int load_segment(int i, void * addr = 0);

private:

    static signed char ctab[];
    static int ltab[];

    struct srec_t {
      unsigned char type;
      unsigned int  addr;
      unsigned char count;
      unsigned char data[256];
    };

    inline int C1(unsigned char* l, unsigned char p) {
      return ctab[l[p]];
    };

    inline int C2(unsigned char* l, unsigned char p) {
      return (C1(l,p) << 4) | (C1(l, p+1));
    };

    bool srec_decode(srec_t *srec, char *_line);

};

__END_UTIL

#endif

