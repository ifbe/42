.PHONY:bsd
bsd:
	rm -f mac.*.o win.*.o
	gcc -c -fPIC -O2 bsd/bsd.event.c
	gcc -c -fPIC -O2 bsd/bsd.file.c
	gcc -c -fPIC -O2 bsd/bsd.memory.c
	gcc -c -fPIC -O2 bsd/bsd.random.c
	gcc -c -fPIC -O2 system.c
.PHONY:linux
linux:
	rm -f mac.*.o win.*.o
	gcc -c -fPIC -O2 linux/linux.event.fake.c
	gcc -c -fPIC -O2 linux/linux.event.net.c
	gcc -c -fPIC -O2 linux/linux.flatmem.file.c
	gcc -c -fPIC -O2 linux/linux.random.c
	gcc -c -fPIC -O2 system.c
.PHONY:mac
mac:
	rm -f linux.*.o win.*.o
	clang -c -O2 mac/mac.event.c
	clang -c -O2 mac/mac.file.c
	clang -c -O2 mac/mac.memory.c
	clang -c -O2 mac/mac.random.c
	clang -c -O2 system.c
.PHONY:win
win:
	rm -f linux.*.o mac.*.o
	gcc -c -O2 win/win.event.c
	gcc -c -O2 win/win.file.c
	gcc -c -O2 win/win.memory.c
	gcc -c -O2 win/win.process.c
	gcc -c -O2 win/win.random.c
	gcc -c -O2 system.c
clean:
	rm -f *.o *.a
