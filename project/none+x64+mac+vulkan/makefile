MACSDK=~/VulkanSDK/1.3.268.1/macOS
project?=$(shell pwd)
library?=../../library
CC=clang
CF="-c -O2 -fno-stack-protector -Wno-address-of-packed-member -I../lib1"
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1 shader
	make -s exe
	make -s lib
	make -s dylib
	install_name_tool -add_rpath ${MACSDK}/lib a.out
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
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF) macvulkan
.PHONY:libuser1
libuser1:
	mkdir -p $@
	make -s -C $(library)/$@ DIR=$(project)/$@ CC=$(CC) CF=$(CF)
shader:
	mkdir -p $@
lib:
	rm -f libboot0/entry.o
	libtool -static -o 42.a \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o 2>/dev/null
dylib:
	clang -dynamiclib -o 42.dylib \
	-current_version 1.0 -compatibility_version 1.0 -fvisibility=hidden \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-framework AVFoundation -framework CoreVideo -framework CoreMedia \
	-framework Foundation \
	-framework AudioToolbox \
	-lvulkan -lglfw -lc -lm
exe:
	clang \
	libboot0/*.o libboot1/*.o \
	libhard0/*.o libhard1/*.o \
	libsoft0/*.o libsoft1/*.o \
	libuser0/*.o libuser1/*.o \
	-framework AVFoundation -framework CoreVideo -framework CoreMedia \
	-framework AudioToolbox \
	-framework Foundation \
	-lvulkan -lglfw -lm
clean:
	rm -f */*.o
	rm -f *.exe *.out *.o *.a *.dylib
