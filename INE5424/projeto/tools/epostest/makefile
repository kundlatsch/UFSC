# EPOS Test Tool Makefile

include	../../makedefs

all:		install

install:	epostest
		$(INSTALL) -m 775 epostest $(BIN)
		sed -e 's/^EPOS=.*$$/EPOS=$(subst /,\/,$(EPOS))/' -i $(BIN)/epostest

clean:
