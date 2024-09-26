project=$(shell pwd)
library=../../library




#asm
AS=as
AF=
#c
CC=gcc
CF=-c -fPIC -ffreestanding -fno-stack-protector -mno-sse2 -mno-red-zone -Wno-address-of-packed-member -I$(library)/lib1
CF_GNUEFI=-fshort-wchar -DGNU_EFI_USE_MS_ABI -I$(library)/lib0/gnu-efi -I$(library)/lib0/gnu-efi/inc -I$(library)/lib0/gnu-efi/inc/x86_64 -I$(library)/lib0/gnu-efi/inc/protocol
#link
LD=ld
LF=-nostdlib -znocombreloc -T link.ld -shared -Bsymbolic
#objcopy
CP=objcopy

ifeq ($(filter cross,$(MAKECMDGOALS)),cross)
	AS := x86_64-elf-as
	CC := x86_64-elf-gcc
	LD := x86_64-elf-ld
	CP := x86_64-elf-objcopy
endif




linux:pe
cross:pe
pe:so
	$(CP) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 bootx64.so bootx64.efi
so:elf
	mv libboot0/start.o .
	mv libboot0/main.o .
	$(LD) $(LF) \
	start.o main.o \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-o bootx64.so
elf:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	echo 1111
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ AS=$(AS) AF=$(AF) CC=$(CC) CF="$(CF) $(CF_GNUEFI)" bareefisysvabi
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
	rm -f *.o *.so *.elf *.efi *.EFI
