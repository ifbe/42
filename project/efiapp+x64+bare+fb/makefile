library?=../../library
flag?=-c -fPIC -ffreestanding -I.. -I../gnu-efi/inc -I../gnu-efi/inc/x86_64 -I../gnu-efi/inc/protocol
all:
	make -s -C $(library)/libboot0 efiapp \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(flag)"
	make -s -C $(library)/libboot1 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(flag)"
	make -s -C $(library)/libhard0 efix64 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(flag)"
	make -s -C $(library)/libhard1 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(flag)"
	make -s -C $(library)/libsoft0 efiapp \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(flag)"
	make -s -C $(library)/libsoft1 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(flag)"
	make -s -C $(library)/libuser0 efifb \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(flag)"
	make -s -C $(library)/libuser1 \
	compiler="x86_64-w64-mingw32-gcc" cflag="$(flag)"
	make -s gnuefi
	make -s link
gnuefi:
	x86_64-w64-mingw32-gcc -o data.o \
	-c -ffreestanding \
	-I$(library)/gnu-efi/inc \
	-I$(library)/gnu-efi/inc/x86_64 \
	-I$(library)/gnu-efi/inc/protocol \
	$(library)/gnu-efi/lib/data.c
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
