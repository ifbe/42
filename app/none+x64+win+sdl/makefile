library?=../../library
res?=../../res
all:
	make -s -C $(library)/libboot0 wincmd
	make -s -C $(library)/libboot1 mingw
	make -s -C $(library)/libhard0 winx64
	make -s -C $(library)/libhard1 mingw
	make -s -C $(library)/libsoft0 win
	make -s -C $(library)/libsoft1 mingw
	make -s -C $(library)/libui0 winsdl
	make -s -C $(library)/libui1 mingw
	make -s -C $(library)/visitor0 win
	make -s -C $(library)/visitor1 gui
	make -s a
	make -s dll
	make -s program
a:
	ar crs 42.a \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor0/*.o
dll:
	gcc -shared -o 42.dll \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor0/*.o \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid
program:
	gcc -mwindows \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor0/*.o \
	$(library)/visitor1/gui.o \
	-Wl,--no-undefined -static -static-libgcc \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid
clean:
	rm -f *.o *.a *.so
