what:
	@echo "0,1,2,3:    beautiful interface for upper layer"
	@echo "linux/win/mac:    specific implementation"
.PHONY:linux
linux:
	gcc -c -fPIC -O2 0.event.c
	gcc -c -fPIC -O2 1.say.c
	gcc -c -fPIC -O2 2.diary.c
	gcc -c -fPIC -O2 3.operator.c
	gcc -c -fPIC -O2 linux/file.c
	gcc -c -fPIC -O2 linux/arg.c
.PHONY:win
win:
	gcc -c -O2 0.event.c
	gcc -c -O2 1.say.c
	gcc -c -O2 2.diary.c
	gcc -c -O2 3.operator.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -c -O2 win/file.c
	gcc -c -O2 win/process.c
	gcc -c -O2 win/arg.c
.PHONY:mac
mac:
	clang -c -O2 memory/memory.c
	clang -c -O2 memory/journal.c
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	clang -c -O2 mac/beforemain.c
clean:
	rm -f *.o *.a *.res
