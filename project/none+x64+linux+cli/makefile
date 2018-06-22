library?=../../library
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	+make -s a
	+make -s so
	+make -s program
libboot0:
	+make -s -C $(library)/libboot/0 none \
	cflag="-c -O2 -fPIC -I.."
libboot1:
	+make -s -C $(library)/libboot/1 \
	cflag="-c -O2 -fPIC -I.."
libhard0:
	+make -s -C $(library)/libhard/0 x64 \
	cflag="-c -O2 -fPIC -I.."
libhard1:
	+make -s -C $(library)/libhard/1 \
	cflag="-c -O2 -fPIC -I.."
libsoft0:
	+make -s -C $(library)/libsoft/0 linux \
	cflag="-c -O2 -fPIC -I.."
libsoft1:
	+make -s -C $(library)/libsoft/1 \
	cflag="-c -O2 -fPIC -I.."
libuser0:
	+make -s -C $(library)/libuser/0 linuxcli \
	cflag="-c -O2 -fPIC -I.."
libuser1:
	+make -s -C $(library)/libuser/1 \
	cflag="-c -O2 -fPIC -I.."
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
so:
	gcc -shared -o 42.so \
	$(library)/libboot/0/*.o \
	$(library)/libboot/1/*.o \
	$(library)/libhard/0/*.o \
	$(library)/libhard/1/*.o \
	$(library)/libsoft/0/*.o \
	$(library)/libsoft/1/*.o \
	$(library)/libuser/0/*.o \
	$(library)/libuser/1/*.o
program:
	gcc -o a.out \
	$(library)/libboot/0/*.o \
	$(library)/libboot/1/*.o \
	$(library)/libhard/0/*.o \
	$(library)/libhard/1/*.o \
	$(library)/libsoft/0/*.o \
	$(library)/libsoft/1/*.o \
	$(library)/libuser/0/*.o \
	$(library)/libuser/1/*.o \
	-lpulse -lpulse-simple -lpthread -lm
	#-lasound -lpthread
clean:
	rm -f *.o *.a *.so
