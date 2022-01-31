obj-m := ledRGB.o
ledRGB-objs:= ledModule.o
KDIR := /home/hugo/Downloads/buildroot-2021.02.5/output/build/linux-custom
ARCH ?= arm
CROSS_COMPILE ?= arm-buildroot-linux-gnueabihf-
PIgateFolder = ../PIgate

export ARCH CROSS_COMPILE

KBUILD_TARGETS	:= clean help modules modules_install


.PHONY: all $(KBUILD_TARGETS)

all: modules 


$(KBUILD_TARGETS):
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@	

transferledRGBKO:
	cp ledRGB.ko $(PIgateFolder)
	
clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
