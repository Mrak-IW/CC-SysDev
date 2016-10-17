ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

moduleName := examplemodule
modulePath := /lib/modules/$(shell uname -r)
moduleMarker := MODTEST

.PHONY: build clean install remove check

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c

check:
	dmesg | grep $(moduleMarker)

install: $(moduleName).ko
#	modprobe -v $(shell pwd)/$(moduleName).ko
	insmod $(shell pwd)/$(moduleName).ko

remove:
	rmmod $(moduleName)

else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})
moduleName := examplemodule
obj-m := $(moduleName).o

endif
