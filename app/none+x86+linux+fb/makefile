all:
	make -s -C libboot0 none
	make -s -C libboot1 gcc
	make -s -C libhard0 x86
	make -s -C libhard1 gcc
	make -s -C libsoft0 linux
	make -s -C libsoft1 gcc
	make -s -C libui0 fb
	make -s -C libui1 gcc
	gcc -c -O2 librarian.c
	rm -f library.a
	ar crs library.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o gui.o
clean:
	rm -f *.o
