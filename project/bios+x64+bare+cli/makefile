library?=../../library
all:
	make -s -C $(library)/libboot0 bare
	make -s -C $(library)/libboot1 gcc
	make -s -C $(library)/libhard0 x64
	make -s -C $(library)/libhard1 gcc
	make -s -C $(library)/libsoft0 bare
	make -s -C $(library)/libsoft1 gcc
	make -s -C $(library)/libui0 barecli
	make -s -C $(library)/libui1 gcc
	make -s -C $(library)/visitor0 bare
	make -s -C $(library)/visitor1 cli
	ld -T link.ld \
	$(library)/visitor0/start.o \
	$(library)/visitor0/event.o \
	$(library)/visitor1/cli.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-o core.bin
clean:
	rm -f *.out *.bin *.img
