library?=../../library
flag?=-c -fPIC -ffreestanding -I../lib1 -I../lib0/gnu-efi/inc -I../lib0/gnu-efi/inc/x86_64 -I../lib0/gnu-efi/inc/protocol
all:
	make -s -C $(library)/libboot0 efiapp \
	CC="x86_64-w64-mingw32-gcc" CF="$(flag)"
	make -s -C $(library)/libboot1 \
	CC="x86_64-w64-mingw32-gcc" CF="$(flag)"
	make -s -C $(library)/libhard0 efix64 \
	CC="x86_64-w64-mingw32-gcc" CF="$(flag)"
	make -s -C $(library)/libhard1 \
	CC="x86_64-w64-mingw32-gcc" CF="$(flag)"
	make -s -C $(library)/libsoft0 efiapp \
	CC="x86_64-w64-mingw32-gcc" CF="$(flag)"
	make -s -C $(library)/libsoft1 \
	CC="x86_64-w64-mingw32-gcc" CF="$(flag)"
	make -s -C $(library)/libuser0 efifb \
	CC="x86_64-w64-mingw32-gcc" CF="$(flag)"
	make -s -C $(library)/libuser1 \
	CC="x86_64-w64-mingw32-gcc" CF="$(flag)"
	make -s gnuefi
	make -s link
gnuefi:
	x86_64-w64-mingw32-gcc -o data.o \
	-c -ffreestanding \
	-I$(library)/lib0/gnu-efi/inc \
	-I$(library)/lib0/gnu-efi/inc/x86_64 \
	-I$(library)/lib0/gnu-efi/inc/protocol \
	$(library)/lib0/gnu-efi/lib/data.c
link:
	x86_64-w64-mingw32-gcc -e efi_main \
	-nostdlib -fno-use-linker-plugin \
	-Wl,-dll -shared -Wl,--subsystem,10 \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	data.o -lgcc -o BOOTX64.EFI
clean:
	rm -f *.o *.EFI
	make -C $(library) clean
