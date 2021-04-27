project=$(shell pwd)
library=../../library
CC=sdcc
CF=

all:libboot0
	packihx test.ihx > test.hex
	hex2bin test.hex
flash:
	python3 stcflash.py test.bin
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) 8051
clean:
	rm -rf lib*/*.o
	rm -f *.o *.EFI
	make -C $(library) clean
