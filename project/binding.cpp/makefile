static:
	g++ test.cpp 42.a -lpulse -lpulse-simple -lpthread
	./a.out
shared:
	g++ test.cpp 42.so
	LD_LIBRARY_PATH=. ./a.out
shared-macos:
	g++ test.cpp 42.dylib
