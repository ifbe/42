all:
	make -s -C libboot0 none
	make -s -C libboot1 clang
	make -s -C libhard0 x86
	make -s -C libhard1 clang
	make -s -C libsoft0 mac
	make -s -C libsoft1 clang
	make -s -C libui0 cli
	make -s -C libui1 clang
	clang -c librarian.c
	rm -f library.a
	ar crs library.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o gui.o \
	-lSDL2 -lm
clean:
	rm -f *.o
