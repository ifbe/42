compiler ?= gcc
cflag ?= -c -O2 -fPIC

cli:
	rm -f *.o
	$(compiler) $(cflag) cli.c

gui:
	rm -f *.o
	$(compiler) $(cflag) gui.c

clean:
	rm -f *.o
