# EPOS Setup Makefile

include ../../makedefs

TARGET := $(if $(shell find setup_$(MMOD)* 2> /dev/null), $(if $(SETUP_ADDR), install, install.o))

all:	$(TARGET)

setup_$(MMOD):	setup_$(MMOD).o setup_binding.o
		$(LD) $(LDFLAGS) -L$(CCLIB) --omagic --entry=_entry --section-start .init=$(SETUP_ADDR) -o $@ $^ -l$(LINIT) -l$(LMACH) -l$(LARCH) -l$(LUTIL) -lgcc

install:	setup_$(MMOD)
		$(INSTALL) $< $(IMG)

install.o:	setup_$(MMOD).o
		$(INSTALL) $< $(LIB)

clean:
		$(CLEAN) *.o $(shell find -maxdepth 1 -executable -not -type d)
