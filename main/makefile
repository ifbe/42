#console interface
.PHONY:win
cli:
	gcc -c -O2 cli.c
	rm -f main.a
	ar crs main.a cli.o

#graphic interface
.PHONY:gui
gui:
	gcc -c -O2 gui/gui.c
	gcc -c -O2 gui/0.menu.c
	gcc -c -O2 gui/1.hex.c
	gcc -c -O2 gui/2.keyboard.c
	gcc -c -O2 gui/2.tree.c
	gcc -c -O2 gui/3.sketchpad.c
	gcc -c -O2 gui/4.console.c
	rm -f main.a
	ar crs main.a \
	gui.o \
	0.menu.o \
	1.hex.o \
	2.keyboard.o 2.tree.o \
	3.sketchpad.o \
	4.console.o

#web interface
.PHONY:web
web:
	gcc -c -O2 web.c
	rm -f main.a
	ar crs main.a web.o
clean:
	rm -f *.o *.a
