target?=none+x64+linux+cli
########################################help###################################
help:
	@echo "###############################################################"
	@echo "usage:"
	@echo "make vendor+cpu+system+ui"
	@echo "example:"
	@echo "make none+x64+win+cli"
	@echo "make none+x64+linux+xlib"
	@echo "make bcm+arm+android+cli"
	@echo "make rk+arm+android+apk"
	@echo "###############################################################"
###############################################################################




##################################mobile platform##############################
#withoutjnifolder:
#	ndk-build \
#	APP_BUILD_SCRIPT=app/android/Android.mk \
#	NDK_APPLICATION_MK=app/android/Application.mk \
#	NDK_PROJECT_PATH=app/android \
#	LOCAL_PATH=.
android:
	make -s -C app/android --no-print-directory
androidpush:
	adb push app/android/libs/arm64-v8a/42 /data/42

apk:
	make -s -C app/apk --no-print-directory
	cp app/apk/bin/Plasma-debug.apk 42.apk
apkinstall:
	adb install -r 42.apk

ipa:
	make -s -C app/ipa --no-print-directory
ipainstall:

uwp:
	make -s -C app/uwp --no-print-directory
uwpinstall:

efi:
	make -s -C app/efi --no-print-directory
############################################################################




#################################small device################################
none+arm+linux+cli:
	make -C app/none+arm+linux+cli --no-print-directory
	cp app/none+arm+linux+cli/42.a .
	cp app/none+arm+linux+cli/42.so .
	cp app/none+arm+linux+cli/a.out .
none+arm+linux+epoll:
	make -C app/none+arm+linux+epoll --no-print-directory
	cp app/none+arm+linux+epoll/42.a .
	cp app/none+arm+linux+epoll/42.so .
	cp app/none+arm+linux+epoll/a.out .
none+arm+linux+text:
	make -C app/none+arm+linux+text --no-print-directory
	cp app/none+arm+linux+text/42.a .
	cp app/none+arm+linux+text/42.so .
	cp app/none+arm+linux+text/a.out .
none+arm+linux+fb:
	make -C app/none+arm+linux+fb --no-print-directory
	cp app/none+arm+linux+fb/42.a .
	cp app/none+arm+linux+fb/42.so .
	cp app/none+arm+linux+fb/a.out .
none+arm+linux+xlib:
	make -C app/none+arm+linux+xlib --no-print-directory
	cp app/none+arm+linux+xlib/42.a .
	cp app/none+arm+linux+xlib/42.so .
	cp app/none+arm+linux+xlib/a.out .
############################################################################




############################################################################
none+mips+linux+cli:
	make -C app/none+mips+linux+cli --no-print-directory
	cp app/none+mips+linux+cli/a.out .
############################################################################




#####################################pc#######################################
none+x64+linux+cli:
	make -C app/none+x64+linux+cli --no-print-directory
	cp app/none+x64+linux+cli/42.a .
	cp app/none+x64+linux+cli/42.so .
	cp app/none+x64+linux+cli/a.out .
none+x64+linux+epoll:
	make -C app/none+x64+linux+epoll --no-print-directory
	cp app/none+x64+linux+epoll/42.a .
	cp app/none+x64+linux+epoll/42.so .
	cp app/none+x64+linux+epoll/a.out .
none+x64+linux+sdl:
	make -C app/none+x64+linux+sdl --no-print-directory
	cp app/none+x64+linux+sdl/42.a .
	cp app/none+x64+linux+sdl/42.so .
	cp app/none+x64+linux+sdl/a.out .
none+x64+linux+text:
	make -C app/none+x64+linux+text --no-print-directory
	cp app/none+x64+linux+text/42.a .
	cp app/none+x64+linux+text/42.so .
	cp app/none+x64+linux+text/a.out .
none+x64+linux+fb:
	make -C app/none+x64+linux+fb --no-print-directory
	cp app/none+x64+linux+fb/42.a .
	cp app/none+x64+linux+fb/42.so .
	cp app/none+x64+linux+fb/a.out .
none+x64+linux+xlib:
	make -C app/none+x64+linux+xlib --no-print-directory
	cp app/none+x64+linux+xlib/42.a .
	cp app/none+x64+linux+xlib/42.so .
	cp app/none+x64+linux+xlib/a.out .
none+x64+linux+wayland:
	make -C app/none+x64+linux+wayland --no-print-directory
	cp app/none+x64+linux+wayland/42.a .
	cp app/none+x64+linux+wayland/42.so .
	cp app/none+x64+linux+wayland/a.out .




none+x64+mac+cli:
	make -C app/none+x64+mac+cli --no-print-directory
	cp app/none+x64+mac+cli/42.a .
	cp app/none+x64+mac+cli/42.dylib .
	cp app/none+x64+mac+cli/a.exe .
none+x64+mac+kqueue:
	make -C app/none+x64+mac+kqueue --no-print-directory
	cp app/none+x64+mac+kqueue/42.a .
	cp app/none+x64+mac+kqueue/42.dylib .
	cp app/none+x64+mac+kqueue/a.out .
none+x64+mac+sdl:
	make -C app/none+x64+mac+sdl --no-print-directory
	cp app/none+x64+mac+sdl/42.a .
	cp app/none+x64+mac+sdl/42.dylib .
	cp app/none+x64+mac+sdl/a.out .
none+x64+mac+cocoa:
	make -C app/none+x64+mac+cocoa --no-print-directory
	cp app/none+x64+mac+cocoa/42.a .
	cp app/none+x64+mac+cocoa/42.dylib .
	cp app/none+x64+mac+cocoa/a.exe .
none+x64+mac+quartz:
	make -C app/none+x64+mac+quartz --no-print-directory
	cp app/none+x64+mac+quartz/42.a .
	cp app/none+x64+mac+quartz/42.dylib .
	cp app/none+x64+mac+quartz/a.exe .
none+x64+mac+text:
	make -C app/none+x64+mac+text --no-print-directory
	cp app/none+x64+mac+text/42.a .
	cp app/none+x64+mac+text/42.dylib .
	cp app/none+x64+mac+text/a.exe .




none+x64+win+cli:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+cli --no-print-directory
	cp app/none+x64+win+cli/42.a .
	cp app/none+x64+win+cli/42.dll .
	cp app/none+x64+win+cli/a.exe .
none+x64+win+iocp:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+iocp --no-print-directory
	cp app/none+x64+win+iocp/42.a .
	cp app/none+x64+win+iocp/42.dll .
	cp app/none+x64+win+iocp/a.exe .
none+x64+win+sdl:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+sdl --no-print-directory
	cp app/none+x64+win+sdl/42.a .
	cp app/none+x64+win+sdl/42.dll .
	cp app/none+x64+win+sdl/a.exe .
none+x64+win+text:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+text --no-print-directory
	cp app/none+x64+win+text/42.a .
	cp app/none+x64+win+text/42.dll .
	cp app/none+x64+win+text/a.exe .
none+x64+win+api:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+api --no-print-directory
	cp app/none+x64+win+api/42.a .
	cp app/none+x64+win+api/42.dll .
	cp app/none+x64+win+api/a.exe .




push:
	git add --all .
	git commit -a
	git push
pull:
	git pull
clean:
	rm -f *.apk *.out *.exe *.o *.a *.dll *.dylib *.so *.res
	make -s -C app clean
	make -s -C library clean
	make -s -C res clean
	make -s -C tool clean
