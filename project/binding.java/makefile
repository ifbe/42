all:
	javac com/example/test.java
	javah com.example.test
	gcc com_example_test.c -fPIC -shared -o lib42.so \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux
	java com.example.test
clean:
	rm -f *.so *.dll com/example/*.class
