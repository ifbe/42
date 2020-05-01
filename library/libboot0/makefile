compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) origin.c
	$(compiler) $(cflag) bare/memory.c
	$(compiler) $(cflag) bare/barex64/entry.c
	$(compiler) $(cflag) bare/barex64/serial.c
	$(compiler) $(cflag) bare/barex64/thread.c

.PHONY:efi
efi:
	rm -f *.o
	$(compiler) $(cflag) origin.c
	$(compiler) $(cflag) bare/memory.c
	$(compiler) $(cflag) bare/efi/entry.c
	$(compiler) $(cflag) bare/efi/serial.c
	$(compiler) $(cflag) bare/efi/thread.c

.PHONY:none
none:
	rm -f *.o
	$(compiler) $(cflag) origin.c
	$(compiler) $(cflag) app/memory.c
	$(compiler) $(cflag) app/none/entry.c
	$(compiler) $(cflag) app/none/envev.c
	$(compiler) $(cflag) app/none/serial.c
	$(compiler) $(cflag) app/none/thread.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	$(compiler) $(cflag) origin.c
	$(compiler) $(cflag) app/memory.c
	$(compiler) $(cflag) app/win/entry.c
	$(compiler) $(cflag) app/win/envev.c
	$(compiler) $(cflag) app/win/serial.c
	$(compiler) $(cflag) app/win/thread.c

clean:
	rm -f *.o
