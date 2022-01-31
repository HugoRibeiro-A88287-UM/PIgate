CCRASP = arm-buildroot-linux-gnueabihf-gcc
CFLAGS = -g -I. 
DEPS = -pthread -lrt -lpython3.9

SRCFLODER=$(wildcard *.c)

.PHONY: all
all: PIgateProgram 

PIgateProgram:
	$(CCRASP) $(SRCFLODER) -o PIgateProgram.elf $(DEPS) $(CFLAGS)


.PHONY: clean
clean:
	rm -f *.elf