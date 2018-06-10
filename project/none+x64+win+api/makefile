library?=../../library
datafile?=../../datafile
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s a
	make -s dll
	make -s program
libboot0:
	make -s -C $(library)/libboot/0 wincmd \
	cflag="-c -O2"
libboot1:
	make -s -C $(library)/libboot/1 \
	cflag="-c -O2"
libhard0:
	make -s -C $(library)/libhard/0 winx64 \
	cflag="-c -O2"
libhard1:
	make -s -C $(library)/libhard/1 \
	cflag="-c -O2"
libsoft0:
	make -s -C $(library)/libsoft/0 win \
	cflag="-c -O2 -I.."
libsoft1:
	make -s -C $(library)/libsoft/1 \
	cflag="-c -O2 -I.."
libuser0:
	make -s -C $(library)/libuser/0 winapi \
	cflag="-c -O2 -I.."
libuser1:
	make -s -C $(library)/libuser/1 \
	cflag="-c -O2 -I.."
a:
	ar crs 42.a \
	$(library)/libboot/0/*.o \
	$(library)/libboot/1/*.o \
	$(library)/libhard/0/*.o \
	$(library)/libhard/1/*.o \
	$(library)/libsoft/0/*.o \
	$(library)/libsoft/1/*.o \
	$(library)/libuser/0/*.o \
	$(library)/libuser/1/*.o
dll:
	g++ -shared -o 42.dll \
	$(library)/libboot/0/*.o \
	$(library)/libboot/1/*.o \
	$(library)/libhard/0/*.o \
	$(library)/libhard/1/*.o \
	$(library)/libsoft/0/*.o \
	$(library)/libsoft/1/*.o \
	$(library)/libuser/0/*.o \
	$(library)/libuser/1/*.o \
	-lgdi32 -lole32 -lstrmiids -lxinput -lwinmm -lws2_32
program:
	windres --input-format=rc -O coff -i $(datafile)/42.rc -o $(datafile)/42.res
	g++ -mwindows \
	$(datafile)/42.res \
	$(library)/libboot/0/*.o \
	$(library)/libboot/1/*.o \
	$(library)/libhard/0/*.o \
	$(library)/libhard/1/*.o \
	$(library)/libsoft/0/*.o \
	$(library)/libsoft/1/*.o \
	$(library)/libuser/0/*.o \
	$(library)/libuser/1/*.o \
	-lgdi32 -lole32 -lstrmiids -lxinput -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.so
	make -C $(library) clean