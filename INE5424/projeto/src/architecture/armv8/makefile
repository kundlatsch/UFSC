# EPOS ARMv8 Architecture Makefile

include ../../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc | grep -v _init | grep -v _test))
CRTS := $(subst .S,.o,$(shell find *.S | grep crt)) $(ARCH)_crtbegin.o $(ARCH)_crtend.o 
CRTSI := $(subst .S,.s,$(shell find *.S | grep crt))
INITS := $(subst .cc,.o,$(shell find *.cc | grep _init))

all:		crts $(LIBARCH) $(LIBINIT)

crts:		$(CRTS)
		$(INSTALL) $(ARCH)_crt0.o $(LIB)/crt0_$(MMOD).o
		$(INSTALL) $(ARCH)_crtbegin.o $(LIB)/crtbegin_$(MMOD).o
		$(INSTALL) $(ARCH)_crtend.o $(LIB)/crtend_$(MMOD).o

.INTERMEDIATE:	$(CRTSI)

$(LIBARCH):	$(LIBARCH)($(OBJS))

$(LIBINIT):	$(LIBINIT)($(INITS))

$(ARCH)_crtbegin.o: ../common/crtbegin.c
		$(CC) $(CCFLAGS) $< -o $@

$(ARCH)_crtend.o: ../common/crtend.c
		$(CC) $(CCFLAGS) $< -o $@

clean:
		$(CLEAN) *.o *.s *_test
