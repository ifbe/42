linuxbuild:
	ndk-build
	android update project --path . --target android-21
	ant debug
	cp bin/FinalAnswer-debug.apk 42.apk
windowsbuild:
	cmd "/C ndk-build"
	cmd "/C android update project --path . --target android-21"
	ant debug
	cp bin/FinalAnswer-debug.apk 42.apk
install:
	adb install -r 42.apk
clean:
	rm -rf bin
	rm -rf gen
	rm -rf libs
	rm -rf obj
	rm -rf build.xml
	rm -rf local.properties
	rm -rf proguard-project.txt
	rm -rf project.properties
