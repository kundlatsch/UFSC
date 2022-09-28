# EPOS Boot Makefile

include ../../makedefs

TARGET := $(if $(shell find boot_$(MMOD)* 2> /dev/null), install_$(MMOD))
ELF_HEADER_OFFSET := sh -c "$(OBJDUMP) -p $(IMG)/setup_$(MMOD) | sed -n -e 's/^ *LOAD off *\(0x.*\) vaddr.*$$/\1/ p' | head -1"

all: 		$(TARGET)

boot_legacy_pc: boot_legacy_pc.o
		ld86 -0 -s -T $(BOOT_ADDR) -o $@ $<

boot_legacy_pc.o: boot_legacy_pc.s
		sed -e 's/^ELF_HDR_SIZE =.*$$/ELF_HDR_SIZE = $(shell $(ELF_HEADER_OFFSET))/' -i $<
		as86 -0 -o $@ $<

install_legacy_pc: boot_legacy_pc
		$(DD) if=$< of=$<.tmp ibs=32 skip=1 obs=512 >& /dev/null
		$(INSTALL) boot_$(MMOD).tmp $(IMG)/boot_$(MMOD)
		$(CLEAN) boot_$(MMOD).tmp

clean:
		$(CLEAN) *.s *.o *.tmp $(shell find -maxdepth 1 -executable -not -type d)

