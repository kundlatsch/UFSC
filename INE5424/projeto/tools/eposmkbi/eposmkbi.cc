/*=======================================================================*/
/* MKBI.C                                                                */
/*                                                                       */
/* Desc: Tool to generate an EPOS bootable image.                        */
/*                                                                       */
/* Parm: <boot image> <os image> <app1> <app2> ...                       */
/*                                                                       */
/* Auth: Guto - Changed By Fauze                                         */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#include <system/info.h>

// CONSTANTS
static const unsigned int MAX_SI_LEN = 512;
static const char CFG_FILE[] = "etc/eposmkbi.conf";

// TYPES

// Target Machine Description
struct Configuration
{
    char           mode[16];
    char           arch[16];
    char           mach[16];
    char           mmod[16];
    unsigned short n_cpus;
    unsigned int   clock;
    unsigned char  word_size;
    bool           endianess;  // true => little, false => big
    unsigned int   mem_base;
    unsigned int   mem_top;
    unsigned int   mio_base;
    unsigned int   mio_top;
    short          node_id;   // nodes in SAN (-1 => dynamic)
    int            space_x;   // Spatial coordinates of a node (-1 => mobile)
    int            space_y;
    int            space_z;
    unsigned char  uuid[8];   // EPOS image Universally Unique Identifier
    bool           need_boot;
    bool           need_si;
    bool           si_in_setup;
    unsigned int   boot_length_min;
    unsigned int   boot_length_max;
};

// System_Info
typedef _SYS::System_Info System_Info;

// PROTOTYPES
bool parse_config(FILE * cfg_file, Configuration * cfg);
void strtolower (char * dst,const char * src);
bool add_machine_secrets(int fd_img, unsigned int i_size, char * mach, char * mmod);

bool file_exist(char *file);

int put_buf(int fd_out, void * buf, int size);
int put_file(int fd_out, char * file);
int pad(int fd_out, int size);
bool lil_endian();

template<typename T> void invert(T &n);
template<typename T> int put_number(int fd, T num);
template<typename T> bool add_boot_map(int fd_out, System_Info * si);

// GLOBALS
FILE * out;
FILE * err;
Configuration CONFIG;

