linux-a:
	g++ test.cpp 42.a -lpulse -lpulse-simple -lpthread
	./a.out
linux-so:
	g++ test.cpp 42.so
	LD_LIBRARY_PATH=. ./a.out

macos-a:
	g++ test.cpp 42.a
macos-dylib:
	g++ test.cpp 42.dylib

win-a:
	g++ test.cpp 42.a
win-dll:
	g++ test.cpp 42.dll
