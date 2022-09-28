# EPOS Init Makefile

include ../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc))

all:	install

install: install_$(SMOD)_$(MMOD)

install_library_$(MMOD): init_library_$(MMOD)
		$(INSTALL) init_begin_$(MMOD).o $(LIB)
		$(INSTALL) init_system_$(MMOD).o $(LIB)
		$(INSTALL) init_application_$(MMOD).o $(LIB)
		$(INSTALL) init_end_$(MMOD).o $(LIB)

install_builtin_$(MMOD): init_builtin_$(MMOD)
		$(INSTALL) init_$(MMOD) $(IMG)
		$(INSTALL) init_application_$(MMOD).o $(LIB)

install_kernel_$(MMOD): init_kernel_$(MMOD)
		$(INSTALL) init_$(MMOD) $(IMG)
		$(INSTALL) init_application_$(MMOD).o $(LIB)


init_library_$(MMOD): $(OBJS)
		$(INSTALL) init_begin.o init_begin_$(MMOD).o
		$(INSTALL) init_system.o init_system_$(MMOD).o
		$(INSTALL) init_application.o init_application_$(MMOD).o
		$(INSTALL) init_end.o init_end_$(MMOD).o

init_builtin_$(MMOD): $(SRC)/system/system_$(MMOD) $(OBJS) $(LIBINIT)
		$(LD) $(LDFLAGS) -L$(CCLIB) --omagic \
		--section-start $(MACH_CODE_NAME)=$(INIT_ADDR) \
		--entry=_init -o init_$(MMOD) \
		$(LIB)/crtbegin_$(MMOD).o \
		init_end.o init_system.o init_begin.o \
		$(LIB)/crtend_$(MMOD).o \
		-l$(LINIT) \
		-R $(SRC)/system/system_$(MMOD) \
		-l$(LUTIL) -lgcc
		$(INSTALL) init_application.o init_application_$(MMOD).o

init_kernel_$(MMOD): $(SRC)/system/system_$(MMOD) $(OBJS) $(LIBINIT)
		$(LD) $(LDFLAGS) -L$(CCLIB) --omagic \
		--section-start $(MACH_CODE_NAME)=$(INIT_ADDR) \
		--entry=_init -o init_$(MMOD) \
		$(LIB)/crtbegin_$(MMOD).o \
		init_end.o init_system.o init_begin.o \
		$(LIB)/crtend_$(MMOD).o \
		-l$(LINIT) \
		-R $(SRC)/system/system_$(MMOD) \
		-l$(LUTIL) -lgcc
		$(INSTALL) init_application.o init_application_$(MMOD).o


clean:
		$(CLEAN) *.o $(shell find -maxdepth 1 -executable -not -type d)

FORCE:
