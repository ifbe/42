project?=$(shell pwd)
library?=../../library
CC=x86_64-elf-gcc
CF="-c -O2 -fPIC -I.."
LD=x86_64-elf-ld
LF=-T link.ld
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s lib
	make -s exe
	cp a.out ../..
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) bareapp
.PHONY:libboot1
libboot1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
.PHONY:libhard0
libhard0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) os42x64
.PHONY:libhard1
libhard1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) os42
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
.PHONY:libuser0
libuser0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) myoscli
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
lib:
	rm -f libboot0/entry.o
	x86_64-elf-ar crs 42.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o 2>/dev/null
exe:
	$(CC) -c -O2 -fPIC -I.. start.c
	$(CC) -c -O2 -fPIC -I.. main.c
	$(LD) $(LF) start.o main.o 42.a
clean:
	rm -f */*.o
	rm -f *.exe *.out *.o *.a
	make -C $(library) clean