//=============================================================================
// MAIN
//=============================================================================
int main(int argc, char **argv)
{
    // Defult Space
    CONFIG.space_x = -1; // dynamic
    CONFIG.space_y = -1; // dynamic
    CONFIG.space_z = -1; // dynamic

    // Default Output
    out = stdout;
    err = stderr;

    // Parse options and arguments
    bool error = false;
    if(argc < 3)
        error = true;

    int opt;
    while((opt = getopt(argc, argv, "sx:y:z:")) != -1) {
        switch(opt) {
        case 's': {
            FILE * nul = fopen("/dev/null", "w");
            if(!nul) {
                fprintf(stderr, "Error: can't open /dev/null for -s!\n");
                return 1;
            }
            out = nul;
            err = nul;
        } break;
        case 'x':
            CONFIG.space_x = atoi(optarg);
            break;
        case 'y':
            CONFIG.space_y = atoi(optarg);
            break;
        case 'z':
            CONFIG.space_z = atoi(optarg);
            break;
        default: /* '?' */
            error = true;
        }
    }
    if(optind >= argc)
        error = true;

    if(error) {
        fprintf(err, "Usage: %s [-s] [-x X] [-y Y] [-z Z] <EPOS root> <boot image> <app1> <app2> ...\n", argv[0]);
        return 1;
    }

    // Say hello
    fprintf(out, "\nEPOS bootable image tool\n\n");

    // Read configuration
    char file[256];
    sprintf(file, "%s/%s", argv[optind], CFG_FILE);
    FILE * cfg_file = fopen(file, "rb");
    if(!cfg_file) {
        fprintf(err, "Error: can't read configuration file \"%s\"!\n", file);
        return 1;
    }
    if(!parse_config(cfg_file, &CONFIG)) {
        fprintf(err, "Error: invalid configuration file \"%s\"!\n", file);
        return 1;
    }

    // Open destination file (rewrite)
    int fd_img = open(argv[optind + 1], O_RDWR | O_CREAT | O_TRUNC, 00644);
    if(fd_img < 0) {
        fprintf(err, "Error: can't create boot image \"%s\"!\n", argv[optind + 1]);
        return 1;
    }

    // Show configuration
    fprintf(out, "  EPOS mode: %s\n", CONFIG.mode);
    fprintf(out, "  Machine: %s\n", CONFIG.mach);
    fprintf(out, "  Model: %s\n", CONFIG.mmod);
    fprintf(out, "  Processor: %s (%d bits, %s-endian)\n", CONFIG.arch, CONFIG.word_size, CONFIG.endianess ? "little" : "big");
    fprintf(out, "  Memory: %d KBytes\n", (CONFIG.mem_top - CONFIG.mem_base) / 1024);
    if(CONFIG.space_x != -1)
        fprintf(out, "  Node location: (%d, %d, %d)\n", CONFIG.space_x, CONFIG.space_y, CONFIG.space_z);
    fprintf(out, "  UUID: ");
    for(unsigned int i = 0; i < 8; i++)
        fprintf(out, "%.2x", CONFIG.uuid[i]);

    // Initialize the Boot_Map in System_Info
    System_Info si;
    si.bm.n_cpus   = CONFIG.n_cpus;     // can be adjusted by SETUP in some machines
    si.bm.mem_base = CONFIG.mem_base;
    si.bm.mem_top  = CONFIG.mem_top;
    si.bm.mio_base = CONFIG.mio_base;   // can be adjusted by SETUP in some machines
    si.bm.mio_top  = CONFIG.mio_top;    // can be adjusted by SETUP in some machines
    si.bm.node_id  = CONFIG.node_id;
    si.bm.space_x  = CONFIG.space_x;
    si.bm.space_y  = CONFIG.space_y;
    si.bm.space_z  = CONFIG.space_z;
    for(unsigned int i = 0; i < 8; i++)
        si.bm.uuid[i]  = CONFIG.uuid[i];


    // Create the boot image
    unsigned int image_size = 0;
    fprintf(out, "\n  Creating EPOS bootable image in \"%s\":\n", argv[optind + 1]);

    // Add BOOT
    if(CONFIG.need_boot) {
        sprintf(file, "%s/img/boot_%s", argv[optind], CONFIG.mmod);
        fprintf(out, "    Adding bootstrap \"%s\":", file);
        image_size += put_file(fd_img, file);
        if(image_size > CONFIG.boot_length_max) {
            fprintf(out, " failed!\n");
            fprintf(err, "Boot strap \"%s\" is too large! (%d bytes)\n", file, image_size);
            return 1;
        } else {
            if(image_size < CONFIG.boot_length_min)
                image_size += pad(fd_img, CONFIG.boot_length_min - image_size);
        }
    }
    unsigned int boot_size = image_size;

    // Reserve space for System_Info if necessary
    if(CONFIG.need_si && !CONFIG.si_in_setup) {
        if(sizeof(System_Info) <= MAX_SI_LEN) {
            image_size += pad(fd_img, MAX_SI_LEN);
        } else {
            fprintf(out, " failed!\n");
            fprintf(err, "System_Info structure is too large (%ld)!\n", static_cast<unsigned long>(sizeof(System_Info)));
            return 1;
        }
    }

    // Add SETUP
    sprintf(file, "%s/img/setup_%s", argv[optind], CONFIG.mmod);
    if(file_exist(file)) {
        si.bm.setup_offset = image_size - boot_size;
        fprintf(out, "    Adding setup \"%s\":", file);
        image_size += put_file(fd_img, file);
    } else
        si.bm.setup_offset = -1;

    // Add INIT and OS (for mode != library only)
    if(!strcmp(CONFIG.mode, "library")) {
        si.bm.init_offset = -1;
        si.bm.system_offset = -1;
    } else {
        // Add INIT
        si.bm.init_offset = image_size - boot_size;
        sprintf(file, "%s/img/init_%s", argv[optind], CONFIG.mmod);
        fprintf(out, "    Adding init \"%s\":", file);
        image_size += put_file(fd_img, file);

        // Add SYSTEM
        si.bm.system_offset = image_size - boot_size;
        sprintf(file, "%s/img/system_%s", argv[optind], CONFIG.mmod);
        fprintf(out, "    Adding system \"%s\":", file);
        image_size += put_file(fd_img, file);
    }

    // Add application(s) and data
    si.bm.application_offset = image_size - boot_size;
    fprintf(out, "    Adding application \"%s\":", argv[optind + 2]);
    image_size += put_file(fd_img, argv[optind + 2]);
    if((argc - optind) == 3) // single APP
        si.bm.extras_offset = -1;
    else { // multiple APPs or data
        si.bm.extras_offset = image_size - boot_size;
        struct stat file_stat;
        for(int i = optind + 3; i < argc; i++) {
            fprintf(out, "    Adding file \"%s\":", argv[i]);
            stat(argv[i], &file_stat);
            image_size += put_number(fd_img, file_stat.st_size);
            image_size += put_file(fd_img, argv[i]);
        }
        // Signalize last application by setting its size to 0
        image_size += put_number(fd_img, 0);
    }

    // Add the size of the image to the Boot_Map in System_Info (excluding BOOT)
    si.bm.img_size = image_size - boot_size;

    // Add System_Info
    if(CONFIG.need_si) {
        unsigned int si_offset = boot_size;
        fprintf(out, "    Adding system info");
        if(CONFIG.si_in_setup) {
            fprintf(out, " to SETUP:");
            struct stat stat;
            if(fstat(fd_img, &stat) < 0)  {
                fprintf(out, " failed! (stat)\n");
                return 0;
            }
            char * buffer = (char *) malloc(stat.st_size);
            if(!buffer) {
                fprintf(out, " failed! (malloc)\n");
                return 0;
            }
            memset(buffer, '\1', stat.st_size);
            lseek(fd_img, 0, SEEK_SET);
            if(read(fd_img, buffer, stat.st_size) < 0) {
                fprintf(out, " failed! (read)\n");
                free(buffer);
                return 0;
            }

            char placeholder[] = "<System_Info placeholder>";
            char * setup_si = reinterpret_cast<char *>(memmem(buffer, stat.st_size, placeholder, strlen(placeholder)));
            if(setup_si) {
                si_offset = setup_si - buffer;
            } else {
                fprintf(out, " failed! (SETUP does not contain System_Info placeholder)\n");
                free(buffer);
                return 0;
            }
        } else {
            fprintf(out, " to image:");
            si_offset = boot_size;
        }
        if(lseek(fd_img, si_offset, SEEK_SET) < 0) {
            fprintf(err, "Error: can't seek the boot image!\n");
            return 1;
        }
        switch(CONFIG.word_size) {
        case  8: if(!add_boot_map<char>(fd_img, &si)) return 1; break;
        case 16: if(!add_boot_map<short>(fd_img, &si)) return 1; break;
        case 32: if(!add_boot_map<long>(fd_img, &si)) return 1; break;
        case 64: if(!add_boot_map<long long>(fd_img, &si)) return 1; break;
        default: return 1;
        }
        fprintf(out, " done.\n");
    }

    // Adding MACH specificities
    fprintf(out, "\n  Adding specific boot features of \"%s\":", CONFIG.mmod);
    if(!(add_machine_secrets(fd_img, image_size, CONFIG.mach, CONFIG.mmod))) {
        fprintf(err, "Error: specific features error!\n");
        return 1;
    }
    fprintf(out, " done.\n");

    // Finish
    close(fd_img);
    fprintf(out, "\n  Image successfully generated (%d bytes)!\n\n", image_size);

    return 0;
}

