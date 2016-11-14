library?=../../library
all:
	make -s -C $(library)/libboot0 none
	make -s -C $(library)/libboot1
	make -s -C $(library)/libhard0 x64
	make -s -C $(library)/libhard1
	make -s -C $(library)/libsoft0 linux
	make -s -C $(library)/libsoft1
	make -s -C $(library)/libui0 linuxfb
	make -s -C $(library)/libui1
	make -s -C $(library)/visitor0
	make -s -C $(library)/visitor1 gui
	make -s a
	make -s so
	make -s program
a:
	ar crs 42.a \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor0/*.o
so:
	gcc -shared -o 42.so \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor0/*.o
program:
	gcc -o a.out \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor0/*.o \
	$(library)/visitor1/gui.o \
	-lpthread
clean:
	rm -f *.o *.a *.so
