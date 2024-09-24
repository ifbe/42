project=$(shell pwd)
library=../../library




#asm
AS=x86_64-elf-as
AF=
#c
CC=x86_64-elf-gcc
CF=-c -fPIC -ffreestanding -fno-stack-protector -mno-sse2 -mno-red-zone -Wno-address-of-packed-member -I$(library)/lib1
CF_GNUEFI=-fshort-wchar -DGNU_EFI_USE_MS_ABI -I$(library)/lib0/gnu-efi -I$(library)/lib0/gnu-efi/inc -I$(library)/lib0/gnu-efi/inc/x86_64 -I$(library)/lib0/gnu-efi/inc/protocol
#link
LD=x86_64-elf-ld
LF=-nostdlib -znocombreloc -T link.ld -shared -Bsymbolic
#objcopy
CP=x86_64-elf-objcopy




all:
	make -s gnuefi
	make -s addbin
	make -s link
	make -s elf2pe
gnuefi:
	$(AS) $(AF) $(library)/libboot0/bare/efi/carrybin/start.S -o start.o
	$(CC) $(CF) $(CF_GNUEFI) $(library)/libboot0/bare/efi/carrybin/main.c -o main.o
addbin:
	dd if=/dev/zero of=test.bin bs=1M count=1
	$(CP) --add-section .binary=test.bin main.o
link:
	$(LD) $(LF) start.o main.o -o bootx64.so
elf2pe:
	$(CP) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 bootx64.so bootx64.efi




clean:
	rm -rf lib*/*.o
	rm -f *.o *.so *.efi *.EFI
