project=$(shell pwd)
library=../../library




CC=x86_64-w64-mingw32-gcc
CF=-c -fPIC -ffreestanding -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -mno-sse2 -mno-red-zone -Wno-address-of-packed-member -I../lib1
CF_GNUEFI=-I../lib0/gnu-efi -I../lib0/gnu-efi/inc -I../lib0/gnu-efi/inc/x86_64 -I../lib0/gnu-efi/inc/protocol
LD=x86_64-w64-mingw32-gcc
LF=-e efi_main -nostdlib -shared -fno-use-linker-plugin -Wl,-dll -Wl,--subsystem,10




withclang:libboot0 libboot1 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -f makefile.clang -s libhard0	#mingw64 bug: attribute interrupt
	make -s link
withmingw:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s link
	@echo "--------mingw may have bug, if so, make withclang--------"
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF) $(CF_GNUEFI)" bareefimsabi
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
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF)" efifb
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF="$(CF)"
link:
	mv libboot0/main.o .
	$(LD) $(LF) \
	main.o \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-o BOOTX64.EFI




#----------------linux----------------
linuxmkfs:
	dd if=/dev/zero of=test.img bs=1048576 count=62
	mkfs.fat -F32 test.img
linuxmount:
	mkdir -p mntdir
	sudo mount test.img mntdir
	mkdir -p mntdir/efi/boot
linuxcopy:
	cp BOOTX64.EFI mntdir/efi/boot
	ls -l mntdir/efi/boot
linuxumount:
	sudo umount mntdir
linuxremove:
	rm test.img



#----------------mac----------------
macdd:
	dd if=/dev/zero of=test.img bs=1048576 count=64
macmkfs:
	$(eval DISK := $(shell hdiutil attach -nomount test.img))
	newfs_msdos -F 32 $(DISK)
	hdiutil detach $(DISK)
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

#----------------img----------------
vhd:
	qemu-img convert -f raw -O vpc -o subformat=fixed test.img test.vhd
vmdk:
	qemu-img convert -f raw -O vmdk test.img test.vmdk
hdd:
	qemu-img convert -f raw -O parallels test.img test.hdd
vhd2img:
	qemu-img convert -f vpc -O raw test.vhd test.img

#----------------run----------------
qemu:
	../../../thelive/software/virt/qemu/efi.sh "qemu-system-x86_64" $(shell pwd)/test.img $(shell pwd)/test.vhd




clean:
	rm -rf lib*/*.o
	rm -f *.o *.EFI
