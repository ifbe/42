project=$(shell pwd)
library=../../library
AS=x86_64-elf-as
AF=
CC=x86_64-elf-gcc
CF=-c -fpic -fPIC -ffreestanding -mno-red-zone -Wno-address-of-packed-member -I$(library)/lib0/gnu-efi -I$(library)/lib0/gnu-efi/inc -I$(library)/lib0/gnu-efi/inc/x86_64 -I$(library)/lib0/gnu-efi/inc/protocol -I$(library)/lib1
LD=x86_64-elf-ld
LF=-e efi_main -nostdlib -fPIC -shared -fno-use-linker-plugin
all:gnuefi
	make -s addbin
	make -s link
	make -s elf2pe
gnuefi:
	$(AS) $(AF) $(library)/lib0/gnu-efi/gnuefi/crt0-efi-x86_64.S -o start.o
	$(CC) $(CF) $(library)/libboot0/bare/efipecarrybin/entry.c -o entry.o
	#$(CC) $(CF) $(library)/lib0/gnu-efi/lib/data.c -o data.o
addbin:
	dd if=/dev/zero of=test.bin bs=1M count=1
	x86_64-elf-objcopy --add-section .binary=test.bin entry.o
link:
	#$(LD) $(LF) -T link.ld -o bootx64.elf start.o entry.o data.o
	$(LD) -nostdlib -znocombreloc -T link.ld -shared -Bsymbolic -o bootx64.so start.o entry.o
elf2pe:
	x86_64-elf-objcopy -j .text -j .sdata -j .rodata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target efi-app-x86_64 --subsystem=10 bootx64.so bootx64.efi

clean:
	rm -rf lib*/*.o
	rm -f *.o *.so *.efi *.EFI
