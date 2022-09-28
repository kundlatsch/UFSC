# EPOS Tools Makefile

include	../makedefs

SUBDIRS := $(shell find * -maxdepth 0 -type d)

all:		$(SUBDIRS)

$(SUBDIRS):	FORCE
		(cd $@ && $(MAKE))

clean:
		make MAKE:="$(MAKECLEAN)" $(SUBDIRS)

FORCE:
