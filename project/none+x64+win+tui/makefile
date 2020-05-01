library?=../../library
datafile?=../../datafile
flag="-c -O2 -I.."
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	make -s exe
	make -s lib
	cp a.exe ../..
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
	make -s -C $(library)/libuser0 wintui \
	cflag=$(flag)
libuser1:
	make -s -C $(library)/libuser1 \
	cflag=$(flag)
lib:
	rm -f $(library)/libboot0/entry.o
	ar crs 42.a \
	$(library)/libboot0/*.o $(library)/libboot1/*.o \
	$(library)/libhard0/*.o $(library)/libhard1/*.o \
	$(library)/libsoft0/*.o $(library)/libsoft1/*.o \
	$(library)/libuser0/*.o $(library)/libuser1/*.o
	g++ -shared -o 42.dll \
	$(library)/libboot0/*.o $(library)/libboot1/*.o \
	$(library)/libhard0/*.o $(library)/libhard1/*.o \
	$(library)/libsoft0/*.o $(library)/libsoft1/*.o \
	$(library)/libuser0/*.o $(library)/libuser1/*.o \
	-lole32 -lstrmiids -lxinput -lwinmm -lws2_32
exe:
	windres --input-format=rc -O coff -i $(datafile)/winres/42.rc -o $(datafile)/winres/42.res
	g++ $(datafile)/42.res \
	$(library)/libboot0/*.o $(library)/libboot1/*.o \
	$(library)/libhard0/*.o $(library)/libhard1/*.o \
	$(library)/libsoft0/*.o $(library)/libsoft1/*.o \
	$(library)/libuser0/*.o $(library)/libuser1/*.o \
	-lole32 -lstrmiids -lxinput -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.so
	make -C $(library) clean
