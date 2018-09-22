library?=../../library
datafile?=../../datafile
flag="-c -O2 -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s a
	make -s dll
	make -s program
libboot0:
	make -s -C $(library)/libboot0 wincmd \
	cflag=$(flag)
libboot1:
	make -s -C $(library)/libboot1 \
	cflag=$(flag)
libhard0:
	make -s -C $(library)/libhard0 winx64 \
	cflag=$(flag)
libhard1:
	make -s -C $(library)/libhard1 \
	cflag=$(flag)
libsoft0:
	make -s -C $(library)/libsoft0 win \
	cflag=$(flag)
libsoft1:
	make -s -C $(library)/libsoft1 \
	cflag=$(flag)
libuser0:
	make -s -C $(library)/libuser0 wind3d \
	cflag=$(flag)
libuser1:
	make -s -C $(library)/libuser1 \
	cflag=$(flag)
a:
	ar crs 42.a \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o
dll:
	g++ -shared -o 42.dll \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-lole32 -lstrmiids -lxinput -lwinmm -lws2_32
program:
	windres --input-format=rc -O coff -i $(datafile)/42.rc -o $(datafile)/42.res
	g++ -mwindows \
	$(datafile)/42.res \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-lole32 -lstrmiids -lxinput -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.dll
