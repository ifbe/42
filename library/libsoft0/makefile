.PHONY:linux
linux:
	rm -f win.*.o mac.*.o
	gcc -c -O2 linux/linux.event.c
	gcc -c -O2 linux/linux.file.c
	gcc -c -O2 linux/linux.random.c
	gcc -c -O2 memory.c

.PHONY:win
win:
	rm -f linux.*.o mac.*.o
	gcc -c -O2 win/win.event.c
	gcc -c -O2 win/win.file.c
	gcc -c -O2 win/win.process.c
	gcc -c -O2 win/win.random.c
	gcc -c -O2 memory.c

.PHONY:mac
mac:
	rm -f linux.*.o win.*.o
	clang -c -O2 mac/mac.event.c
	clang -c -O2 mac/mac.file.c
	clang -c -O2 mac/mac.random.c
	clang -c -O2 memory.c
clean:
	rm -f *.o *.a *.res
