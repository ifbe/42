.PHONY:cli
cli:
	gcc -c -O2 console/console.c
.PHONY:fb
fb:
	gcc -c -fPIC -O2 framebuffer/fb.c
.PHONY:xlib
xlib:
	gcc -c -fPIC -O2 xlib/xlib.c
.PHONY:winapi
winapi:
	gcc -c -O2 win/winapi.c
.PHONY:html
webui:
	gcc -c -O2 webui/webui.c
.PHONY:sdl
sdl:
	gcc -c -fPIC -O2 sdl2/sdl.c
.PHONY:mac+sdl
mac+sdl:
	clang -c -O2 sdl/sdl.c -I /usr/local/Cellar/sdl2/2.0.3/include
.PHONY:win+sdl
win+sdl:
	gcc -c -O2 sdl2/sdl.c -I /opt/SDL2-2.0.3/x86_64-w64-mingw32/include
clean:
	rm -f *.o *.a *.res
