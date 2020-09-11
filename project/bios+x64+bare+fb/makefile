library?=../../library
linker?=ld
CF="-c -O2 -I.. -fPIC -ffreestanding -fno-stack-protector -mno-red-zone"
linux:
	+make -s -C $(library)/libboot0 CF=$(CF) bare
	+make -s -C $(library)/libboot1 CF=$(CF)
	+make -s -C $(library)/libhard0 CF=$(CF) barex64
	+make -s -C $(library)/libhard1 CF=$(CF)
	+make -s -C $(library)/libsoft0 CF=$(CF) bare
	+make -s -C $(library)/libsoft1 CF=$(CF)
	+make -s -C $(library)/libuser0 CF=$(CF) barefb
	+make -s -C $(library)/libuser1 CF=$(CF)
	make -s link
cross:
	+make -s -C $(library)/libboot0 CC="x86_64-elf-gcc" CF=$(CF) bare
	+make -s -C $(library)/libboot1 CC="x86_64-elf-gcc" CF=$(CF)
	+make -s -C $(library)/libhard0 CC="x86_64-elf-gcc" CF=$(CF) barex64
	+make -s -C $(library)/libhard1 CC="x86_64-elf-gcc" CF=$(CF)
	+make -s -C $(library)/libsoft0 CC="x86_64-elf-gcc" CF=$(CF) bare
	+make -s -C $(library)/libsoft1 CC="x86_64-elf-gcc" CF=$(CF)
	+make -s -C $(library)/libuser0 CC="x86_64-elf-gcc" CF=$(CF) barefb
	+make -s -C $(library)/libuser1 CC="x86_64-elf-gcc" CF=$(CF)
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
