project?=$(shell pwd)
library?=../../library
CC=clang
CF="-c -O2 -fno-stack-protector -Wno-address-of-packed-member -I../lib1"
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s exe
	make -s lib
	cp a.out ../..
.PHONY:libboot0
libboot0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) none
.PHONY:libboot1
libboot1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
.PHONY:libhard0
libhard0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) macx64
.PHONY:libhard1
libhard1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
.PHONY:libsoft0
libsoft0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) mac
.PHONY:libsoft1
libsoft1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
.PHONY:libuser0
libuser0:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) macglfw
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
lib:
	rm -f libboot0/entry.o
	libtool -static -o 42.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o 2>/dev/null
dylib:
	libtool -dynamic -macosx_version_min 11.0 -o 42.dylib \
	$(library)/libboot0/*.o $(library)/libboot1/*.o \
	$(library)/libhard0/*.o $(library)/libhard1/*.o \
	$(library)/libsoft0/*.o $(library)/libsoft1/*.o \
	$(library)/libuser0/*.o $(library)/libuser1/*.o \
	-lGLEW -lglfw -lc -lm -framework OpenGL -framework AudioToolbox
exe:
	clang \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-lGLEW -lglfw -framework OpenGL \
	-framework AVFoundation -framework CoreVideo -framework CoreMedia \
	-framework AudioToolbox \
	-framework Foundation -lm
clean:
	rm -f */*.o
	rm -f *.exe *.out *.o *.a *.dylib
	make -C $(library) clean
