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
############################################################################




#################################small device################################
arm+linux+cli:
	make -C app/none+arm+linux+cli --no-print-directory
	cp app/none+arm+linux+cli/a.out .
arm+linux+fb:
	make -C app/none+arm+linux+fb --no-print-directory
	cp app/none+arm+linux+fb/a.out .
arm+linux+xlib:
	make -C app/none+arm+linux+xlib --no-print-directory
	cp app/none+arm+linux+xlib/a.out .
############################################################################




############################################################################
mips+linux+cli:
	make -C app/none+mips+linux+cli --no-print-directory
	cp app/none+mips+linux+cli/a.out .
############################################################################




#####################################pc#######################################
x64+linux+cli:
	make -C app/none+x64+linux+cli --no-print-directory
	cp app/none+x64+linux+cli/a.out .
x64+linux+text:
	make -C app/none+x64+linux+text --no-print-directory
	cp app/none+x64+linux+text/a.out .
x64+linux+fb:
	make -C app/none+x64+linux+fb --no-print-directory
	cp app/none+x64+linux+fb/a.out .
x64+linux+xlib:
	make -C app/none+x64+linux+xlib --no-print-directory
	cp app/none+x64+linux+xlib/a.out .
x64+linux+sdl:
	make -C app/none+x64+linux+sdl --no-print-directory
	cp app/none+x64+linux+sdl/a.out .
x64+linux+web:
	make -C app/none+x64+linux+web --no-print-directory
	cp app/none+x64+linux+web/a.out .




x64+mac+cli:
	make -C app/none+x64+mac+cli --no-print-directory
	cp app/none+x64+mac+cli/a.exe .
x64+mac+sdl:
	make -C app/none+x64+mac+sdl --no-print-directory
	cp app/none+x64+mac+sdl/a.out .




x64+win+cli:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+cli --no-print-directory
	cp app/none+x64+win+cli/a.exe .
x64+win+api:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+api --no-print-directory
	cp app/none+x64+win+api/a.exe .
x64+win+sdl:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+sdl --no-print-directory
	cp app/none+x64+win+sdl/a.exe .
x64+win+web:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+web --no-print-directory
	cp app/none+x64+win+web/a.exe .




push:
	git add --all .
	git commit -a
	git push
pull:
	git pull
clean:
	rm -f *.apk *.out *.exe *.o *.a *.res
	make -s -C app clean
	make -s -C library clean
	make -s -C res clean
	make -s -C tool clean
