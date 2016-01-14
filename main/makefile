#console interface
.PHONY:cli
cli:
	make -C cli
	rm -f main.a
	ar crs main.a cli/*.o

#graphic interface
.PHONY:gui
gui:
	make -C gui
	rm -f main.a
	ar crs main.a gui/*.o

#web interface
.PHONY:web
web:
	make -C web
	rm -f main.a
	ar crs main.a web/*.o
clean:
	rm -f *.o *.a
	rm -f */*.o */*.a
