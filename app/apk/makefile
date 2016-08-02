linuxbuild:
	make cleanicon
	make createicon
	ndk-build
	android update project --path . --target android-21
	ant debug
	make cleanicon
windowsbuild:
	make cleanicon
	make createicon
	cmd "/C ndk-build"
	cmd "/C android update project --path . --target android-21"
	ant debug
	make cleanicon
createicon:
	mkdir res/drawable-ldpi
	mkdir res/drawable-mdpi
	mkdir res/drawable-hdpi
	mkdir res/drawable-xhdpi
	mkdir res/drawable-xxhdpi
	cp ../../res/tardis.ico res/drawable-ldpi
	cp ../../res/tardis.ico res/drawable-mdpi
	cp ../../res/tardis.ico res/drawable-hdpi
	cp ../../res/tardis.ico res/drawable-xhdpi
	cp ../../res/tardis.ico res/drawable-xxhdpi
cleanicon:
	rm -rf res/drawable-*
clean:
	rm -rf bin
	rm -rf gen
	rm -rf libs
	rm -rf obj
	rm -rf build.xml
	rm -rf local.properties
	rm -rf proguard-project.txt
	rm -rf project.properties
