compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) basic.c
	$(compiler) $(cflag) bare/print.c

.PHONY:none
none:
	rm -f *.o
	$(compiler) $(cflag) startapp.c
	$(compiler) $(cflag) basic.c
	$(compiler) $(cflag) none/tx.c

.PHONY:wincmd
wincmd:
	rm -f *.o
	$(compiler) $(cflag) startapp.c
	$(compiler) $(cflag) basic.c
	$(compiler) $(cflag) wincmd/wincmd.c

.PHONY:efi
efi:
	rm -f *.o
	$(compiler) $(cflag) startefi.c
	$(compiler) $(cflag) basic.c

clean:
	rm -f *.o
