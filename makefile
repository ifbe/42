all:
	echo "what?"
linux:
	make -s -C main cli
	make -s -C wrap2 linux
	make -s -C wrap1 linux
	make -s -C wrap linux
linux+fb:
	make -s -C main gui
	make -s -C wrap2 linux
	make -s -C wrap1 linux
	make -s -C wrap linux+fb
linux+xlib:
	make -s -C main gui
	make -s -C wrap2 linux
	make -s -C wrap1 linux
	make -s -C wrap linux+xlib
linux+sdl:
	make -s -C main gui
	make -s -C wrap2 linux
	make -s -C wrap1 linux
	make -s -C wrap linux+sdl
mac:
	make -s -C main cli
	make -s -C wrap2 mac
	make -s -C wrap1 mac
	make -s -C wrap mac
win:
	make -s -C main cli
	make -s -C wrap2 win
	make -s -C wrap1 win
	make -s -C wrap win
win+api:
	make -s -C main gui
	make -s -C wrap2 win
	make -s -C wrap1 win
	make -s -C wrap win+api
win+sdl:
	make -s -C main gui
	make -s -C wrap2 win
	make -s -C wrap1 win
	make -s -C wrap win+sdl
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
