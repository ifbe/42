.PHONY:win
win:
	gcc -c -O2 cli.c
	rm -f main.a
	ar crs main.a cli.o
.PHONY:gui
gui:
	gcc -c -O2 gui/gui.c
	gcc -c -O2 gui/middle.c
	gcc -c -O2 gui/left.c
	gcc -c -O2 gui/right.c
	gcc -c -O2 gui/overview.c
	gcc -c -O2 gui/console.c
	gcc -c -O2 gui/hex.c
	rm -f main.a
	ar crs main.a gui.o middle.o left.o right.o overview.o console.o hex.o
clean:
	rm -f *.o *.a
