# EPOS Machine Mediators Makefile

include	../../makedefs

ifeq ($(MAKE),$(MAKECLEAN))
SUBDIRS := $(shell find * -maxdepth 0 -type d)
else
SUBDIRS := common "$(MACH)"
endif

all:		$(SUBDIRS)

$(SUBDIRS):	FORCE
		(cd $@ && $(MAKE))

clean:
		make MAKE:="$(MAKECLEAN)" $(SUBDIRS)

FORCE:
