project?=$(shell pwd)
library?=../../library
datafile?=../../datafile
CC=gcc
CF="-c -O2 -Wno-address-of-packed-member -I../lib1"
CPPC=g++
CPPF="-c -O2 -fno-rtti -Wno-address-of-packed-member -I../lib1"
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s exe
	make -s lib
	make -s dll
	cp a.exe ../..
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF) wincmd
.PHONY:libboot1
libboot1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF)
.PHONY:libhard0
libhard0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF) winx64
.PHONY:libhard1
libhard1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF)
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF) win
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF)
.PHONY:libuser0
libuser0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF) wintui
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF)
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
	-lmf -lmfplat -lmfreadwrite -lmfuuid \
	-lole32 -lgdi32 -lstrmiids -lxinput -lwinmm -lws2_32
exe:
	g++ \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lmf -lmfplat -lmfreadwrite -lmfuuid \
	-lole32 -lgdi32 -lstrmiids -lxinput -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.so
	rm -f */*.o
