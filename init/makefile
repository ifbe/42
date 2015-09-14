all:
	echo "what?"
.PHONY:linux
linux:
	gcc -c -fPIC -O2 linux/base/disk.c
	gcc -c -fPIC -O2 linux/base/arg.c
	gcc -c -fPIC -O2 linux/cli/init.c
	gcc -c -fPIC -O2 linux/cli/log.c
	rm -f init.a
	ar crs init.a disk.o arg.o init.o log.o
.PHONY:win
win:
	windres --input-format=rc -O coff -i win/base/uac.rc -o uac.res
	gcc -c -O2 win/base/commandline.c
	gcc -c -O2 win/base/disk.c
	gcc -c -O2 win/base/mem2file.c
	gcc -c -O2 win/base/process.c
	gcc -c -O2 win/cli/init.c
	gcc -c -O2 win/cli/log.c
	rm -f init.a
	ar crs init.a commandline.o disk.o mem2file.o process.o init.o log.o
.PHONY:linux+fb
linux+fb:
	gcc -c -fPIC -O2 memory/memory.c
	gcc -c -fPIC -O2 memory/journal.c
	gcc -c -fPIC -O2 linux/fb.c
	gcc -c -fPIC -O2 linux/disk.c
	gcc -c -fPIC -O2 linux/beforemain.c
	rm -f init.a
	ar crs init.a memory.o journal.o fb.o disk.o beforemain.o
.PHONY:linux+xlib
linux+xlib:
	gcc -c -fPIC -O2 linux/base/disk.c
	gcc -c -fPIC -O2 linux/base/arg.c
	gcc -c -fPIC -O2 linux/gui/init.c
	gcc -c -fPIC -O2 linux/gui/journal.c
	gcc -c -fPIC -O2 linux/gui/xlib.c
	rm -f init.a
	ar crs init.a disk.o arg.o init.o journal.o xlib.o
.PHONY:linux+sdl
linux+sdl:
	gcc -c -fPIC -O2 memory/memory.c
	gcc -c -fPIC -O2 memory/journal.c
	gcc -c -fPIC -O2 sdl2/sdl.c
	gcc -c -fPIC -O2 linux/disk.c
	gcc -c -fPIC -O2 linux/beforemain.c
	rm -f init.a
	ar crs init.a memory.o journal.o sdl2.o disk.o beforemain.o
.PHONY:mac+sdl
mac+sdl:
	clang -c -O2 memory/memory.c
	clang -c -O2 memory/journal.c
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	clang -c -O2 mac/beforemain.c
	rm -f init.a
	ar crs init.a memory.o journal.o sdl.o beforemain.o
.PHONY:win+api
win+api:
	windres --input-format=rc -O coff -i win/base/uac.rc -o uac.res
	gcc -c -O2 win/base/commandline.c
	gcc -c -O2 win/base/mem2file.c
	gcc -c -O2 win/base/disk.c
	gcc -c -O2 win/base/process.c
	gcc -c -O2 win/gui/init.c
	gcc -c -O2 win/gui/journal.c
	gcc -c -O2 win/gui/api.c
	rm -f init.a
	ar crs init.a commandline.o mem2file.o disk.o process.o init.o journal.o api.o
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
	rm -f init.a
	ar crs init.a disk.o earmouth.o memory.o sdl.o
clean:
	rm -f *.o *.a *.res
