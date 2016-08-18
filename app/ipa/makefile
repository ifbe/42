all:
	xcodebuild -sdk iphoneos7.0 -configuration Release -target 42 clean
	xcodebuild -sdk iphoneos7.0 -configuration Release -target 42
	xcrun -sdk iphoneos PackageApplication -v ./build/Release-iphoneos/42.app -o 42.ipa
clean:
	rm -f *.ipa
