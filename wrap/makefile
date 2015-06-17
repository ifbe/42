all:
	echo "what?"
.PHONY:linux
linux:
	gcc -c -fPIC -O2 memory/memory.c
	gcc -c -fPIC -O2 memory/log.c
	gcc -c -fPIC -O2 linux/disk.c
	gcc -c -fPIC -O2 console/beforemain.c
	gcc -o ../a.out \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	memory.o log.o disk.o beforemain.o
.PHONY:linux+sdl
linux+sdl:
	gcc -c -fPIC -O2 memory/memory.c
	gcc -c -fPIC -O2 memory/journal.c
	gcc -c -fPIC -O2 sdl2/sdl.c
	gcc -c -fPIC -O2 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE linux/disk.c
	gcc -c -fPIC -O2 console/beforemain.c
	gcc -o ../a.out \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	memory.o journal.o sdl2.o disk.o beforemain.o \
	-lSDL2 -lm
.PHONY:mac+sdl
mac+sdl:
	clang -c -O2 memory/memory.c
	clang -c -O2 memory/journal.c
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	clang -c -O2 mac/beforemain.c
	clang -o ../a.out \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	memory.o journal.o sdl.o beforemain.o \
	-L /usr/local/Cellar/sdl2/2.0.3/lib \
	-lSDL2 -lm
.PHONY:win
win:
	gcc -c -O2 memory/memory.c
	gcc -c -O2 memory/log.c
	gcc -c -O2 win/disk.c
	gcc -c -O2 win/process.c
	gcc -c -O2 console/beforemain.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -o ../a.exe \
	../main/main.a \
	../wrap2/wrap2.a \
	../wrap1/wrap1.a \
	memory.o log.o disk.o process.o beforemain.o uac.res
.PHONY:win+api
win+api:
	gcc -c -O2 memory/memory.c
	gcc -c -O2 memory/journal.c
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
	gcc -c -O2 memory/memory.c
	gcc -c -O2 memory/journal.c
	gcc -c -O2 sdl/sdl.c
	gcc -c -O2 win/disk.c
	gcc -c -O2 win/process.c
	gcc -c -O2 win/beforemain.c
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
