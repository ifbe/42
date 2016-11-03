here = ?????
1:
	cd ../../
	make what+what+what+what
	cp 42.a $here/sourcecode
	cd -
2:
	git clone https://git01.codeplex.com/sdlpal
	cd sdlpal/sdlpal
	dos2unix *
	git apply $here/*.patch
	make
3:
	download gamedata
	cp sdlpal/sdlpal/sdlpal gamedata/
	cd gamedata
	./sdlpal
