datafile?=../../datafile
library?=../../library
project?=$(shell pwd)
CC=gcc
CF="-c -O2 -I../lib1"
CXX=g++
CXF="-c -O2 -fno-rtti -I../lib1"
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s exe
	make -s lib
	make -s dll
	cp a.exe ../..
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CXF=$(CXF) wincmd
.PHONY:libboot1
libboot1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CXF=$(CXF)
.PHONY:libhard0
libhard0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CXF=$(CXF) winx64
.PHONY:libhard1
libhard1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CXF=$(CXF)
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CXF=$(CXF) win
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CXF=$(CXF)
.PHONY:libuser0
libuser0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CXF=$(CXF) winvulkan
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CXF=$(CXF)
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
	-lglfw3 -lvulkan-1 \
	-lole32 -lstrmiids -lxinput -lwinmm -lws2_32 -lgdi32
exe:
	windres --input-format=rc -O coff -i $(datafile)/winres/42.rc -o $(datafile)/winres/42.res
	g++ -mwindows $(datafile)/winres/42.res \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lglfw3 -lvulkan-1 \
	-lole32 -lstrmiids -lxinput -lwinmm -lws2_32
clean:
	rm -f *.o *.a
	rm -f */*.o
	make -C $(library) clean
