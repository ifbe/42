.PHONY:binary
binary:
	gcc -c -fPIC -O2 bare/start.c
	gcc -c -fPIC -O2 bare/screen.c
	gcc -c -fPIC -O2 bare/random.c
	gcc -c -fPIC -O2 bare/input.c
	rm -f wrap.a
	ar crs wrap.a screen.o random.o input.o
.PHONY:crosscompile
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 bare/start.c
	x86_64-elf-gcc -c -fPIC -O2 bare/screen.c
	x86_64-elf-gcc -c -fPIC -O2 bare/random.c
	x86_64-elf-gcc -c -fPIC -O2 bare/input.c
	rm -f wrap.a
	ar crs wrap.a screen.o random.o input.o
.PHONY:linux
linux:
	gcc -c -fPIC -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -O2 linux/disk.c
	gcc -c -fPIC -O2 linux/log.c
	gcc -c -fPIC -O2 linux/memory.c
	rm -f wrap.a
	ar crs wrap.a disk.o log.o memory.o
.PHONY:mac
mac:
	clang -c system/screen.c
	clang -c system/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	rm -f wrap.a
	ar crs wrap.a screen.o sdl.o
.PHONY:win
win:
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -c -O2 win/disk.c
	gcc -c -O2 win/earmouth.c
	gcc -c -O2 win/memory.c
	rm -f wrap.a
	ar crs wrap.a disk.o earmouth.o memory.o
.PHONY:win-gui
win-gui:
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -c -O2 win/disk.c
	gcc -c -O2 win-gui/2d.c
	gcc -c -O2 win-gui/earmouth.c
	gcc -c -O2 win-gui/memory.c
	gcc -c -O2 win-gui/sdl.c -I /opt/SDL2-2.0.3/x86_64-w64-mingw32/include
	rm -f wrap.a
	ar crs wrap.a disk.o 2d.o earmouth.o memory.o sdl.o
clean:
	rm -f *.o *.a *.res
