CCRASP = arm-buildroot-linux-gnueabihf-gcc

CFLAGS = -g -I. 

DEPS = -pthread -lrt

RASPIP = 10.42.0.74

SRCFLODER = 


.PHONY: all
all:
	$(CCRASP) src/main.c src/deamon.c src/utilits.c -o daemonTest.elf $(DEPS)
	scp daemonTest.elf root@$(RASPIP):/etc
	 

.PHONY: clean 
clean: 
	rm -f  daemonTest.elf

	