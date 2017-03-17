library?=../../library
datafile?=../../datafile
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
	make -s -C $(library)/visitor \
	cflag="-c -O2"
	make -s a
	make -s dll
	make -s program
a:
	ar crs 42.a \
	$(library)/visitor/startapp.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o
dll:
	g++ -shared -o 42.dll \
	$(library)/visitor/startapp.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-lole32 -lstrmiids -lwinmm -lws2_32
program:
	windres --input-format=rc -O coff -i $(datafile)/42.rc -o $(datafile)/42.res
	g++ \
	$(datafile)/42.res \
	$(library)/visitor/*.o \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libui0/*.o \
	$(library)/libui1/*.o \
	-lole32 -lstrmiids -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.so
