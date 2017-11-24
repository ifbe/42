library?=../../library
res?=../../res
all:
	make -s -C $(library)/libboot0 none \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libboot1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libhard0 x64 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libhard1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libsoft0 mac \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libsoft1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libui0 maccocoa \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libui1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s a
	make -s dylib
	make -s program
a:
	libtool -static -o 42.a \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o
dylib:
	libtool -dynamic -macosx_version_min 10.12 \
	-o 42.dylib \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-lc -framework Cocoa
program:
	clang -framework Foundation \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-framework Cocoa
clean:
	rm -f *.o *.a *.dylib
