project=$(shell pwd)
library?=../../library
linker?=ld
elf2bin?=objcopy
CC:=gcc
CF="-c -O2 -I../lib1 -fPIC -ffreestanding -fno-stack-protector -mno-sse2 -mno-red-zone -Wno-address-of-packed-member"

ifeq ($(filter cross,$(MAKECMDGOALS)),cross)
    CC := x86_64-elf-gcc
endif

linux:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s link
cross:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s link linker=x86_64-elf-ld elf2bin=x86_64-elf-objcopy

.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) bare
.PHONY:libboot1
libboot1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
.PHONY:libhard0
libhard0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) barex64
.PHONY:libhard1
libhard1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) bare
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
.PHONY:libuser0
libuser0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) barefb
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
link:
	mv libboot0/entry.o .
	$(linker) -T link.ld entry.o \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-o 42.elf
	$(elf2bin) -O binary 42.elf 42.bin

copy:
	cp 42.bin ../../../live/mntdir/
macall:
	make -C /Users/ifbe/Desktop/code/ifbe/thelive/software/loader/bios macmount
	cp 42.bin ../../../thelive/software/loader/bios/mntdir
	make -C /Users/ifbe/Desktop/code/ifbe/thelive/software/loader/bios macumount
	make -C /Users/ifbe/Desktop/code/ifbe/thelive/software/loader/bios liveimg
	make -C /Users/ifbe/Desktop/code/ifbe/thelive/software/loader/bios qemu

clean:
	rm -f lib*/*.o
	rm -f *.o *.bin
cleanimage:
	rm -f *.o *.out *.bin *.img
	make -C $(library) clean
