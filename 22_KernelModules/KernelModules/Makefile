chdevModuleName := cctaskmodule
modulePath := /lib/modules/$(shell uname -r)
moduleMarker := MODTEST
charDevName := ccchardev

ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: build clean install remove check

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.symvers *.order

tests:
	@printf "\n"
	echo test_message > /dev/$(charDevName)
	@printf "\n"
	cat /dev/$(charDevName)
	
log:
	@printf "\n"
	dmesg | grep $(moduleMarker) | tail -n 30
	@printf "\n"
	lsmod | grep $(chdevModuleName)
	@printf "\n"
	cat /proc/devices | grep $(charDevName)
	@printf "\n"
	ls -lah /dev | grep $(charDevName)

check: tests log

install: $(chdevModuleName).ko
#	modprobe -v $(shell pwd)/$(chdevModuleName).ko
	sudo insmod $(shell pwd)/$(chdevModuleName).ko
	sudo chmod 666 /dev/$(charDevName)

remove:
	sudo rmmod $(chdevModuleName)

else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})
obj-m := $(chdevModuleName).o

endif
