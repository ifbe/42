.PHONY:linux
linux:
	gcc -c -fPIC -O2 memory.c
	gcc -c -fPIC -O2 stdio/0.listen.c
	gcc -c -fPIC -O2 stdio/1.say.c
	gcc -c -fPIC -O2 stdio/2.diary.c
	gcc -c -fPIC -O2 linux/file.c
	gcc -c -fPIC -O2 linux/arg.c

.PHONY:win
win:
	gcc -c -O2 memory.c
	gcc -c -O2 stdio/0.listen.c
	gcc -c -O2 stdio/1.say.c
	gcc -c -O2 stdio/2.diary.c
	gcc -c -O2 win/file.c
	gcc -c -O2 win/process.c
	gcc -c -O2 win/arg.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res

.PHONY:mac
mac:
	clang -c -O2 memory.c
	clang -c -O2 stdio/0.listen.c
	clang -c -O2 stdio/1.say.c
	clang -c -O2 stdio/2.diary.c
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
clean:
	rm -f *.o *.a *.res
