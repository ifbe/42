.PHONY:linux
linux:
	gcc -c -O2 linux/linux.arg.c
	gcc -c -O2 linux/linux.file.c
	gcc -c -O2 stdio/0.listen.c
	gcc -c -O2 stdio/1.say.c
	gcc -c -O2 stdio/2.diary.c
	gcc -c -O2 stdio/3.history.c
	gcc -c -O2 memory.c

.PHONY:win
win:
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -c -O2 win/win.arg.c
	gcc -c -O2 win/win.file.c
	gcc -c -O2 win/win.process.c
	gcc -c -O2 stdio/0.listen.c
	gcc -c -O2 stdio/1.say.c
	gcc -c -O2 stdio/2.diary.c
	gcc -c -O2 stdio/3.history.c
	gcc -c -O2 memory.c

.PHONY:mac
mac:
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	clang -c -O2 stdio/0.listen.c
	clang -c -O2 stdio/1.say.c
	clang -c -O2 stdio/2.diary.c
	clang -c -O2 stdio/3.history.c
	clang -c -O2 memory.c
clean:
	rm -f *.o *.a *.res
