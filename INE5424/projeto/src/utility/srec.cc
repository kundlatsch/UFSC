// EPOS SREC Utility Implementation

#include <utility/srec.h>
#include <utility/string.h>
#include <architecture/cpu.h>

__BEGIN_UTIL

void * SREC::entry()
{
    srec_t srec;
    int pos;

    void * entry = 0;
    char * s_image = reinterpret_cast<char *>(this);
    for(pos = 0; s_image[pos] == 'S'; pos++) {
        if((s_image[pos + 1] == '7') ||
            (s_image[pos + 1] == '8') ||
            (s_image[pos + 1] == '9'))  {
            srec_decode(&srec, &s_image[pos]);
            entry = CPU::Log_Addr(srec.addr);
        }
        while(s_image[pos + 1] != 'S')
            pos++;
    }
    return entry;
}


int SREC::segment_size(int i)
{
    int seg_size = 0;
    int pos = 0;
    srec_t srec;

    char* s_image = reinterpret_cast<char *>(this);
    if(i > segments())
        return -1;

    for(pos = 0; s_image[pos] == 'S'; pos++) {
        if((s_image[pos + 1] == '7') ||
            (s_image[pos + 1] == '8') ||
            (s_image[pos + 1] == '9'))
            break;

        switch(s_image[pos+1]) {
        case '0': break;
        case '5': break;
        case '1': case '2': case '3':
            if(srec_decode(&srec,&s_image[pos]))
                seg_size += srec.count;
            else
                return -1;
            break;

        default:
            return -1;
            break;
        }

        while (s_image[pos + 1] != 'S') pos++;
    }

    return seg_size;
}

int SREC::load_segment(int i, void * addr)
{
    int seg_size = 0;
    int pos = 0;
    srec_t srec;
    void * dst;

    char * s_image = reinterpret_cast<char *>(this);
    if (i > segments())
        return -1;

    for(pos = 0; s_image[pos] == 'S'; pos++) {
        if((s_image[pos + 1] == '7') ||
            (s_image[pos + 1] == '8') ||
            (s_image[pos + 1] == '9'))
            break;

        switch (s_image[pos+1]) {
        case '0': break;
        case '5': break;
        case '1': case '2': case '3':
            if(!(srec_decode(&srec, &s_image[pos])))
                return(-1);
            else {
                dst = addr ? CPU::Log_Addr(addr) + seg_size : CPU::Log_Addr(srec.addr);
                seg_size += srec.count;
                memcpy(dst,srec.data,srec.count);
            }
            break;

        default:
            return(-1);
            break;
        }

        while (s_image[pos + 1] != 'S') pos++;
    }

    return(seg_size);
};


bool SREC::srec_decode(srec_t *srec, char *_line)
{
    int len, pos = 0, count, alen, sum = 0;
    unsigned char *line = (unsigned char *)_line;

    for (len = 0; line[len]; len++)
        if (line[len] == '\n' || line[len] == '\r')
            break;

    if ((srec->type = C1(line, 1)) > 9)
        return (false);

    count = C2(line, 2);
    alen = ltab[srec->type];
    sum += count;
    len -= 4;
    line += 4;

    srec->addr = 0;
    for (pos = 0; pos < alen; pos += 2) {
        unsigned char value = C2(line, pos);
        srec->addr = (srec->addr << 8) | value;
        sum += value;
    }

    len -= alen;
    line += alen;
    for (pos = 0; pos < len - 2; pos += 2) {
        unsigned char value = C2(line, pos);
        srec->data[pos / 2] = value;
        sum += value;
    }

    srec->count = count - (alen / 2) - 1;
    sum += C2(line, pos);
    if ((sum & 0xff) != 0xff)
        return(false);

    return(true);
};


signed char SREC::ctab[] = {
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            0, 1, 2, 3, 4, 5, 6, 7,   8, 9,-1,-1,-1,-1,-1,-1,
                            -1,10,11,12,13,14,15,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,10,11,12,13,14,15,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
                            -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,
};

int SREC::ltab[] = {4,4,6,8,0,4,0,8,6,4};


__END_UTIL
