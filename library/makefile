purelibrary:
	make -s -C libsoft0 linux
	make -s -C libsoft1 linux
	make -s -C libui0 linux+xlib
	make -s -C libui1 linux
	mkdir -p temp/soft
	mkdir -p temp/ui
	mv libsoft0/*.o temp/soft
	mv libsoft1/*.o temp/soft
	mv libui0/*.o temp/ui
	mv libui1/*.o temp/ui
	rm -f library.a
	ar crs library.a temp/soft/*.o temp/ui/*.o
linux:
	gcc -c -O2 librarian.old.c
	make -s -C libsoft0 linux
	make -s -C libsoft1 linux
	mkdir -p temp
	mv libsoft0/*.o temp/
	mv libsoft1/*.o temp/
	rm -f library.a
	ar crs library.a librarian.old.o temp/*.o
mac:
	clang -c librarian.c
	make -s -C libsoft0 mac
	make -s -C libsoft1 mac
	rm -f library.a
	ar crs library.a temp/*.o
win:
	gcc -c -O2 librarian.c
	make -s -C libsoft0 win
	make -s -C libsoft1 win
	rm -f library.a
	ar crs library.a temp/*.o

linux+xlib:
	gcc -c -O2 librarian.c
	make -s -C libsoft0 linux
	make -s -C libsoft1 linux
	make -s -C libui0 linux+xlib
	make -s -C libui1 linux
	mkdir -p temp/soft
	mkdir -p temp/ui
	mv libsoft0/*.o temp/soft
	mv libsoft1/*.o temp/soft
	mv libui0/*.o temp/ui
	mv libui1/*.o temp/ui
	rm -f library.a
	ar crs library.a librarian.o temp/soft/*.o temp/ui/*.o
win+api:
	gcc -c -O2 librarian.c
	make -s -C libsoft0 win
	make -s -C libsoft1 win
	make -s -C libui0 win+api
	make -s -C libui1 win
	rm -f library.a
	ar crs library.a librarian.o \
	libui0/libui0.a libui1/libui1.a \
	libsoft0/libsoft0.a libsoft1/libsoft1.a
mac+sdl:
	clang -c librarian.c
	make -s -C libsoft0 mac
	make -s -C libsoft1 mac
	make -s -C libui0 mac+api
	make -s -C libui1 mac
	rm -f library.a
	ar crs library.a librarian.o \
	libui0/libui0.a libui1/libui1.a \
	libsoft0/libsoft0.a libsoft1/libsoft1.a
clean:
	rm -f *.o *.a
	rm -f */*.o */*.a
	rm -rf temp
