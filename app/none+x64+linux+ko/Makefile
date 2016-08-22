obj-m := testusb.o

KERNEL_DIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
fixgcc:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules CC=gcc-4.8
insmod:
	sudo insmod ./testusb.ko
rmmod:
	sudo rmmod testusb.ko
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *~
	rm -rf  .*.cmd .tmp_versions
