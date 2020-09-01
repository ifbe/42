CC ?= gcc
CF ?= -c -O2 -fPIC

.PHONY:rpi
rpi:
	rm -f *.o
	$(CC) $(CF) origin.c
	$(CC) $(CF) bare/initer.c
	$(CC) $(CF) bare/memory.c
	$(AS) -c bare/rpi/entry.s -o entry.o
	$(CC) $(CF) bare/rpi/main.c
	$(CC) $(CF) bare/rpi/runenv.c
	$(CC) $(CF) bare/rpi/serial.c
	$(CC) $(CF) bare/rpi/thread.c

.PHONY:bare
bare:
	rm -f *.o
	$(CC) $(CF) origin.c
	$(CC) $(CF) bare/initer.c
	$(CC) $(CF) bare/memory.c
	$(CC) $(CF) bare/barex64/entry.c
	$(CC) $(CF) bare/barex64/keyboard.c
	$(CC) $(CF) bare/barex64/runenv.c
	$(CC) $(CF) bare/barex64/serial.c
	$(CC) $(CF) bare/barex64/thread.c

.PHONY:efios
efios:
	rm -f *.o
	$(CC) $(CF) origin.c
	$(CC) $(CF) bare/initer.c
	$(CC) $(CF) bare/memory.c
	$(CC) $(CF) bare/efi/entry.c
	$(CC) $(CF) bare/efi/keyboard.c
	$(CC) $(CF) bare/efi/runenv.c
	$(CC) $(CF) bare/efi/serial.c
	$(CC) $(CF) bare/efi/thread.c

.PHONY:efiapp
efiapp:
	rm -f *.o
	$(CC) $(CF) origin.c
	$(CC) $(CF) bare/initer.c
	$(CC) $(CF) bare/memory.c
	$(CC) $(CF) app/efi/entry.c
	$(CC) $(CF) app/efi/runenv.c
	$(CC) $(CF) app/efi/thread.c

.PHONY:none
none:
	rm -f *.o
	$(CC) $(CF) origin.c
	$(CC) $(CF) app/initer.c
	$(CC) $(CF) app/memory.c
	$(CC) $(CF) app/none/entry.c
	$(CC) $(CF) app/none/runenv.c
	$(CC) $(CF) app/none/serial.c
	$(CC) $(CF) app/none/thread.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	$(CC) $(CF) origin.c
	$(CC) $(CF) app/initer.c
	$(CC) $(CF) app/memory.c
	$(CC) $(CF) app/win/entry.c
	$(CC) $(CF) app/win/runenv.c
	$(CC) $(CF) app/win/serial.c
	$(CC) $(CF) app/win/thread.c

clean:
	rm -f *.o
