compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) bare/start.c
	$(compiler) $(cflag) bare/serial.c
	$(compiler) $(cflag) bare/memory.c
	$(compiler) $(cflag) bare/thread.c
	$(compiler) $(cflag) none/board.c

.PHONY:efi
efi:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) efi/start.c
	$(compiler) $(cflag) efi/serial.c
	$(compiler) $(cflag) efi/memory.c
	$(compiler) $(cflag) efi/thread.c
	$(compiler) $(cflag) none/board.c

.PHONY:none
none:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) none/start.c
	$(compiler) $(cflag) none/serial.c
	$(compiler) $(cflag) none/memory.c
	$(compiler) $(cflag) none/thread.c
	$(compiler) $(cflag) none/board.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) wincmd/start.c
	$(compiler) $(cflag) wincmd/serial.c
	$(compiler) $(cflag) wincmd/memory.c
	$(compiler) $(cflag) wincmd/thread.c
	$(compiler) $(cflag) none/board.c

.PHONY:rpi
rpi:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) none/start.c
	$(compiler) $(cflag) none/serial.c
	$(compiler) $(cflag) none/memory.c
	$(compiler) $(cflag) none/thread.c
	$(compiler) $(cflag) rpi/wiringpi.c

clean:
	rm -f *.o
