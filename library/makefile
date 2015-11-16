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
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o
mac:
	make -s -C libsoft0 mac
	make -s -C libsoft1 mac
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o
win:
	make -s -C libsoft0 win
	make -s -C libsoft1 win
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o

linux+xlib:
	make -s -C libsoft0 linux
	make -s -C libsoft1 linux
	make -s -C libui0 linux+xlib
	make -s -C libui1 linux
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o
win+api:
	make -s -C libsoft0 win
	make -s -C libsoft1 win
	make -s -C libui0 win+api
	make -s -C libui1 win
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o
mac+sdl:
	make -s -C libsoft0 mac
	make -s -C libsoft1 mac
	make -s -C libui0 mac+api
	make -s -C libui1 mac
	rm -f library.a
	ar crs library.a \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o
clean:
	rm -f *.o *.a
	rm -f */*.o */*.a
	rm -rf temp
