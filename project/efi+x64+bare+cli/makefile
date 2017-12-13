library?=../../library
folder?=$(shell pwd)/gnu-efi
aflag?=-c -ffreestanding -I$(folder)/inc -I$(folder)/inc/x86_64 -I$(folder)/inc/protocol
all:
	make -s -C $(library)/libboot0 efi \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(aflag)"
	make -s -C $(library)/libboot1 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(aflag)"
	make -s -C $(library)/libhard0 x64 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(aflag)"
	make -s -C $(library)/libhard1 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(aflag)"
	make -s -C $(library)/libsoft0 efi \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(aflag) -I."
	make -s -C $(library)/libsoft1 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(aflag) -I."
	make -s -C $(library)/libui0 eficli \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(aflag) -I."
	make -s -C $(library)/libui1 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(aflag) -I."
	x86_64-w64-mingw32-gcc $(aflag) gnu-efi/lib/data.c -o data.o
	make -s link
link:
	x86_64-w64-mingw32-gcc -e efi_main \
	-nostdlib -fno-use-linker-plugin \
	-Wl,-dll -shared -Wl,--subsystem,10 \
	$(library)/libboot0/startefi.o \
	$(library)/libboot0/basic.o \
	$(library)/libboot0/print.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	data.o -lgcc -o BOOTX64.EFI
