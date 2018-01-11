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
	make -s -C $(library)/libuser0 maccli \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
	make -s -C $(library)/libuser1 \
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
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o
dylib:
	libtool -dynamic -macosx_version_min 10.12 \
	-o 42.dylib \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-lm -lc
program:
	clang \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o
clean:
	rm -f *.o *.a *.dylib
