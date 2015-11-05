.PHONY:linux+fb
linux+fb:
	gcc -c -fPIC -O2 fb.c
.PHONY:linux+xlib
linux+xlib:
	gcc -c -fPIC -O2 linux/xlib.c
.PHONY:linux+sdl
linux+sdl:
	gcc -c -fPIC -O2 sdl2/sdl.c
.PHONY:mac+sdl
mac+sdl:
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
.PHONY:win+api
win+api:
	gcc -c -O2 win/winapi.c
.PHONY:win+sdl
win+sdl:
	gcc -c -O2 sdl2/sdl.c -I /opt/SDL2-2.0.3/x86_64-w64-mingw32/include
clean:
	rm -f *.o *.a *.res
