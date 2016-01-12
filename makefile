what:
	@echo "----------------please choose one example below----------------"
	@echo "make x86+linux+cli:    run in linux terminal"
	@echo "make x86+win+cli    run in windows cmd"
	@echo "make x86+linux+xlib    run in X:"
	@echo "make x86+win+api    run in windows xp+"
	@echo "make clean:    clean"
	@echo "----------------please choose one example above----------------"
arm+linux+cli:
	make -C library arm+linux+cli
	make -C main cli
	gcc main/main.a library/library.a \
	-o a.out
x86+linux+cli:
	make -C library x86+linux+cli
	make -C main cli
	gcc main/main.a library/library.a \
	-o a.out
x86+mac+cli:
	make -C library x86+mac+cli
	make -C main cli
	clang main/main.a library/library.a \
	-o a.out
x86+win+cli:
	make -C library x86+win+cli
	make -C main cli
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	gcc res/uac.res \
	main/main.a library/library.a \
	-o a.exe




x86+linux+fb:
	make -C library x86+linux+fb
	make -C main gui
	gcc main/main.a library/library.a \
	-o a.out
x86+linux+xlib:
	make -C library x86+linux+xlib
	make -C main gui
	gcc main/main.a library/library.a \
	-lX11 \
	-o a.out
x86+linux+sdl:
	make -C library x86+linux+sdl
	make -C main gui
	gcc main/main.a library/library.a \
	-lSDL2 -lm \
	-o a.out
x86+linux+web:
	make -C library x86+linux+web
	make -C main gui
	gcc \
	main/main.a library/library.a \
	-o a.out




x86+mac+sdl:
	make -C library x86+mac+sdl
	make -C main cli
	clang main/main.a library/library.a \
	-L /usr/local/Cellar/sdl2/2.0.3/lib \
	-lSDL2 -lm \
	-o a.out




x86+win+api:
	make -C library x86+win+api
	make -C main gui
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	gcc -mwindows res/uac.res \
	main/main.a library/library.a \
	-lgdi32 \
	-o a.exe
x86+win+sdl:
	make -C library x86+win+sdl
	make -C main gui
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	gcc -mwindows res/uac.res \
	main/main.a library/library.a \
	-Wl,--no-undefined -static -static-libgcc \
	-L /opt/SDL2-2.0.3/x86_64-w64-mingw32/lib \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid \
	-o a.exe
x86+win+web:
	make -C library x86+win+web
	make -C main gui
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	gcc -mwindows res/uac.res \
	main/main.a library/library.a \
	-o a.exe




qemu:
	cp ../../../core/core.bin test.img    #first64k
	dd if=color.bin of=test.img bs=65536 seek=1 conv=notrunc    #real bin
	echo -n 9 | dd of=test.img bs=1 seek=1048575 conv=notrunc    #fill 0
	echo "test" | dd of=test.img bs=1 seek=448 conv=notrunc    #test sign
	../../../tool/qemu $(shell pwd)/test.img
kvm:
	cp ../../../core/core.bin test.img    #first64k
	dd if=color.bin of=test.img bs=65536 seek=1 conv=notrunc    #real bin
	echo -n 9 | dd of=test.img bs=1 seek=1048575 conv=notrunc    #fill 0
	echo "test" | dd of=test.img bs=1 seek=448 conv=notrunc    #test sign
	../../../tool/kvm $(shell pwd)/test.img




clean:
	rm -f *.out *.exe
	rm -f */*.o
	rm -f */*.a
	rm -f */*.res
	make -s -C library clean
	make -s -C main clean
