library?=../../library
linker?=ld
linux:
	make -s -C $(library)/libboot0 bare \
	CF="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libboot1 \
	CF="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libhard0 barex64 \
	CF="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libhard1 \
	CF="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libsoft0 bare \
	CF="-c -O2 -fPIC -fno-stack-protector -I."
	make -s -C $(library)/libsoft1 \
	CF="-c -O2 -fPIC -fno-stack-protector -I."
	make -s -C $(library)/libuser0 baretui \
	CF="-c -O2 -fPIC -fno-stack-protector -I."
	make -s -C $(library)/libuser1 \
	CF="-c -O2 -fPIC -fno-stack-protector -I."
	make -s link linker=/opt/cross/bin/x86_64-elf-ld
cross:
	make -s -C $(library)/libboot0 bare \
	CC="x86_64-elf-gcc" CF="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libboot1 \
	CC="x86_64-elf-gcc" CF="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libhard0 barex64 \
	CC="x86_64-elf-gcc" CF="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libhard1 \
	CC="x86_64-elf-gcc" CF="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libsoft0 bare \
	CC="x86_64-elf-gcc" CF="-c -O2 -fPIC -fno-stack-protector -I."
	make -s -C $(library)/libsoft1 \
	CC="x86_64-elf-gcc" CF="-c -O2 -fPIC -fno-stack-protector -I."
	make -s -C $(library)/libuser0 baretui \
	CC="x86_64-elf-gcc" CF="-c -O2 -fPIC -fno-stack-protector -I."
	make -s -C $(library)/libuser1 \
	CC="x86_64-elf-gcc" CF="-c -O2 -fPIC -fno-stack-protector -I."
	make -s link linker=x86_64-elf-ld
link:
	$(linker) -T link.ld \
	$(library)/libboot0/bare.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-o 42.bin
clean:
	rm -f *.out *.bin *.img
