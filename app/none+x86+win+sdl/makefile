library?=../../library
res?=../../res
all:
	make -s -C $(library)/libboot0 none
	make -s -C $(library)/libboot1 gcc
	make -s -C $(library)/libhard0 x86
	make -s -C $(library)/libhard1 gcc
	make -s -C $(library)/libsoft0 mac
	make -s -C $(library)/libsoft1 clang
	make -s -C $(library)/libui0 sdl
	make -s -C $(library)/libui1 gcc
	make -s -C $(library)/visitor gui
	gcc -mwindows \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor/*.o \
	-Wl,--no-undefined -static -static-libgcc \
	-L /opt/SDL2-2.0.3/x86_64-w64-mingw32/lib \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid
clean:
	rm -f *.o
