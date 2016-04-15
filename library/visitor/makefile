compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) adult/master.c
	$(compiler) $(cflag) adult/slave.c
linuxstatic:
	rm -f *.o
	make -s target compiler="gcc" cflag="-c -fPIC -O2"
linuxshared:
	rm -f *.o
	make -s target compiler="gcc" cflag="-c -fPIC -O2"
winstatic:
	rm -f *.o
	make -s target compiler="gcc" cflag="-c -O2"
winshared:
	rm -f *.o
	make -s target compiler="gcc" cflag="-c -O2"
cli:
	rm -f *.o
	gcc -c -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast child/cli.c
	make -s target compiler="gcc" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
gui:
	rm -f *.o
	gcc -c -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast child/gui.c
	make -s target compiler="gcc" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
web:
	rm -f *.o
	gcc -c -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast child/web.c
	make -s target compiler="gcc" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
clean:
	rm -f *.o
