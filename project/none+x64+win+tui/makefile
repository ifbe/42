project?=$(shell pwd)
library?=../../library
datafile?=../../datafile
CC=gcc
CF="-c -O2 -I../lib1"
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s exe
	make -s lib
	make -s dll
	cp a.exe ../..
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) wincmd
.PHONY:libboot1
libboot1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF)
.PHONY:libhard0
libhard0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) winx64
.PHONY:libhard1
libhard1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF)
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) win
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF)
.PHONY:libuser0
libuser0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) wintui
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF)
lib:
	rm -f libboot0/entry.o
	ar crs 42.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o
dll:
	g++ -shared -o 42.dll \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lole32 -lstrmiids -lxinput -lwinmm -lws2_32
exe:
	g++ \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lole32 -lstrmiids -lxinput -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.so
	make -C $(library) clean
