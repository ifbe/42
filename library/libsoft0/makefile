what:
	@echo "0,1,2,3,world:    beautiful interface for upper layer"
	@echo "linux/win/mac:    specific implementation"
.PHONY:linux
linux:
	gcc -c -fPIC -O2 world.c
	gcc -c -fPIC -O2 0.event.c
	gcc -c -fPIC -O2 1.say.c
	gcc -c -fPIC -O2 2.diary.c
	gcc -c -fPIC -O2 3.operator.c
	gcc -c -fPIC -O2 linux/file.c
	gcc -c -fPIC -O2 linux/arg.c
	rm -f libsoft0.a
	ar crs libsoft0.a \
	world.o 0.event.o 1.say.o 2.diary.o 3.operator.o \
	file.o arg.o
.PHONY:win
win:
	gcc -c -O2 world.c
	gcc -c -O2 0.event.c
	gcc -c -O2 1.say.c
	gcc -c -O2 2.diary.c
	gcc -c -O2 3.operator.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -c -O2 win/file.c
	gcc -c -O2 win/process.c
	gcc -c -O2 win/arg.c
	rm -f libsoft0.a
	ar crs libsoft0.a \
	world.o 0.event.o 1.say.o 2.diary.o 3.operator.o \
	file.o process.o arg.o
.PHONY:mac
mac:
	clang -c -O2 memory/memory.c
	clang -c -O2 memory/journal.c
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	clang -c -O2 mac/beforemain.c
	rm -f libsoft0.a
	ar crs libsoft0.a memory.o journal.o sdl.o beforemain.o
clean:
	rm -f *.o *.a *.res
