project?=$(shell pwd)
library?=../../library
CC=gcc
CF="-c -O2 -fPIC -Wno-address-of-packed-member -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -I../lib1"
CPPC=g++
CPPF="-std=c++20 -c -O2 -fPIC -Wno-address-of-packed-member -I../lib1 -I/usr/include/libcamera"
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	+make -s a
	+make -s so
	+make -s program
	cp a.out ../..
.PHONY:libboot0
libboot0:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF) none
.PHONY:libboot1
libboot1:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF)
.PHONY:libhard0
libhard0:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF) linuxpi32
.PHONY:libhard1
libhard1:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF)
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF) linux
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF)
.PHONY:libuser0
libuser0:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF) pilinuxcli
.PHONY:libuser1
libuser1:
	mkdir -p $@
	+make -s -C $(library)/$@ DIR=$(project)/$@ CF=$(CF) CPPF=$(CPPF)
a:
	ar crs 42.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o
so:
	gcc -shared -o 42.so \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o
program:
	gcc -o a.out \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lcamera -lcamera-base -lstdc++ \
	-lpulse -lpulse-simple -lpthread -lm
	#-lasound -lpthread -lm
clean:
	make -C $(library) clean
	rm -f *.o *.a *.so
	rm -f */*.o
