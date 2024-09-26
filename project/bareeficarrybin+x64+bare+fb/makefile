project=$(shell pwd)
library=../../library




#asm
AS=as
AF=
#c
CC=gcc
CF=-c -O2 -fPIC -ffreestanding -fno-stack-protector -mno-sse2 -mno-red-zone -Wno-address-of-packed-member -I$(library)/lib1
CF_GNUEFI=-fshort-wchar -DGNU_EFI_USE_MS_ABI -I$(library)/lib0/gnu-efi -I$(library)/lib0/gnu-efi/inc -I$(library)/lib0/gnu-efi/inc/x86_64 -I$(library)/lib0/gnu-efi/inc/protocol
#link
LD=ld
LF=-T kernel.ld
#objcopy
CP=objcopy
#linux(make), non-linux(make cross)
ifeq ($(filter cross,$(MAKECMDGOALS)),cross)
	AS := x86_64-elf-as
	CC := x86_64-elf-gcc
	LD := x86_64-elf-ld
	CP := x86_64-elf-objcopy
endif




linux:bin
	make -f makefile.bin2pe
cross:bin
	make -f makefile.bin2pe cross
bin:elf
	$(CP) -O binary kernel.elf kernel.bin
elf:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	mv libboot0/main.o kernel-main.o
	$(LD) $(LF) \
	kernel-main.o \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-o kernel.elf
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) AF=$(AF) CC=$(CC) CF="$(CF) $(CF_GNUEFI)" bareeficarrybin
.PHONY:libboot1
libboot1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF)"
.PHONY:libhard0
libhard0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF)" barex64
.PHONY:libhard1
libhard1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF)"
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF)" bare
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF)"
.PHONY:libuser0
libuser0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF)" barefb
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF)"




macmount:
	hdiutil attach -mountpoint mntdir test.img
	mkdir -p mntdir/efi/boot
	ls -l mntdir/efi/boot
maccopy:
	cp BOOTX64.EFI mntdir/efi/boot
	ls -l mntdir/efi/boot
macumount:
	hdiutil detach mntdir
macremove:
	rm test.img
macall:
	make -s macmount
	make -s maccopy
	make -s macumount




clean:
	rm -rf lib*/*.o
	rmdir lib*
	rm -f *.o *.so *.elf *.efi *.EFI *.bin
