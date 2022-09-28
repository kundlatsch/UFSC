# EPOS Cortex Machine Mediators Makefile

include	../../../makedefs

ifeq ($(MAKE),$(MAKECLEAN))
SUBDIRS := $(shell find * -maxdepth 0 -type d)
else
SUBDIRS := $(MMOD)
endif

all:		common $(SUBDIRS)

$(SUBDIRS):	FORCE
		(cd $@ && $(MAKE))

OBJS := $(subst .cc,.o,$(shell find *.cc | grep -v _test | grep -v _init))
INITS := $(subst .cc,.o,$(shell find *.cc | grep _init))
common:		$(LIBMACH) $(LIBINIT)

$(LIBMACH):	$(LIBMACH)($(OBJS))

$(LIBINIT):	$(LIBINIT)($(INITS))

clean:
		make MAKE:="$(MAKECLEAN)" $(SUBDIRS)
		$(CLEAN) *.o *_test

FORCE:
