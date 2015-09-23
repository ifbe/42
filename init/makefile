what:
	@echo "make what?"
.PHONY:linux
linux:
	gcc -c -fPIC -O2 soft/world.c
	gcc -c -fPIC -O2 soft/0.event.c
	gcc -c -fPIC -O2 soft/1.say.c
	gcc -c -fPIC -O2 soft/2.diary.c
	gcc -c -fPIC -O2 soft/3.operator.c
	gcc -c -fPIC -O2 soft/linux/file.c
	gcc -c -fPIC -O2 soft/linux/arg.c
	gcc -c -fPIC -O2 ui/cli.c
	rm -f init.a
	ar crs init.a \
	world.o 0.event.o 1.say.o 2.diary.o 3.operator.o \
	file.o arg.o \
	cli.o
.PHONY:win
win:
	gcc -c -O2 soft/world.c
	gcc -c -O2 soft/0.event.c
	gcc -c -O2 soft/1.say.c
	gcc -c -O2 soft/2.diary.c
	gcc -c -O2 soft/3.operator.c
	windres --input-format=rc -O coff -i soft/win/uac.rc -o uac.res
	gcc -c -O2 soft/win/file.c
	gcc -c -O2 soft/win/process.c
	gcc -c -O2 soft/win/arg.c
	gcc -c -O2 ui/cli.c
	rm -f init.a
	ar crs init.a \
	world.o 0.event.o 1.say.o 2.diary.o 3.operator.o \
	file.o process.o arg.o \
	cli.o
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
	gcc -c -fPIC -O2 soft/world.c
	gcc -c -fPIC -O2 soft/0.event.c
	gcc -c -fPIC -O2 soft/1.say.c
	gcc -c -fPIC -O2 soft/2.diary.c
	gcc -c -fPIC -O2 soft/3.operator.c
	gcc -c -fPIC -O2 soft/linux/file.c
	gcc -c -fPIC -O2 soft/linux/arg.c
	gcc -c -fPIC -O2 ui/screen.c
	gcc -c -fPIC -O2 ui/linux/xlib.c
	rm -f init.a
	ar crs init.a \
	world.o 0.event.o 1.say.o 2.diary.o 3.operator.o \
	file.o arg.o \
	screen.o xlib.o
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
	gcc -c -O2 soft/world.c
	gcc -c -O2 soft/0.event.c
	gcc -c -O2 soft/1.say.c
	gcc -c -O2 soft/2.diary.c
	gcc -c -O2 soft/3.operator.c
	windres --input-format=rc -O coff -i soft/win/uac.rc -o uac.res
	gcc -c -O2 soft/win/file.c
	gcc -c -O2 soft/win/process.c
	gcc -c -O2 soft/win/arg.c
	gcc -c -O2 ui/screen.c
	gcc -c -O2 ui/win/winapi.c
	rm -f init.a
	ar crs init.a \
	world.o 0.event.o 1.say.o 2.diary.o 3.operator.o \
	file.o process.o arg.o \
	screen.o winapi.o
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
