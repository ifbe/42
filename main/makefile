.PHONY:win
cli:
	gcc -c -O2 cli.c
	rm -f main.a
	ar crs main.a cli.o
.PHONY:gui
gui:
	gcc -c -O2 gui/gui.c
	gcc -c -O2 gui/0-menu.c
	gcc -c -O2 gui/1-hex.c
	gcc -c -O2 gui/1-console.c
	gcc -c -O2 gui/2-real.c
	gcc -c -O2 gui/2-logic.c
	gcc -c -O2 gui/3-d.c
	gcc -c -O2 gui/3-jiong.c
	rm -f main.a
	ar crs main.a gui.o 0-menu.o 1-hex.o 1-console.o 2-real.o 2-logic.o 3-d.o 3-jiong.o
clean:
	rm -f *.o *.a
