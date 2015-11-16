.PHONY:linux
linux:
	gcc -c -fPIC -O2 world.c
	gcc -c -fPIC -O2 linux/file.c
	gcc -c -fPIC -O2 linux/arg.c

.PHONY:win
win:
	gcc -c -O2 world.c
	gcc -c -O2 win/file.c
	gcc -c -O2 win/process.c
	gcc -c -O2 win/arg.c
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res

.PHONY:mac
mac:
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	clang -c -O2 mac/beforemain.c
clean:
	rm -f *.o *.a *.res
