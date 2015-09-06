all:
	echo "what?"
linux:
	make -s -C main cli
	make -s -C libsoft1 linux
	make -s -C libsoft linux
	make -s -C libhard1 linux
	make -s -C init linux
linux+fb:
	make -s -C main gui
	make -s -C libsoft1 linux
	make -s -C libsoft linux
	make -s -C libhard1 linux
	make -s -C init linux+fb
linux+xlib:
	make -s -C main gui
	make -s -C libsoft1 linux
	make -s -C libsoft linux
	make -s -C libhard1 linux
	make -s -C init linux+xlib
linux+sdl:
	make -s -C main gui
	make -s -C libsoft1 linux
	make -s -C libsoft linux
	make -s -C libhard1 linux
	make -s -C init linux+sdl
mac:
	make -s -C main cli
	make -s -C libsoft1 mac
	make -s -C libsoft mac
	make -s -C libhard1 mac
	make -s -C init mac
win:
	make -s -C main cli
	make -s -C libsoft1 win
	make -s -C libsoft win
	make -s -C libhard1 win
	make -s -C init win
win+api:
	make -s -C main gui
	make -s -C libsoft1 win
	make -s -C libsoft win
	make -s -C libhard1 win
	make -s -C init win+api
win+sdl:
	make -s -C main gui
	make -s -C libsoft1 win
	make -s -C libsoft win
	make -s -C libhard1 win
	make -s -C init win+sdl
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
