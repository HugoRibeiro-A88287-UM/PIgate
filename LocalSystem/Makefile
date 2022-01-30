CCRASP = arm-buildroot-linux-gnueabihf-gcc

CFLAGS = -g -I. 

DEPS = -pthread -lrt -lpython3.9

RASPIP = 10.42.0.74

SRCFLODER = src/main.c \
			src/deamon.c \
			src/utilits.c \
			src/firebase.c \

PYTHONFILE = Firebase/firebase.py

.PHONY: all
all:
	$(CCRASP) $(SRCFLODER) -o daemonTest.elf $(DEPS)
	scp daemonTest.elf $(PYTHONFILE) root@$(RASPIP):/etc
	 

.PHONY: clean 
clean: 
	rm -f  daemonTest.elf

	