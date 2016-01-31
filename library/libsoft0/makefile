.PHONY:linux
linux:
	rm -f win.*.o mac.*.o
	gcc -c -O2 linux/linux.arg.c
	gcc -c -O2 linux/linux.file.c
	gcc -c -O2 linux/linux.epoll.c
	gcc -c -O2 linux/linux.random.c
	gcc -c -O2 memory.c

.PHONY:win
win:
	rm -f linux.*.o mac.*.o
	gcc -c -O2 win/win.arg.c
	gcc -c -O2 win/win.file.c
	gcc -c -O2 win/win.iocp.c
	gcc -c -O2 win/win.random.c
	gcc -c -O2 win/win.process.c
	gcc -c -O2 memory.c

.PHONY:mac
mac:
	rm -f linux.*.o win.*.o
	clang -c -O2 mac/mac.kqueue.c
	clang -c -O2 memory.c
clean:
	rm -f *.o *.a *.res