//=============================================================================
// PARSE_CONFIG
//=============================================================================
bool parse_config(FILE * cfg_file, Configuration * cfg)
{
    char line[256];
    char * token;

    // EPOS Mode
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read SMOD from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "SMOD") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid SMOD in configuration!\n");
        return false;
    }
    strtolower(cfg->mode, token);

    // Arch
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read ARCH from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "ARCH") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid ARCH in configuration!\n");
        return false;
    }
    strtolower(cfg->arch, token);

    // Machine
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read MACH from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "MACH") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid MACH in configuration!\n");
        return false;
    }
    strtolower(cfg->mach, token);

    // Model
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read MMOD from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "MMOD") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid MMOD in configuration!\n");
        return false;
    }
    strtolower(cfg->mmod, token);

    // CPUS
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read CPUS from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "CPUS") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid CPUS in configuration!\n");
        return false;
    }
    cfg->n_cpus = atoi(token);

    // Clock
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read CLOCK from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "CLOCK") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid CLOCK in configuration!\n");
        return false;
    }
    cfg->clock = atoi(token);

    // Word Size
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read WORD_SIZE from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "WORD_SIZE") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid WORD_SIZE in configuration!\n");
        return false;
    }
    cfg->word_size = atoi(token);

    // Endianess
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read ENDIANESS from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "ENDIANESS") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid ENDIANESS in configuration!\n");
        return false;
    }
    cfg->endianess = !strcmp(token, "little");

    // Memory Base
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read RAM_BASE from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "RAM_BASE") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid RAM_BASE in configuration!\n");
        return false;
    }
    cfg->mem_base = strtoll(token, 0, 16);

    // Memory Top
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read RAM_TOP from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "RAM_TOP") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid RAM_TOP in configuration!\n");
        return false;
    }
    cfg->mem_top = strtoll(token, 0, 16);

    // I/O Base
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read MIO_BASE from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "MIO_BASE") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid MIO_BASE in configuration!\n");
        return false;
    }
    cfg->mio_base = strtoll(token, 0, 16);

    // I/O Top
    if(fgets(line, 256, cfg_file) != line) {
        fprintf(err, "Error: failed to read MIO_TOP from configuration file!\n");
        return false;
    }
    token = strtok(line, "=");
    if(strcmp(token, "MIO_TOP") || !(token = strtok(NULL, "\n"))) {
        fprintf(err, "Error: no valid MIO_TOP in configuration!\n");
        return false;
    }
    cfg->mio_top = strtoll(token, 0, 16);

    // Node Id
    if(fgets(line, 256, cfg_file) != line)
        cfg->node_id = -1; // get from net
    else {
        token = strtok(line, "=");
        if(!strcmp(token, "NODE_ID") && (token = strtok(NULL, "\n")))
            cfg->node_id = atoi(token);
        else
            cfg->node_id = -1; // get from net
    }

    // UUID
    if(fgets(line, 256, cfg_file) == line) {
        token = strtok(line, "=");
        if(!strcmp(token, "UUID") && (token = strtok(NULL, "\n"))) {
            unsigned int buf[16];
            unsigned int i, j;
            for(i = j = 0; (i < 16) && (sscanf(&token[j], "%2x", &buf[i]) == 1); i++, j+=2);
            for(i = j = 0; i < sizeof(_SYS::UUID); i++, j+=2)
                cfg->uuid[i] = buf[j] ^ buf[j+1];
        }
    }

    // Determine if BOOT is needed and how it must be handled
    if(!strcmp(cfg->mach, "pc")) {
        cfg->need_boot = true;
        cfg->boot_length_min = 512;
        cfg->boot_length_max = 512;
    } else {
        cfg->need_boot = false;
        cfg->boot_length_min = 0;
        cfg->boot_length_max = 0;
    }

    // Determine if System_Info is needed and how it must be handled
    cfg->need_si = (!strcmp(cfg->mach, "pc") || !strcmp(cfg->mach, "riscv") || !strcmp(cfg->mach, "cortex"));
    cfg->si_in_setup = cfg->need_si && !cfg->need_boot; // If the image contains a boot sector, then SI will be on a separate disk sector. Otherwise, it will be inside SETUP.

    return true;
}

