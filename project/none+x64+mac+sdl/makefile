library?=../../library
all:
	make -s -C $(library)/libboot0 none
	make -s -C $(library)/libboot1 clang
	make -s -C $(library)/libhard0 x64
	make -s -C $(library)/libhard1 clang
	make -s -C $(library)/libsoft0 mac
	make -s -C $(library)/libsoft1 clang
	make -s -C $(library)/libui0 macsdl
	make -s -C $(library)/libui1 clang
	make -s -C $(library)/init
	make -s a
	make -s dylib
	make -s program
a:
	ar crs 42.a \
	$(library)/init/startapp.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-lSDL2 -lm
dylib:
	ar crs 42.dylib \
	$(library)/init/startapp.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-lSDL2 -lm
program:
	gcc \
	$(library)/init/*.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-lSDL2 -lm
clean:
	rm -f *.o
