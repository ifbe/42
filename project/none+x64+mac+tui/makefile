library?=../../library
flag="-c -O2 -fno-stack-protector -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s a
	make -s dylib
	make -s program
libboot0:
	make -s -C $(library)/libboot0 none \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
libboot1:
	make -s -C $(library)/libboot1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
libhard0:
	make -s -C $(library)/libhard0 x64 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
libhard1:
	make -s -C $(library)/libhard1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector"
libsoft0:
	make -s -C $(library)/libsoft0 mac \
	compiler="clang" cflag="-c -O2 -fno-stack-protector -I.."
libsoft1:
	make -s -C $(library)/libsoft1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector -I.."
libuser0:
	make -s -C $(library)/libuser0 mactui \
	compiler="clang" cflag="-c -O2 -fno-stack-protector -I.."
libuser1:
	make -s -C $(library)/libuser1 \
	compiler="clang" cflag="-c -O2 -fno-stack-protector -I.."
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
	libtool -dynamic -macosx_version_min 10.13 \
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
	$(library)/libuser1/*.o \
	-lm
clean:
	rm -f *.o *.a *.dylib