//=============================================================================
// ADD_BOOT_MAP
//=============================================================================
template<typename T> bool add_boot_map(int fd, System_Info * si)
{
    if(!put_number(fd, static_cast<T>(si->bm.n_cpus)))
        return false;
    if(!put_number(fd, static_cast<T>(si->bm.mem_base)))
        return false;
    if(!put_number(fd, static_cast<T>(si->bm.mem_top)))
        return false;

    if(!put_number(fd, static_cast<T>(si->bm.mio_base)))
        return false;
    if(!put_number(fd, static_cast<T>(si->bm.mio_top)))
        return false;

    if(!put_number(fd, si->bm.node_id))
        return false;
    if(!put_number(fd, si->bm.space_x))
        return false;
    if(!put_number(fd, si->bm.space_y))
        return false;
    if(!put_number(fd, si->bm.space_z))
        return false;
    for(unsigned int i = 0; i < 8; i++)
        if(!put_number(fd, si->bm.uuid[i]))
            return false;

    if(!put_number(fd, static_cast<T>(si->bm.img_size)))
        return false;
    if(!put_number(fd, static_cast<T>(si->bm.setup_offset)))
        return false;
    if(!put_number(fd, static_cast<T>(si->bm.init_offset)))
        return false;
    if(!put_number(fd, static_cast<T>(si->bm.system_offset)))
        return false;
    if(!put_number(fd, static_cast<T>(si->bm.application_offset)))
        return false;
    if(!put_number(fd, static_cast<T>(si->bm.extras_offset)))
        return false;

    return true;
}

