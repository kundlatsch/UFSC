# EPOS Application Makefile

include ../makedefs

SUBDIRS := $(shell find * -maxdepth 0 -type d)

all:		$(APPLICATION)

$(APPLICATION):	FORCE
		(cd $@ && $(MAKE))

clean:
		make MAKE:="$(MAKECLEAN)" $(APPLICATION)

FORCE:
		