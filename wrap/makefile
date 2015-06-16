.PHONY:linux+console
linux+console:
	gcc -c -O2 win/memory.c
	gcc -c -O2 win/log.c
	gcc -c -O2 win/disk.c
	gcc -c -fPIC -O2 linux/beforemain.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -o ../a.exe \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	memory.o log.o disk.o uac.res
.PHONY:linux
linux:
	gcc -c -fPIC -O2 linux/memory.c
	gcc -c -fPIC -O2 linux/journal.c
	gcc -c -fPIC -O2 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE linux/disk.c
	gcc -c -fPIC -O2 linux/beforemain.c
	gcc -o ../a.out \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	memory.o journal.o disk.o beforemain.o \
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
.PHONY:win+console
win+console:
	gcc -c -O2 win/memory.c
	gcc -c -O2 win/log.c
	gcc -c -O2 win/disk.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -o ../a.exe \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	memory.o log.o disk.o uac.res
.PHONY:win+api
win+api:
	gcc -c -O2 win/memory.c
	gcc -c -O2 win/journal.c
	gcc -c -O2 win/window.c
	gcc -c -O2 win/disk.c
	gcc -c -O2 win/process.c
	gcc -c -O2 win/beforemain.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -mwindows -o ../a.exe \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	memory.o journal.o window.o disk.o process.o beforemain.o uac.res \
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
