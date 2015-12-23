testutf:
	make -s -C ../../ x86+linux+cli
	gcc utf.c ../../library/library.a
	cp a.out ~/ && ~/a.out
testunicode:
	make -s -C ../../ x86+linux+cli
	gcc unicodetable.c ../../library/library.a
	cp a.out ~/ && ~/a.out
testxlib:
	make -s -C ../../ x86+linux+xlib
	gcc graphical.c ../../library/library.a  -lX11
	cp a.out ~/ && ~/a.out
testhanzi:
	make -s -C ../../ x86+linux+xlib
	gcc hanzi.c ../../library/library.a  -lX11
	cp a.out ~/ && ~/a.out
