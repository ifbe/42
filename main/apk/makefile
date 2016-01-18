linuxbuild:
	ndk-build
	android update project --path . --target android-21
	ant debug
windowsbuild:
	cmd "/C ndk-build"
	cmd "/C android update project --path . --target android-21"
	ant debug
