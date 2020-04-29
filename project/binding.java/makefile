run:
	#step j0: generate test.class
	javac test.java
	#step j1: run java
	java test
mac:
	#step c0: generate test.h
	javac test.java -h .
	#step c1: compile library
	gcc -fPIC -shared -o libadd.dylib add.c \
-I/Library/Java/JavaVirtualMachines/jdk-10.0.2.jdk/Contents/Home/include \
-I/Library/Java/JavaVirtualMachines/jdk-10.0.2.jdk/Contents/Home/include/darwin
linux:
	#step c0: generate test.h
	javah test.java
	#step c1: compile library
	gcc -fPIC -shared -o libadd.so add.c \
-I/usr/lib/jvm/java-8-openjdk-amd64/include \
-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux
clean:
	rm -f *.class test.h *.so *.dylib *.out
