compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) init/init.c
	$(compiler) $(cflag) init/args.c
	$(compiler) $(cflag) bare/memory.c
	$(compiler) $(cflag) bare/board.c
	$(compiler) $(cflag) bare/barex64/baremain.c
	$(compiler) $(cflag) bare/barex64/serial.c
	$(compiler) $(cflag) bare/barex64/thread.c

.PHONY:efi
efi:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) init/init.c
	$(compiler) $(cflag) init/args.c
	$(compiler) $(cflag) bare/memory.c
	$(compiler) $(cflag) bare/board.c
	$(compiler) $(cflag) bare/efi/efimain.c
	$(compiler) $(cflag) bare/efi/serial.c
	$(compiler) $(cflag) bare/efi/thread.c

.PHONY:none
none:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) init/init.c
	$(compiler) $(cflag) init/args.c
	$(compiler) $(cflag) app/memory.c
	$(compiler) $(cflag) app/board.c
	$(compiler) $(cflag) app/none/main.c
	$(compiler) $(cflag) app/none/envev.c
	$(compiler) $(cflag) app/none/serial.c
	$(compiler) $(cflag) app/none/thread.c

.PHONY:rpi
rpi:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) init/init.c
	$(compiler) $(cflag) init/args.c
	$(compiler) $(cflag) app/memory.c
	$(compiler) $(cflag) app/rpi/wiringpi.c
	$(compiler) $(cflag) app/none/main.c
	$(compiler) $(cflag) app/none/envev.c
	$(compiler) $(cflag) app/none/serial.c
	$(compiler) $(cflag) app/none/thread.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	$(compiler) $(cflag) pwrclk.c
	$(compiler) $(cflag) init/init.c
	$(compiler) $(cflag) init/args.c
	$(compiler) $(cflag) app/memory.c
	$(compiler) $(cflag) app/board.c
	$(compiler) $(cflag) app/win/main.c
	$(compiler) $(cflag) app/win/envev.c
	$(compiler) $(cflag) app/win/serial.c
	$(compiler) $(cflag) app/win/thread.c

clean:
	rm -f *.o
