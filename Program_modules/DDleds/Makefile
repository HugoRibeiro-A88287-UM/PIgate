obj-m := ledRGB.o
ledRGB-objs:= ledModule.o
KDIR := /home/hugo/Downloads/buildroot-2021.02.5/output/build/linux-custom
ARCH ?= arm64
CROSS_COMPILE ?= aarch64-buildroot-linux-gnu-


export ARCH CROSS_COMPILE


KBUILD_TARGETS	:= clean help modules modules_install

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@	
