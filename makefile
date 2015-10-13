all:
	echo "what?"
linux:								#cli
	make -s -C main cli
	make -s -C libsoft2 linux
	make -s -C libsoft1 linux
	make -s -C libsof0 linux
	gcc -o a.out \
	main/main.a \
	libsoft2/libsoft2.a \
	libsoft1/libsoft1.a \
	libsoft0/libsoft0.a
mac:								#cli
	make -s -C main cli
	make -s -C libsoft2 mac
	make -s -C libsoft1 mac
	make -s -C libsoft0 mac
	clang -o a.out \
	main/main.a \
	libsoft2/libsoft2.a \
	libsoft1/libsoft1.a \
	libsoft0/libsoft0.a
win:								#cli
	make -s -C main cli
	make -s -C libsoft2 win
	make -s -C libsoft1 win
	make -s -C libsoft0 win
	gcc -o a.exe libsoft0/uac.res \
	main/main.a \
	libsoft2/libsoft2.a \
	libsoft1/libsoft1.a \
	libsoft0/libsoft0.a
linux+fb:
	make -s -C main gui
	make -s -C libsoft2 linux
	make -s -C libsoft1 linux
	make -s -C libsoft0 linux
	make -s -C libui2 linux
	make -s -C libui1 linux
	make -s -C libui0 linux+fb
	gcc -o a.out \
	main/main.a \
	libui2/libui2.a \
	libui1/libui1.a \
	libui0/libui0.a \
	libsoft2/libsoft2.a \
	libsoft1/libsoft1.a \
	libsoft0/libsoft0.a
linux+xlib:
	make -s -C main gui
	make -s -C libsoft2 linux
	make -s -C libsoft1 linux
	make -s -C libsoft0 linux
	make -s -C libui2 linux
	make -s -C libui1 linux
	make -s -C libui0 linux+xlib
	gcc -o a.out \
	main/main.a \
	libui2/libui2.a \
	libui1/libui1.a \
	libui0/libui0.a \
	libsoft2/libsoft2.a \
	libsoft1/libsoft1.a \
	libsoft0/libsoft0.a \
	-lX11
linux+sdl:
	make -s -C main gui
	make -s -C libsoft2 linux
	make -s -C libsoft1 linux
	make -s -C libsoft0 linux
	make -s -C libui2 linux
	make -s -C libui1 linux
	make -s -C libui0 linux+sdl
	gcc -o a.out \
	main/main.a \
	libui2/libui2.a \
	libui1/libui1.a \
	libui0/libui0.a \
	libsoft2/libsoft2.a \
	libsoft1/libsoft1.a \
	libsoft0/libsoft0.a \
	-lSDL2 -lm
mac+sdl:
	make -s -C main cli
	make -s -C libsoft2 mac
	make -s -C libsoft1 mac
	make -s -C libsoft0 mac
	make -s -C libui2 mac
	make -s -C libui1 mac
	make -s -C libui0 mac
	clang -o a.out \
	main/main.a \
	libui2/libui2.a \
	libui1/libui1.a \
	libui0/libui0.a \
	libsoft2/libsoft2.a \
	libsoft1/libsoft1.a \
	libsoft0/libsoft0.a \
	-L /usr/local/Cellar/sdl2/2.0.3/lib \
	-lSDL2 -lm
win+api:
	make -s -C main gui
	make -s -C libsoft2 win
	make -s -C libsoft1 win
	make -s -C libsoft0 win
	make -s -C libui2 win
	make -s -C libui1 win
	make -s -C libui0 win+api
	#gcc -o a.exe libsoft0/uac.res \
	#gcc -o a.exe \
	gcc -o a.exe -mwindows libsoft0/uac.res \
	main/main.a \
	libui2/libui2.a \
	libui1/libui1.a \
	libui0/libui0.a \
	libsoft2/libsoft2.a \
	libsoft1/libsoft1.a \
	libsoft0/libsoft0.a \
	-lgdi32
win+sdl:
	make -s -C main gui
	make -s -C libsoft2 win
	make -s -C libsoft1 win
	make -s -C libsoft0 win
	make -s -C libui2 win
	make -s -C libui1 win
	make -s -C libui0 win+sdl
	gcc -o a.exe \
	libsoft0/uac.res \
	main/main.a \
	libui2/libui2.a \
	libui1/libui1.a \
	libui0/libui0.a \
	libsoft2/libsoft2.a \
	libsoft1/libsoft1.a \
	libsoft0/libsoft0.a \
	-Wl,--no-undefined -static -static-libgcc \
	-L /opt/SDL2-2.0.3/x86_64-w64-mingw32/lib \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid
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
	rm -f */*.o */*.a */*.res *.bin *.out *.exe