//=============================================================================
// ADD_MACHINE_SCRETS
//=============================================================================
bool add_machine_secrets(int fd, unsigned int i_size, char * mach, char * mmod)
{
    if(!strcmp(mach, "pc")) { // PC
        const unsigned int floppy_size   = 1474560;
        const unsigned int secrets_offset   = 512 - 6;
        const unsigned short boot_id        = 0xaa55;
        const unsigned short num_sect       = ((i_size + 511) / 512);
        const unsigned short last_track_sec = num_sect <= 2880 ? 19 : 49; // either 144 tracks with 20 sectors or 144 tracks with 50 sectors

        // Pad the image to the size of a standard floppy
        if(lseek(fd, 0, SEEK_END) < 0) {
            fprintf(err, "Error: can't seek the boot image!\n");
            return false;
        }
        pad(fd, (floppy_size  - i_size));

        // Write the number of sectors to be read
        if(lseek(fd, secrets_offset, SEEK_SET) < 0) {
            fprintf(err, "Error: can't seek the boot image!\n");
            return false;
        }
        put_number(fd, last_track_sec);
        put_number(fd, num_sect);
        put_number(fd, boot_id);
    } else if (!strcmp(mach, "rcx")) { // RCX
        char key_string[] = "Do you byte, when I knock?";
        const unsigned short key_offset = 128 - (strlen(key_string) + 1);

        // Write key string to unlock epos
        if(lseek(fd,key_offset,SEEK_SET) < 0) {
            fprintf(err, "Error: can't seek the boot image!\n");
            return false;
        }
        put_buf(fd, key_string, (strlen(key_string)+1));
    }
    else if (!strcmp(mmod, "emote3")) { // EPOSMoteIII
        // Customer Configuration Area (CCA)
        //char key_string[] = ":020000040027D3\r\n:0CFFD400FFFFFFF700000000000020000D\r\n:00000001FF\r\n"; // Bootloader Enabled, enter by setting pin PA7 to low
        //char key_string[] = ":020000040027D3\r\n:0CFFD400FFFFFFFF000000000000200005\r\n:00000001FF\r\n"; // Bootloader Enabled, enter by setting pin PA7 to high
        char key_string[] = ":020000040027D3\r\n:0CFFD400FFFFFFEF000000000000200015\r\n:00000001FF\r\n"; // Bootloader Disabled
        const long key_offset = -strlen(":00000001FF\r\n");

        // Write key string to unlock EPOS
        if(lseek(fd,key_offset,SEEK_END) < 0) {
            fprintf(err, "Error: can't seek the boot image!\n");
            return false;
        }
        put_buf(fd, key_string, strlen(key_string));
    }

    return true;
}

