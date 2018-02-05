compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) bare/bare.c

.PHONY:none
none:
	rm -f *.o
	$(compiler) $(cflag) none/none.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	$(compiler) $(cflag) wincmd/wincmd.c

.PHONY:efi
efi:
	rm -f *.o
	$(compiler) $(cflag) efi/efi.c

clean:
	rm -f *.o
