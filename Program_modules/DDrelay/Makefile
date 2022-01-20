obj-m := relay.o
relay-objs:= relayModule.o
KDIR := /home/hugo/Downloads/buildroot-2021.02.5/output/build/linux-custom
ARCH ?= arm
CROSS_COMPILE ?= arm-buildroot-linux-gnueabihf-


export ARCH CROSS_COMPILE


KBUILD_TARGETS	:= clean help modules modules_install

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@
	scp relay.ko root@10.42.0.74:/etc

clean:
        $(MAKE) -C $(KDIR) M=$(shell pwd) clean
