.PHONY:win
cli:
	gcc -c -O2 cli.c
	rm -f main.a
	ar crs main.a cli.o
.PHONY:gui
gui:
	gcc -c -O2 gui.c
	rm -f main.a
	ar crs main.a gui.o
clean:
	rm -f *.o *.a
