.PHONY:cli
cli:
	rm -f *.o
	gcc -c -O2 console/console.c
	gcc -c -O2 window.c
.PHONY:fb
fb:
	rm -f *.o
	gcc -c -fPIC -O2 framebuffer/fb.c
	gcc -c -fPIC -O2 window.c
.PHONY:xlib
xlib:
	rm -f *.o
	gcc -c -fPIC -O2 xlib/xlib.c
	gcc -c -fPIC -O2 window.c
.PHONY:winapi
winapi:
	rm -f *.o
	gcc -c -O2 winapi/winapi.c
	gcc -c -O2 window.c
.PHONY:html
webui:
	rm -f *.o
	gcc -c -O2 webui/webui.c
	gcc -c -O2 window.c
.PHONY:sdl
sdl:
	rm -f *.o
	gcc -c -fPIC -O2 sdl2/sdl.c
	gcc -c -fPIC -O2 window.c
.PHONY:mac+sdl
mac+sdl:
	rm -f *.o
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
	gcc -c -O2 window.c
.PHONY:win+sdl
win+sdl:
	rm -f *.o
	gcc -c -O2 sdl2/sdl.c -I /opt/SDL2-2.0.3/x86_64-w64-mingw32/include
	gcc -c -O2 window.c
clean:
	rm -f *.o *.a *.res
