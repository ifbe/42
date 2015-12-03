arm+linux+cli:
	make -s -C libhard0 arm
	make -s -C libhard1 gcc
	make -s -C libsoft0 linux
	make -s -C libsoft1 gcc
	make -s -C libui0 cli
	make -s -C libui1 gcc
	gcc -c -O2 librarian.c
	rm -f library.a
	ar crs library.a \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
x86+linux+cli:
	make -s -C libhard0 x86
	make -s -C libhard1 gcc
	make -s -C libsoft0 linux
	make -s -C libsoft1 gcc
	make -s -C libui0 cli
	make -s -C libui1 gcc
	gcc -c -O2 librarian.c
	rm -f library.a
	ar crs library.a \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
x86+mac+cli:
	make -s -C libhard0 x86
	make -s -C libhard1 clang
	make -s -C libsoft0 mac
	make -s -C libsoft1 clang
	make -s -C libui0 mac
	make -s -C libui1 clang
	clang -c librarian.c
	rm -f library.a
	ar crs library.a \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
x86+win+cli:
	make -s -C libhard0 x86
	make -s -C libhard1 gcc
	make -s -C libsoft0 win
	make -s -C libsoft1 gcc
	make -s -C libui0 win
	make -s -C libui1 gcc
	gcc -c -O2 librarian.c
	rm -f library.a
	ar crs library.a \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
x86+linux+xlib:
	make -s -C libhard0 x86
	make -s -C libhard1 gcc
	make -s -C libsoft0 linux
	make -s -C libsoft1 gcc
	make -s -C libui0 xlib
	make -s -C libui1 gcc
	gcc -c -O2 librarian.c
	rm -f library.a
	ar crs library.a \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
x86+win+api:
	make -s -C libhard0 x86
	make -s -C libhard1 gcc
	make -s -C libsoft0 win
	make -s -C libsoft1 gcc
	make -s -C libui0 winapi
	make -s -C libui1 gcc
	gcc -c -O2 librarian.c
	rm -f library.a
	ar crs library.a \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
x86+mac+sdl:
	make -s -C libhard0 x86
	make -s -C libhard1 clang
	make -s -C libsoft0 mac
	make -s -C libsoft1 clang
	make -s -C libui0 mac+api
	make -s -C libui1 clang
	clang -c librarian.c
	rm -f library.a
	ar crs library.a \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
clean:
	rm -f *.o *.a
	rm -f */*.o */*.a