//=============================================================================
// FILE_EXIST
//=============================================================================
bool file_exist(char *file)
{
    int fd_in;
    struct stat stat;

    fd_in = open(file, O_RDONLY);
    if(fd_in < 0)
        return false;

    if(fstat(fd_in, &stat) < 0)
        return false;

    return true;
}

//=============================================================================
// PUT_FILE
//=============================================================================
int put_file(int fd_out, char * file)
{
    int fd_in;
    struct stat stat;
    char * buffer;

    fd_in = open(file, O_RDONLY);
    if(fd_in < 0) {
        fprintf(out, " failed! (open)\n");
        return 0;
    }

    if(fstat(fd_in, &stat) < 0)  {
        fprintf(out, " failed! (stat)\n");
        return 0;
    }

    buffer = (char *) malloc(stat.st_size);
    if(!buffer) {
        fprintf(out, " failed! (malloc)\n");
        return 0;
    }
    memset(buffer, '\1', stat.st_size);

    if(read(fd_in, buffer, stat.st_size) < 0) {
        fprintf(out, " failed! (read)\n");
        free(buffer);
        return 0;
    }

    if(write(fd_out, buffer, stat.st_size) < 0) {
        fprintf(out, " failed! (write)\n");
        free(buffer);
        return 0;
    }

    free(buffer);
    close(fd_in);

    fprintf(out, " done.\n");

    return stat.st_size;
}

//=============================================================================
// PUT_BUF
//=============================================================================
int put_buf(int fd, void * buf, int size)
{
    if(!size)
        return 0;
    int written = write(fd, buf, size);
    if(written < 0) {
        fprintf(err, "Error: can't write to file!\n");
        written = 0;
    }
    return written;
}

//=============================================================================
// PUT_NUMBER
//=============================================================================
template<typename T> int put_number(int fd, T num)
{
    if((CONFIG.endianess != lil_endian()) && (sizeof(T) > 1))
        invert(num);
    if(write(fd, &num, sizeof(T)) < 0) {
        fprintf(err, "Error: can't write to file!\n");
        return 0;
    }
    return sizeof(T);
}

//=============================================================================
// PAD
//=============================================================================
int pad(int fd, int size)
{
    if(!size)
        return 0;

    char * buffer = (char *) malloc(size);
    if(!buffer) {
        fprintf(err, "Error: not enough memory!\n");
        return 0;
    }

    memset(buffer, '\1', size);
    if(write(fd, buffer, size) < 0) {
        fprintf(err, "Error: can't write to the boot image!\n");
        return 0;
    }

    free(buffer);
    return size;
}

//=============================================================================
// STRTOLOWER
//=============================================================================
void strtolower(char* dst, const char* src) {
    int i = 0;
    strcpy(dst,src);
    while(src[i] != '\0') {
        dst[i] = tolower(dst[i]);
        i++;
    }
}

//=============================================================================
// LIL_ENDIAN
//=============================================================================
bool lil_endian() {
    int test = 1;
    return (*((char*)&test)) ? true : false;
}

//=============================================================================
// INVERT
//=============================================================================
template<typename T> void invert(T & n)
{
    for(int i = 0, j = sizeof(T) - 1; i < (int)sizeof(T) / 2; i++, j--) {
        char * h = &(((char *)&n)[i]);
        char * l = &(((char *)&n)[j]);
        *h ^= *l;
        *l ^= *h;
        *h ^= *l;
    }
}

