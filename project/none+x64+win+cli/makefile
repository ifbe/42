library?=../../library
res?=../../res
all:
	make -s -C $(library)/libboot0 wincmd \
	cflag="-c -O2"
	make -s -C $(library)/libboot1 \
	cflag="-c -O2"
	make -s -C $(library)/libhard0 winx64 \
	cflag="-c -O2"
	make -s -C $(library)/libhard1 \
	cflag="-c -O2"
	make -s -C $(library)/libsoft0 win \
	cflag="-c -O2"
	make -s -C $(library)/libsoft1 \
	cflag="-c -O2"
	make -s -C $(library)/libui0 wincli \
	cflag="-c -O2"
	make -s -C $(library)/libui1 \
	cflag="-c -O2"
	make -s -C $(library)/visitor0 \
	cflag="-c -O2"
	make -s -C $(library)/visitor1 cli \
	cflag="-c -O2"
	make -s a
	make -s dll
	make -s program
a:
	ar crs 42.a \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor0/*.o
dll:
	gcc -shared -o 42.dll \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor0/*.o \
	-lws2_32
program:
	gcc \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	$(library)/visitor0/*.o \
	$(library)/visitor1/cli.o \
	-lwinmm -lws2_32
clean:
	rm -f *.o *.a *.so
