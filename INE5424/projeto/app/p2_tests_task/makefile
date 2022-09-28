# EPOS Application Makefile

include ../../makedefs

all: install

$(APPLICATION):	$(APPLICATION).o $(LIB)/*
		$(ALD) $(ALDFLAGS) -o $@ $(APPLICATION).o

$(APPLICATION).o: $(APPLICATION).cc $(SRC)
		$(ACC) $(ACCFLAGS) -o $@ $<

install: $(APPLICATION)
		$(INSTALL) $(APPLICATION) $(IMG)

clean:
		$(CLEAN) *.o $(APPLICATION)
