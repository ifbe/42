help:
	@echo "step1:	library"
	@echo "		make win+cli"
	@echo "		make win+api"
	@echo "		make linux+cli"
	@echo "		make linux+xlib"
	@echo "step2:	executable"
	@echo "		make 1"
	@echo "		make 2"
	@echo "		make 3"
	@echo "		make 4"
	@echo "		make 5"
	@echo "		make 6"
####################create the library#######################
win+cli:
	make -C ../none+x86+win+cli static --no-print-directory
	cp ../none+x86+win+cli/library.a .
linux+cli:
	make -C ../none+x86+linux
	cp ../none+x86+linux+cli/library.a .
win+api:
	make -C ../none+x86+win+api static --no-print-directory
	cp ../none+x86+win+api/library.a .
linux+xlib:
	make -C ../none+x86+linux+xlib static --no-print-directory
	cp ../none+x86+linux+xlib/library.a .
############################text#############################
testutf:
	gcc text/utf.c ./library.a
testunicode:
	gcc text/unicodetable.c ./library.a
############################graphical##########################
xlib:
	gcc graphic/graphical.c ./library.a  -lX11
xlibhanzi:
	gcc graphic/hanzi.c ./library.a  -lX11
############################so cool############################
1:
	gcc cool/1.c ./library.a  -lgdi32
2:
	gcc cool/2.c ./library.a  -lgdi32
3:
	gcc cool/3.c ./library.a  -lgdi32
4:
	gcc cool/4.c ./library.a  -lgdi32
5:
	gcc cool/5.c ./library.a  -lgdi32
6:
	gcc cool/6.c ./library.a  -lgdi32
###########################image process###########################
blur:
	gcc image/blur.c ./library.a  -lgdi32
gray:
	gcc image/gray.c ./library.a  -lgdi32
