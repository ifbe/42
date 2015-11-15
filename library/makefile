purelibrary:
	make -s -C libsoft0 linux
	make -s -C libsoft1 linux
	make -s -C libui0 linux+xlib
	make -s -C libui1 linux
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o
linux:
	make -s -C libsoft0 linux
	make -s -C libsoft1 linux
	gcc -c -O2 librarian.old.c
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	librarian.old.o
mac:
	make -s -C libsoft0 mac
	make -s -C libsoft1 mac
	clang -c librarian.old.c
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	librarian.old.o
win:
	make -s -C libsoft0 win
	make -s -C libsoft1 win
	gcc -c -O2 librarian.old.c
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	librarian.old.o

linux+xlib:
	make -s -C libsoft0 linux
	make -s -C libsoft1 linux
	make -s -C libui0 linux+xlib
	make -s -C libui1 linux
	gcc -c -O2 librarian.c
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
win+api:
	make -s -C libsoft0 win
	make -s -C libsoft1 win
	make -s -C libui0 win+api
	make -s -C libui1 win
	gcc -c -O2 librarian.c
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
mac+sdl:
	make -s -C libsoft0 mac
	make -s -C libsoft1 mac
	make -s -C libui0 mac+api
	make -s -C libui1 mac
	clang -c librarian.c
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o
clean:
	rm -f *.o *.a
	rm -f */*.o */*.a
	rm -rf temp
