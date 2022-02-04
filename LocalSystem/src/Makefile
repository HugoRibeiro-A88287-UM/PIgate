CCRASP = arm-buildroot-linux-gnueabihf-gcc
CFLAGS = -g -I. 
DEPS = -pthread -lrt -lpython3.9 -lstdc++

CSRCFLODER=$(wildcard *.c)
CPPSRCFLODER=$(wildcard *.cpp)

.PHONY: all
all: PIgateProgram 

PIgateProgram:
	$(CCRASP) $(CSRCFLODER) $(CPPSRCFLODER) -o PIgateProgram.elf $(DEPS) $(CFLAGS)


.PHONY: clean
clean:
	rm -f *.elf