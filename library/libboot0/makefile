compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) bare/print.c
	$(compiler) $(cflag) basic.c

.PHONY:none
none:
	rm -f *.o
	$(compiler) $(cflag) none/tx.c
	$(compiler) $(cflag) basic.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	$(compiler) $(cflag) wincmd/wincmd.c
	$(compiler) $(cflag) basic.c

.PHONY:efi
efi:
	rm -f *.o
	$(compiler) $(cflag) basic.c

clean:
	rm -f *.o
