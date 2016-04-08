linuxstatic:
	rm -f *.o
	gcc -c -fPIC -O2 adult/master.c
	gcc -c -fPIC -O2 adult/slave.c
linuxshared:
	rm -f *.o
	gcc -c -fPIC -O2 adult/master.c
	gcc -c -fPIC -O2 adult/slave.c
winstatic:
	rm -f *.o
	gcc -c -O2 adult/master.c
	gcc -c -O2 adult/slave.c
winshared:
	rm -f *.o
	gcc -c -O2 adult/master.c
	gcc -c -O2 adult/slave.c
cli:
	rm -f *.o
	gcc -c adult/master.c
	gcc -c adult/slave.c
	gcc -c child/cli.c
gui:
	rm -f *.o
	gcc -c adult/master.c
	gcc -c adult/slave.c
	gcc -c child/gui.c
web:
	rm -f *.o
	gcc -c adult/master.c
	gcc -c adult/slave.c
	gcc -c child/web.c
clean:
	rm -f *.o
