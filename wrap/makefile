.PHONY:binary
binary:
	gcc -c -fPIC -O2 bare/start.c
	gcc -c -fPIC -O2 bare/screen.c
	gcc -c -fPIC -O2 bare/random.c
	gcc -c -fPIC -O2 bare/input.c
	ld -T link.ld -o a.bin wrap/start.o \
	../main/main.a \
	screen.o random.o input.o
.PHONY:crosscompile
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 bare/start.c
	x86_64-elf-gcc -c -fPIC -O2 bare/screen.c
	x86_64-elf-gcc -c -fPIC -O2 bare/random.c
	x86_64-elf-gcc -c -fPIC -O2 bare/input.c
	x86_64-elf-ld -T link.ld -o a.bin wrap/start.o \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	screen.o random.o input.o
.PHONY:linux
linux:
	gcc -c -fPIC -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -O2 linux/disk.c
	gcc -c -fPIC -O2 linux/log.c
	gcc -c -fPIC -O2 linux/memory.c
	gcc -o ../a.out \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	disk.o log.o memory.o \
	-lSDL2 -lm
.PHONY:mac
mac:
	clang -c system/screen.c
	clang -c system/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	gcc -o ../a.out \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	disk.o log.o memory.o \
	-L /usr/local/Cellar/sdl2/2.0.3/lib \
	-lSDL2 -lm
.PHONY:win
win:
	gcc -c -O2 win/disk.c
	gcc -c -O2 win/log.c
	gcc -c -O2 win/memory.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -o ../a.exe \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	disk.o memory.o log.o uac.res
.PHONY:win+api
win+api:
	gcc -c -O2 win/api.c
	gcc -c -O2 win/disk.c
	gcc -c -O2 win/journal.c
	gcc -c -O2 win/memory.c
	gcc -c -O2 win/process.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -mwindows -o ../a.exe \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	api.o disk.o journal.o memory.o process.o uac.res \
	-lgdi32
.PHONY:win+sdl
win+sdl:
	gcc -c -O2 win/disk.c
	gcc -c -O2 win/journal.c
	gcc -c -O2 win/memory.c
	gcc -c -O2 sdl2/sdl.c -I /opt/SDL2-2.0.3/x86_64-w64-mingw32/include
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -o ../a.exe \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	disk.o earmouth.o memory.o sdl.o uac.res \
	-Wl,--no-undefined -static -static-libgcc \
	-L /opt/SDL2-2.0.3/x86_64-w64-mingw32/lib \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid
clean:
	rm -f *.o *.a *.res
