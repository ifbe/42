testutf:
	make -C ../none+x86+win+cli static --no-print-directory
	gcc utf.c ../none+x86+win+cli/library.a
testunicode:
	make -C ../none+x86+win+cli static --no-print-directory
	gcc unicodetable.c ../none+x86+win+cli/library.a
winapi:
	make -C ../none+x86+win+api static --no-print-directory
	gcc graphical.c ../none+x86+win+api/library.a  -lgdi32
winapihanzi:
	make -C ../none+x86+win+api static --no-print-directory
	gcc hanzi.c ../none+x86+win+api/library.a  -lgdi32
xlib:
	make -C ../none+x86+win+xlib static --no-print-directory
	gcc graphical.c ../none+x86+win+xlib/library.a  -lX11
xlibhanzi:
	make -C ../none+x86+win+xlib static --no-print-directory
	gcc hanzi.c ../none+x86+win+xlib/library.a  -lX11
