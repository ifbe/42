project=$(shell pwd)
library?=../../library




#c
CC=gcc
CF="-c -O2 -fPIC -ffreestanding -fno-stack-protector -mno-sse2 -mno-red-zone -Wno-address-of-packed-member -I../lib1"
#link
LD=ld
LF=-T link.ld
#objcopy
CP=objcopy
#linux(make), non-linux(make cross)
ifeq ($(filter cross,$(MAKECMDGOALS)),cross)
    CC := x86_64-elf-gcc
	LD := x86_64-elf-ld
	CP := x86_64-elf-objcopy
endif




linux:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s link
	make -s elf2bin
cross:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s link LD=$(LD)
	make -s elf2bin CP=$(CP)

.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) barebioslive
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
	$(LD) $(LF) \
	entry.o \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-o 42.elf
elf2bin:
	$(CP) -O binary 42.elf 42.bin

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
