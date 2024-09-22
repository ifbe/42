project=$(shell pwd)
library=../../library
CC=x86_64-w64-mingw32-gcc
CF=-c -fPIC -ffreestanding -mno-red-zone -Wno-address-of-packed-member -I$(library)/lib0/gnu-efi/inc -I$(library)/lib0/gnu-efi/inc/x86_64 -I$(library)/lib0/gnu-efi/inc/protocol -I$(library)/lib1
LD=x86_64-w64-mingw32-ld
LF=-e efi_main -nostdlib -fno-use-linker-plugin -fPIC -shared
all:binary gnuefi
	x86_64-w64-mingw32-objcopy --add-section .binary=test.bin data.o
	make -s link
binary:
	dd if=/dev/zero of=test.bin bs=1M count=1
gnuefi:
	$(CC) $(CF) $(library)/libboot0/bare/efipecarrybin/entry.c -o entry.o
	$(CC) $(CF) $(library)/lib0/gnu-efi/lib/data.c -o data.o
link:
	$(LD) $(LF) -T link.ld -o BOOTX64.EFI \
	entry.o data.o

clean:
	rm -rf lib*/*.o
	rm -f *.o *.EFI
