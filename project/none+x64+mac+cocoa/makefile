library?=../../library
res?=../../res
all:
	make -s -C $(library)/init
	make -s -C $(library)/libboot0 none
	make -s -C $(library)/libboot1
	make -s -C $(library)/libhard0 x64
	make -s -C $(library)/libhard1
	make -s -C $(library)/libsoft0 mac
	make -s -C $(library)/libsoft1
	make -s -C $(library)/libui0 maccocoa
	make -s -C $(library)/libui1
	make -s a
	make -s dylib
	make -s program
a:
	libtool -static -o 42.a \
	$(library)/init/startapp.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o
dylib:
	libtool -dynamic -o 42.dylib \
	$(library)/init/startapp.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o
program:
	clang -framework Foundation \
	$(library)/init/*.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o
clean:
	rm -f *.o *.a *.dylib
