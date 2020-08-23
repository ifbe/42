compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) origin.c
	$(compiler) $(cflag) bare/initer.c
	$(compiler) $(cflag) bare/memory.c
	$(compiler) $(cflag) bare/barex64/entry.c
	$(compiler) $(cflag) bare/barex64/thread.c
	$(compiler) $(cflag) bare/barex64/keyboard.c
	$(compiler) $(cflag) bare/barex64/runenv.c
	$(compiler) $(cflag) bare/barex64/serial.c

.PHONY:efios
efios:
	rm -f *.o
	$(compiler) $(cflag) origin.c
	$(compiler) $(cflag) bare/initer.c
	$(compiler) $(cflag) bare/memory.c
	$(compiler) $(cflag) bare/efi/entry.c
	$(compiler) $(cflag) bare/efi/runenv.c
	$(compiler) $(cflag) bare/efi/serial.c
	$(compiler) $(cflag) bare/efi/thread.c

.PHONY:efiapp
efiapp:
	rm -f *.o
	$(compiler) $(cflag) origin.c
	$(compiler) $(cflag) bare/initer.c
	$(compiler) $(cflag) bare/memory.c
	$(compiler) $(cflag) app/efi/entry.c
	$(compiler) $(cflag) app/efi/runenv.c
	$(compiler) $(cflag) app/efi/thread.c

.PHONY:none
none:
	rm -f *.o
	$(compiler) $(cflag) origin.c
	$(compiler) $(cflag) app/initer.c
	$(compiler) $(cflag) app/memory.c
	$(compiler) $(cflag) app/none/entry.c
	$(compiler) $(cflag) app/none/runenv.c
	$(compiler) $(cflag) app/none/serial.c
	$(compiler) $(cflag) app/none/thread.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	$(compiler) $(cflag) origin.c
	$(compiler) $(cflag) app/initer.c
	$(compiler) $(cflag) app/memory.c
	$(compiler) $(cflag) app/win/entry.c
	$(compiler) $(cflag) app/win/runenv.c
	$(compiler) $(cflag) app/win/serial.c
	$(compiler) $(cflag) app/win/thread.c

clean:
	rm -f *.o
