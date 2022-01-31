CC = gcc
CCRASP = arm-buildroot-linux-gnueabihf-gcc 
CFLAGS = -g -I. 
DEPS = -lpython3.9
RASPIP = 10.42.0.74

.PHONY: all
all: testFirebase

testFirebase:
	$(CCRASP) firebase_test.c -o firebase_test.elf $(DEPS)
	scp firebase_test.elf firebase.py root@$(RASPIP):/etc


.PHONY: clean 
clean: 
	rm -f *.elf
	
