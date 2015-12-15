.PHONY:linux
linux:
	rm -f *.o
	gcc -c -O2 linux/linux.arg.c
	gcc -c -O2 linux/linux.file.c
	gcc -c -O2 memory.c

.PHONY:win
win:
	rm -f *.o
	windres --input-format=rc -O coff -i win/uac.rc -o uac.res
	gcc -c -O2 win/win.arg.c
	gcc -c -O2 win/win.file.c
	gcc -c -O2 win/win.process.c
	gcc -c -O2 memory.c

.PHONY:mac
mac:
	rm -f *.o
	clang -c -O2 memory.c
clean:
	rm -f *.o *.a *.res
