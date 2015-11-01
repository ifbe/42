#console interface
.PHONY:win
cli:
	gcc -c -O2 cli.c
	rm -f main.a
	ar crs main.a cli.o

#graphic interface
.PHONY:gui
gui:
	gcc -c -O2 gui.c
	rm -f main.a
	ar crs main.a gui.o

#web interface
.PHONY:web
web:
	gcc -c -O2 web.c
	rm -f main.a
	ar crs main.a web.o
clean:
	rm -f *.o *.a
