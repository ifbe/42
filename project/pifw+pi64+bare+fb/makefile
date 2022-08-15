project=$(shell pwd)
library=../../library
AS=aarch64-elf-as
AF="-c"
CC=aarch64-elf-gcc
CF="-c -O2 -I../lib1 -fPIC -ffreestanding -fno-stack-protector"
LD=aarch64-elf-ld
LF=-T link.ld -o 42.elf

all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	$(LD) $(LF) \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o
	aarch64-elf-objcopy 42.elf -O binary kernel8.img
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) CC=$(CC) CF=$(CF) rpi
.PHONY:libboot1
libboot1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) CC=$(CC) CF=$(CF)
.PHONY:libhard0
libhard0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) CC=$(CC) CF=$(CF) barepi64
.PHONY:libhard1
libhard1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) CC=$(CC) CF=$(CF)
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) CC=$(CC) CF=$(CF) bare
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) CC=$(CC) CF=$(CF)
.PHONY:libuser0
libuser0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) CC=$(CC) CF=$(CF) rpifb
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) CC=$(CC) CF=$(CF)
rpi3b:
	qemu-system-aarch64 -M raspi3b -serial null -serial stdio -drive file=../../../live/base/loader/live.img,if=sd,format=raw -kernel kernel8.img
rpi4b:
	echo "not yet"
clean:
	rm -f */*.o
	rm -f *.o *.out *.bin *.img
