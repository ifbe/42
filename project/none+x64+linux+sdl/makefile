library?=../../library
all:libboot0 libboot1 libhard0 libhard1 libsoft0 libsoft1 libuser0 libuser1
	+make -s a
	+make -s so
	+make -s program
	cp a.out ../..
libboot0:
	+make -s -C $(library)/libboot0 none \
	cflag="-c -O2 -fPIC -I.."
libboot1:
	+make -s -C $(library)/libboot1 \
	cflag="-c -O2 -fPIC -I.."
libhard0:
	+make -s -C $(library)/libhard0 x64 \
	cflag="-c -O2 -fPIC -I.."
libhard1:
	+make -s -C $(library)/libhard1 \
	cflag="-c -O2 -fPIC -I.."
libsoft0:
	+make -s -C $(library)/libsoft0 linux \
	cflag="-c -O2 -fPIC -I.."
libsoft1:
	+make -s -C $(library)/libsoft1 \
	cflag="-c -O2 -fPIC -I.."
libuser0:
	+make -s -C $(library)/libuser0 linuxsdl \
	cflag="-c -O2 -fPIC -I.."
libuser1:
	+make -s -C $(library)/libuser1 \
	cflag="-c -O2 -fPIC -I.."
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
so:
	gcc -shared -o 42.so \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-lSDL2 -lm
program:
	gcc \
	$(library)/libboot0/*.o \
	$(library)/libboot1/*.o \
	$(library)/libhard0/*.o \
	$(library)/libhard1/*.o \
	$(library)/libsoft0/*.o \
	$(library)/libsoft1/*.o \
	$(library)/libuser0/*.o \
	$(library)/libuser1/*.o \
	-lpulse -lpulse-simple -lpthread -lSDL2 -lm
clean:
	rm -f *.o *.a *.so
