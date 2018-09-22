compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) bare/start.c
	$(compiler) $(cflag) bare/serial.c
	$(compiler) $(cflag) bare/memory.c
	$(compiler) $(cflag) bare/thread.c

.PHONY:efi
efi:
	rm -f *.o
	$(compiler) $(cflag) efi/start.c
	$(compiler) $(cflag) efi/serial.c
	$(compiler) $(cflag) efi/memory.c
	$(compiler) $(cflag) efi/thread.c

.PHONY:none
none:
	rm -f *.o
	$(compiler) $(cflag) none/start.c
	$(compiler) $(cflag) none/serial.c
	$(compiler) $(cflag) none/memory.c
	$(compiler) $(cflag) none/thread.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	$(compiler) $(cflag) wincmd/start.c
	$(compiler) $(cflag) wincmd/serial.c
	$(compiler) $(cflag) wincmd/memory.c
	$(compiler) $(cflag) wincmd/thread.c

clean:
	rm -f *.o
