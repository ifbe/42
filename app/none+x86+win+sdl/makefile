all:
	make -s -C libboot0 none
	make -s -C libboot1 gcc
	make -s -C libhard0 x86
	make -s -C libhard1 clang
	make -s -C libsoft0 mac
	make -s -C libsoft1 clang
	make -s -C libui0 mac+api
	make -s -C libui1 clang
	clang -c librarian.c
	rm -f library.a
	ar crs library.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libui0/*.o libui1/*.o \
	librarian.o gui.o \
	-Wl,--no-undefined -static -static-libgcc \
	-L /opt/SDL2-2.0.3/x86_64-w64-mingw32/lib \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid
clean:
	rm -f *.o
