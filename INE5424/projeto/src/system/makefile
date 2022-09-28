# EPOS System Makefile

include ../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc))

all:	install

install: install_$(SMOD)_$(MMOD)

install_library_$(MMOD): system_library_$(MMOD)
		$(INSTALL) system_$(MMOD).o $(LIB)
		$(INSTALL) application_$(MMOD).o $(LIB)

install_builtin_$(MMOD): system_builtin_$(MMOD)
		$(OBJCOPY) -L _end system_$(MMOD)
		$(INSTALL) system_$(MMOD) $(IMG)
		$(INSTALL) application_$(MMOD).o $(LIB)

install_kernel_$(MMOD): system_kernel_$(MMOD)
		$(INSTALL) system_$(MMOD) $(IMG)
		$(INSTALL) application_$(MMOD).o $(LIB)

system_library_$(MMOD): system_scaffold.o application_scaffold.o
		$(LD) $(LDFLAGS) -i system_scaffold.o -o system_$(MMOD).o
		$(INSTALL) application_scaffold.o application_$(MMOD).o

system_builtin_$(MMOD): $(OBJS)
		$(LD) $(LDFLAGS) -L$(CCLIB) --nmagic \
		--section-start $(MACH_CODE_NAME)=$(SYS_CODE_ADDR) \
		--section-start $(MACH_DATA_NAME)=$(SYS_DATA_ADDR) \
		--entry=_init -o system_$(MMOD) \
		$(LIB)/crtbegin_$(MMOD).o \
		system_scaffold.o system_bindings.o\
		$(LIB)/crtend_$(MMOD).o \
		--whole-archive \
		-l$(LSYS) -l$(LMACH) -l$(LARCH) \
		--no-whole-archive \
		-l$(LUTIL) -lgcc
		$(INSTALL) application_scaffold.o application_$(MMOD).o

system_kernel_$(MMOD): $(OBJS)
		$(LD) $(LDFLAGS) -L$(CCLIB) --nmagic \
		--section-start $(MACH_CODE_NAME)=$(SYS_CODE_ADDR) \
		--section-start $(MACH_DATA_NAME)=$(SYS_DATA_ADDR) \
		--entry=_init -o system_$(MMOD) \
		$(LIB)/crtbegin_$(MMOD).o \
		system_scaffold.o system_bindings.o\
		$(LIB)/crtend_$(MMOD).o \
		--whole-archive \
		-l$(LSYS) -l$(LMACH) -l$(LARCH) \
		--no-whole-archive \
		-l$(LUTIL) -lgcc
		$(LD) $(LDFLAGS) -i application_scaffold.o application_binding.o -o application_$(MMOD).o

clean:
		$(CLEAN) *.o $(shell find -maxdepth 1 -executable -not -type d)

FORCE:
