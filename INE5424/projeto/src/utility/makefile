# EPOS Utilities Makefile

include ../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc | grep -v test))

all:		$(LIBUTIL)

$(LIBUTIL):	$(LIBUTIL)($(OBJS))


clean:
		$(CLEAN) *.o *_test
