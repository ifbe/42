.PHONY:linux+fb
linux+fb:
	gcc -c -fPIC -O2 fb.c
	rm -f libui0.a
	ar crs libui0.a fb.o
.PHONY:linux+xlib
linux+xlib:
	gcc -c -fPIC -O2 screen.c
	gcc -c -fPIC -O2 linux/xlib.c
	rm -f libui0.a
	ar crs libui0.a screen.o xlib.o
.PHONY:linux+sdl
linux+sdl:
	gcc -c -fPIC -O2 screen.c
	gcc -c -fPIC -O2 sdl2/sdl.c
	rm -f libui0.a
	ar crs libui0.a screen.o sdl2.o
.PHONY:mac+sdl
mac+sdl:
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	rm -f libui0.a
	ar crs libui0.a sdl.o
.PHONY:win+api
win+api:
	gcc -c -O2 screen.c
	gcc -c -O2 win/winapi.c
	rm -f libui0.a
	ar crs libui0.a screen.o winapi.o
.PHONY:win+sdl
win+sdl:
	gcc -c -O2 sdl2/sdl.c -I /opt/SDL2-2.0.3/x86_64-w64-mingw32/include
	rm -f libui0.a
	ar crs libui0.a sdl.o
clean:
	rm -f *.o *.a *.res
