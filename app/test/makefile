############################text#############################
testutf:
	make -C ../none+x86+win+cli static --no-print-directory
	gcc text/utf.c ../none+x86+win+cli/library.a
testunicode:
	make -C ../none+x86+win+cli static --no-print-directory
	gcc text/unicodetable.c ../none+x86+win+cli/library.a
###########################graphical#########################
winapi:
	make -C ../none+x86+win+api static --no-print-directory
	gcc graphic/graphical.c ../none+x86+win+api/library.a  -lgdi32
winapihanzi:
	make -C ../none+x86+win+api static --no-print-directory
	gcc graphic/hanzi.c ../none+x86+win+api/library.a  -lgdi32
xlib:
	make -C ../none+x86+win+xlib static --no-print-directory
	gcc graphic/graphical.c ../none+x86+win+xlib/library.a  -lX11
xlibhanzi:
	make -C ../none+x86+win+xlib static --no-print-directory
	gcc graphic/hanzi.c ../none+x86+win+xlib/library.a  -lX11
############################so cool############################
1:
	make -C ../none+x86+win+api static --no-print-directory
	gcc cool/1.c ../none+x86+win+api/library.a  -lgdi32
2:
	make -C ../none+x86+win+api static --no-print-directory
	gcc cool/2.c ../none+x86+win+api/library.a  -lgdi32
3:
	make -C ../none+x86+win+api static --no-print-directory
	gcc cool/3.c ../none+x86+win+api/library.a  -lgdi32
4:
	make -C ../none+x86+win+api static --no-print-directory
	gcc cool/4.c ../none+x86+win+api/library.a  -lgdi32
5:
	make -C ../none+x86+win+api static --no-print-directory
	gcc cool/5.c ../none+x86+win+api/library.a  -lgdi32
6:
	make -C ../none+x86+win+api static --no-print-directory
	gcc cool/6.c ../none+x86+win+api/library.a  -lgdi32
###########################image process###########################
gray:
	make -C ../none+x86+win+api static --no-print-directory
	gcc image/gray.c ../none+x86+win+api/library.a  -lgdi32