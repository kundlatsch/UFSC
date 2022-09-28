# EPOS Configuration Makefile

include	../makedefs

all: $(APP)/$(APPLICATION)/$(APPLICATION)_traits.h config eposcfg eposcc.conf eposmkbi.conf 

$(APP)/$(APPLICATION)/$(APPLICATION)_traits.h:
		$(error Missing traits file $@ !)

eposcfg: FORCE
		(cd $(TLS)/eposcfg && $(MAKE) clean all)

config: $(INCLUDE)/system/config.h
		sed -e 's/^#define SMOD.*$$/#define SMOD $(SMOD)/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define ARCH.*$$/#define ARCH $(ARCH)/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define MACH.*$$/#define MACH $(MACH)/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define MMOD.*$$/#define MMOD $(MMOD)/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define APPL.*$$/#define APPL $(APPLICATION)/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define __mode_.*__$$/#define __mode_$(SMOD)__/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define __arch_.*__$$/#define __arch_$(ARCH)__/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define __mach_.*__$$/#define __mach_$(MACH)__/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define __mmod_.*__$$/#define __mmod_$(MMOD)__/' -i $(INCLUDE)/system/config.h
	  		
eposcc.conf: FORCE
		@echo "SMOD=$(shell $(BIN)/eposcfg MODE)" > $@
		@echo "ARCH=$(shell $(BIN)/eposcfg ARCH)" >> $@
		@echo "MACH=$(shell $(BIN)/eposcfg MACH)" >> $@
		@echo "MMOD=$(shell $(BIN)/eposcfg MMOD)" >> $@
		@echo "TOOLS_PREFIX=$(COMP_PREFIX)" >> $@
		@echo "APP_CODE=$(shell $(BIN)/eposcfg APP_CODE)" >> $@
		@echo "APP_DATA=$(shell $(BIN)/eposcfg APP_DATA)" >> $@
		@echo "SETUP=$(shell $(BIN)/eposcfg SETUP)" >> $@
		@echo "BOOT_STACK=$(shell $(BIN)/eposcfg BOOT_STACK)" >> $@
		@echo "MACH_CC_FLAGS='$(MACH_CC_FLAGS)'" >> $@
		@echo "MACH_LD_FLAGS='$(MACH_LD_FLAGS)'" >> $@
		@echo "MACH_CODE_NAME='$(MACH_CODE_NAME)'" >> $@
		@echo "MACH_DATA_NAME='$(MACH_DATA_NAME)'" >> $@

eposmkbi.conf:	FORCE
		@echo "SMOD=$(shell $(BIN)/eposcfg MODE)" > $@
		@echo "ARCH=$(shell $(BIN)/eposcfg ARCH)" >> $@
		@echo "MACH=$(shell $(BIN)/eposcfg MACH)" >> $@
		@echo "MMOD=$(shell $(BIN)/eposcfg MMOD)" >> $@
		@echo "CPUS=$(shell $(BIN)/eposcfg CPUS)" >> $@
		@echo "CLOCK=$(shell $(BIN)/eposcfg CLOCK)" >> $@
		@echo "WORD_SIZE=$(shell $(BIN)/eposcfg WORD_SIZE)" >> $@
		@echo "ENDIANESS=$(shell $(BIN)/eposcfg ENDIANESS)" >> $@
		@echo "RAM_BASE=$(shell $(BIN)/eposcfg RAM_BASE)" >> $@
		@echo "RAM_TOP=$(shell $(BIN)/eposcfg RAM_TOP)" >> $@
		@echo "MIO_BASE=$(shell $(BIN)/eposcfg MIO_BASE)" >> $@
		@echo "MIO_TOP=$(shell $(BIN)/eposcfg MIO_TOP)" >> $@
		@echo "NODE_ID=$(NODE_ID)" >> $@
		@echo "UUID=$(UUID)" >> $@

clean: $(APP)/$(APPLICATION)/$(APPLICATION)_traits.h
		sed -e 's/^#define SMOD.*$$/#define SMOD xxx/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define ARCH.*$$/#define ARCH xxx/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define MACH.*$$/#define MACH xxx/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define MMOD.*$$/#define MMOD xxx/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define APPL.*$$/#define APPL xxx/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define __mode_.*__$$/#define __mode_xxx__/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define __arch_.*__$$/#define __arch_xxx__/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define __mach_.*__$$/#define __mach_xxx__/' -i $(INCLUDE)/system/config.h
		sed -e 's/^#define __mmod_.*__$$/#define __mmod_xxx__/' -i $(INCLUDE)/system/config.h
		$(CLEAN) *.conf

FORCE:
