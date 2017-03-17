library?=../../library
linux:
	make -s -C $(library)/libboot0 bare \
	cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libboot1 \
	cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libhard0 x64 \
	cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libhard1 \
	cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libsoft0 bare \
	cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libsoft1 \
	cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libui0 barefb \
	cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libui1 \
	cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/visitor bare \
	cflag="-c -O2 -fPIC -fno-stack-protector"
	ld -T link.ld \
	$(library)/visitor/startbare.o \
	$(library)/visitor/main.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-o core.img
cross:
	make -s -C $(library)/libboot0 bare \
	compiler="x86_64-elf-gcc" cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libboot1 \
	compiler="x86_64-elf-gcc" cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libhard0 x64 \
	compiler="x86_64-elf-gcc" cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libhard1 \
	compiler="x86_64-elf-gcc" cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libsoft0 bare \
	compiler="x86_64-elf-gcc" cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libsoft1 \
	compiler="x86_64-elf-gcc" cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libui0 barefb \
	compiler="x86_64-elf-gcc" cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/libui1 \
	compiler="x86_64-elf-gcc" cflag="-c -O2 -fPIC -fno-stack-protector"
	make -s -C $(library)/visitor bare \
	compiler="x86_64-elf-gcc" cflag="-c -O2 -fPIC -fno-stack-protector"
	x86_64-elf-ld -T link.ld \
	$(library)/visitor/startbare.o \
	$(library)/visitor/main.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-o core.img
clean:
	rm -f *.out *.bin *.img
