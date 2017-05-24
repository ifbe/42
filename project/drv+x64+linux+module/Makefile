CC := gcc
obj-m := test.o
test-objs := ../../library/init/startko.o

KERNEL_DIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
insmod:
	sudo insmod ./test.ko
rmmod:
	sudo rmmod test.ko
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *~
	rm -rf  .*.cmd .tmp_versions
