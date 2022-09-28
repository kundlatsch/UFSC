// EPOS String Utility Implementation (from Newlib)

#include <system/config.h>
#include <utility/string.h>

extern "C"
{

    int memcmp(const void * m1, const void * m2, size_t n) __attribute__ ((weak));
    void * memcpy(void * d, const void * s, size_t n) __attribute__ ((weak));
    void * memset(void * m, int c, size_t n) __attribute__ ((weak));
    void * memchr(const void * m, int c, size_t n) __attribute__ ((weak));
    int strcmp(const char * s1, const char * s2) __attribute__ ((weak));
    int strncmp(const char * s1, const char * s2, size_t n) __attribute__ ((weak));
    char * strcpy(char *d, const char *s) __attribute__ ((weak));
    char * strncpy(char *d, const char *s, size_t n) __attribute__ ((weak));
    char * strcat(char *d, const char *s) __attribute__ ((weak));
    char * strchr(const char * s, int c) __attribute__ ((weak));
    char * strrchr (const char * s, int c) __attribute__ ((weak));
    size_t strlen(const char * s) __attribute__ ((weak));
    long atol(const char * s) __attribute__ ((weak));
    char *itoa(int value, char *str) __attribute__ ((weak));
    int utoa(unsigned long v,char * dst) __attribute__((weak));

    int memcmp(const void * m1, const void * m2, size_t n)
    {
        unsigned char *s1 = (unsigned char *) m1;
        unsigned char *s2 = (unsigned char *) m2;
        unsigned long *a1;
        unsigned long *a2;

        if(!((n) < (sizeof(long))) && !(((long) s1 & (sizeof(long) - 1))
            | ((long) s2 & (sizeof(long) - 1)))) {

            a1 = (unsigned long*) s1;
            a2 = (unsigned long*) s2;
            while(n >= (sizeof(long))) {
                if(*a1 != *a2)
                    break;
                a1++;
                a2++;
                n -= (sizeof(long));
            }

            s1 = (unsigned char*) a1;
            s2 = (unsigned char*) a2;
        }

        while(n--) {
            if(*s1 != *s2)
                return *s1 - *s2;
            s1++;
            s2++;
        }

        return 0;

    }

    void * memcpy(void * dst0, const void * src0, size_t len0)
    {
        char *dst = reinterpret_cast<char *> (dst0);
        const char *src = reinterpret_cast<const char *> (src0);
        long *aligned_dst;
        const long *aligned_src;
        size_t len = len0;

        if(!((len) < (sizeof(long) << 2)) && !(((long) src & (sizeof(long) - 1))
            | ((long) dst & (sizeof(long) - 1)))) {
            aligned_dst = (long*) dst;
            aligned_src = (long*) src;

            while(len >= (sizeof(long) << 2)) {
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                len -= (sizeof(long) << 2);
            }

            while(len >= (sizeof(long))) {
                *aligned_dst++ = *aligned_src++;
                len -= (sizeof(long));
            }

            dst = (char*) aligned_dst;
            src = (char*) aligned_src;
        }

        while(len--)
            *dst++ = *src++;

        return dst0;

    }

    void * memchr(const void * src_void, int c, size_t length)
    {
        const unsigned char *src = (const unsigned char *) src_void;
        unsigned char d = c;

        unsigned long *asrc;
        unsigned long mask;
        size_t i;

        while(((long) src & (sizeof(long) - 1))) {
            if(!length--)
                return 0;
            if(*src == d)
                return (void *) src;
            src++;
        }

        if(!((length) < (sizeof(long)))) {
            asrc = (unsigned long *) src;
            mask = d << 8 | d;
            mask = mask << 16 | mask;
            for(i = 32; i < (sizeof(long)) * 8; i <<= 1)
                mask = (mask << i) | mask;

            while(length >= (sizeof(long))) {
                if(((((*asrc ^ mask) - 0x01010101) & ~(*asrc ^ mask) & 0x80808080)))
                    break;
                length -= (sizeof(long));
                asrc++;
            }

            src = (unsigned char *) asrc;
        }

        while(length--) {
            if(*src == d)
                return (void *) src;
            src++;
        }

        return 0;
    }

    void * memset(void * m, int c, size_t n)
    {
        char *s = (char *) m;

        size_t i;
        unsigned long buffer;
        unsigned long *aligned_addr;
        unsigned int d = c & 0xff;

        while(((long) s & ((sizeof(long)) - 1))) {
            if(n--)
                *s++ = (char) c;
            else
                return m;
        }

        if(!((n) < (sizeof(long)))) {

            aligned_addr = (unsigned long *) s;

            buffer = (d << 8) | d;
            buffer |= (buffer << 16);
            for(i = 32; i < (sizeof(long)) * 8; i <<= 1)
                buffer = (buffer << i) | buffer;

            while(n >= (sizeof(long)) * 4) {
                *aligned_addr++ = buffer;
                *aligned_addr++ = buffer;
                *aligned_addr++ = buffer;
                *aligned_addr++ = buffer;
                n -= 4 * (sizeof(long));
            }

            while(n >= (sizeof(long))) {
                *aligned_addr++ = buffer;
                n -= (sizeof(long));
            }

            s = (char*) aligned_addr;
        }

        while(n--)
            *s++ = (char) c;

        return m;
    }

    int strcmp(const char * s1, const char * s2)
    {
        unsigned long *a1;
        unsigned long *a2;

        if(!(((long) s1 & (sizeof(long) - 1)) | ((long) s2 & (sizeof(long) - 1)))) {

            a1 = (unsigned long*) s1;
            a2 = (unsigned long*) s2;
            while(*a1 == *a2) {

                if((((*a1) - 0x01010101) & ~(*a1) & 0x80808080))
                    return 0;

                a1++;
                a2++;
            }

            s1 = (char*) a1;
            s2 = (char*) a2;
        }

        while(*s1 != '\0' && *s1 == *s2) {
            s1++;
            s2++;
        }
        return (*(unsigned char *) s1) - (*(unsigned char *) s2);
    }

    int strncmp(const char * s1, const char * s2, size_t n)
    {
        unsigned long *a1;
        unsigned long *a2;

        if(n == 0)
            return 0;

        if(!(((long) s1 & (sizeof(long) - 1)) | ((long) s2 & (sizeof(long) - 1)))) {

            a1 = (unsigned long*) s1;
            a2 = (unsigned long*) s2;
            while(n >= sizeof(long) && *a1 == *a2) {
                n -= sizeof(long);

                if(n == 0 || (((*a1) - 0x01010101) & ~(*a1) & 0x80808080))
                    return 0;

                a1++;
                a2++;
            }

            s1 = (char*) a1;
            s2 = (char*) a2;
        }

        while(n-- > 0 && *s1 == *s2) {

            if(n == 0 || *s1 == '\0')
                return 0;
            s1++;
            s2++;
        }
        return (*(unsigned char *) s1) - (*(unsigned char *) s2);
    }

    char * strcpy(char *dst0, const char *src0)
    {
        char *dst = dst0;
        const char *src = src0;
        long *aligned_dst;
        const long *aligned_src;

        if(!(((long) src & (sizeof(long) - 1)) | ((long) dst & (sizeof(long) - 1)))) {
            aligned_dst = (long*) dst;
            aligned_src = (long*) src;

            while(!(((*aligned_src) - 0x01010101) & ~(*aligned_src) & 0x80808080)) {
                *aligned_dst++ = *aligned_src++;
            }

            dst = (char*) aligned_dst;
            src = (char*) aligned_src;
        }

        while((*dst++ = *src++))
            ;
        return dst0;

    }

    char * strncpy(char *dst0, const char *src0, size_t count)
    {
        char *dst = dst0;
        const char *src = src0;
        long *aligned_dst;
        const long *aligned_src;

        if(!(((long) src & (sizeof(long) - 1)) | ((long) dst & (sizeof(long) - 1)))
            && !((count) < sizeof(long))) {
            aligned_dst = (long*) dst;
            aligned_src = (long*) src;

            while(count >= sizeof(long int) && !(((*aligned_src) - 0x01010101)
                & ~(*aligned_src) & 0x80808080)) {
                count -= sizeof(long int);
                *aligned_dst++ = *aligned_src++;
            }

            dst = (char*) aligned_dst;
            src = (char*) aligned_src;
        }

        while(count > 0) {
            --count;
            if((*dst++ = *src++) == '\0')
                break;
        }

        while(count-- > 0)
            *dst++ = '\0';

        return dst0;
    }

    char * strcat(char * dst0, const char *src0)
    {
        size_t dst_len = strlen(dst0);
        return strcpy(&dst0[dst_len], src0);
    }

    char * strchr(const char * s1, int i)
    {
        const unsigned char * s = (const unsigned char *) s1;
        unsigned char c = i;

        unsigned long mask, j;
        unsigned long * aligned_addr;

        if(!c) {
            while(((long) s & (sizeof(long) - 1))) {
                if(!*s)
                    return (char *) s;
                s++;
            }

            aligned_addr = (unsigned long *) s;
            while(!(((*aligned_addr) - 0x01010101) & ~(*aligned_addr) & 0x80808080))
                aligned_addr++;

            s = (const unsigned char *) aligned_addr;
            while(*s)
                s++;
            return (char *) s;
        }

        while(((long) s & (sizeof(long) - 1))) {
            if(!*s)
                return 0;
            if(*s == c)
                return (char *) s;
            s++;
        }

        mask = c;
        for(j = 8; j < (sizeof(long)) * 8; j <<= 1)
            mask = (mask << j) | mask;

        aligned_addr = (unsigned long *) s;
        while(!(((*aligned_addr) - 0x01010101) & ~(*aligned_addr) & 0x80808080)
            && !((((*aligned_addr ^ mask) - 0x01010101) & ~(*aligned_addr
                ^ mask) & 0x80808080)))
            aligned_addr++;

        s = (unsigned char *) aligned_addr;

        while(*s && *s != c)
            s++;
        if(*s == c)
            return (char *) s;
        return 0;
    }

    size_t strlen(const char * str)
    {
        const char *start = str;

        unsigned long *aligned_addr;

        while(((long) str & ((sizeof(long)) - 1))) {
            if(!*str)
                return str - start;
            str++;
        }

        aligned_addr = (unsigned long *) str;
        while(!(((*aligned_addr) - 0x01010101) & ~(*aligned_addr) & 0x80808080))
            aligned_addr++;

        str = (char *) aligned_addr;

        while(*str)
            str++;
        return str - start;
    }

    char * strrchr (const char *s, int c)
    {
        char *rtnval = 0;

        do {
           if (*s == c)
               rtnval = (char*) s;
        } while (*s++);
        return (rtnval);
    }

    long atol(const char *s)
    {
        long ret=0; bool neg=false;

        if (*s == '-') {
            neg = true;
            ++s;
        }

        while ((*s >= '0') && (*s <= '9')) {
           ret = ret * 10 + *s++ - '0';
        }

        return neg ? -ret : ret;
    }

    char *itoa(int value, char *str)
    {
        char *s = str;
        char buffer[11];
        char *buf = buffer;
        unsigned int v;

        if (value < 0) {
            *s++ = '-';
            v = -value;
        } else {
            if (value == 0)
                *s++ = '0';
            v = (unsigned int) value;
        }

        while (v) {
            int i = v % 10;
            v /= 10;
            *buf++ = i + '0';
        }

        while (buf > buffer)
            *s++ = *--buf;
        *s = 0;

        return str;
    }

    int utoa(unsigned long v,char * dst)
    {
        int i = 0;
        unsigned long j;
        // special case for v=0
        if (v == 0)
            dst[i++] = '0';
        for(j = v; j != 0; i++, j /= 10);
        for(j = 0; v != 0; j++, v /= 10)
            dst[i - 1 - j] = '0' + (v % 10);
        return i;
    }

    int atoi(const char *s)
    {
        int i;
        i = 0;
        while(*s) {
            i = (i << 3) + (i << 1) + (*s - '0');
            s++;
            // Dont increment i!
        }
        return i;
    }
}
