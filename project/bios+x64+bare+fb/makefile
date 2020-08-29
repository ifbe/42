library?=../../library
linker?=ld
flag="-c -O2 -I.. -fPIC -ffreestanding -fno-stack-protector -mno-red-zone"
linux:
	make -s -C $(library)/libboot0 cflag=$(flag) bare
	make -s -C $(library)/libboot1 cflag=$(flag)
	make -s -C $(library)/libhard0 cflag=$(flag) barex64
	make -s -C $(library)/libhard1 cflag=$(flag)
	make -s -C $(library)/libsoft0 cflag=$(flag) bare
	make -s -C $(library)/libsoft1 cflag=$(flag)
	make -s -C $(library)/libuser0 cflag=$(flag) barefb
	make -s -C $(library)/libuser1 cflag=$(flag)
	make -s link
cross:
	make -s -C $(library)/libboot0 compiler="x86_64-elf-gcc" cflag=$(flag) bare
	make -s -C $(library)/libboot1 compiler="x86_64-elf-gcc" cflag=$(flag)
	make -s -C $(library)/libhard0 compiler="x86_64-elf-gcc" cflag=$(flag) barex64
	make -s -C $(library)/libhard1 compiler="x86_64-elf-gcc" cflag=$(flag)
	make -s -C $(library)/libsoft0 compiler="x86_64-elf-gcc" cflag=$(flag) bare
	make -s -C $(library)/libsoft1 compiler="x86_64-elf-gcc" cflag=$(flag)
	make -s -C $(library)/libuser0 compiler="x86_64-elf-gcc" cflag=$(flag) barefb
	make -s -C $(library)/libuser1 compiler="x86_64-elf-gcc" cflag=$(flag)
	make -s link linker=x86_64-elf-ld
link:
	mv $(library)/libboot0/entry.o .
	$(linker) -T link.ld entry.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-o 42.bin
copy:
	cp 42.bin ../../../live/mount/
clean:
	rm -f *.o *.out *.bin *.img
	make -C $(library) clean
