# EPOS Make Boot Image Tool Makefile

include	../../makedefs

all: install

eposmkbi: eposmkbi.cc
		$(TCXX) $(TCXXFLAGS) $<
		$(TLD) $(TLDFLAGS) -o $@ eposmkbi.o

install: eposmkbi
		$(INSTALL) -m 775 eposmkbi $(BIN)

clean:
		$(CLEAN) *.o eposmkbi
