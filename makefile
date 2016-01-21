target?=none+x86+linux+cli
########################################help###################################
help:
	@echo "###############################################################"
	@echo "usage:"
	@echo "make vendor+cpu+system+ui"
	@echo "example:"
	@echo "make none+x86+win+cli"
	@echo "make none+x86+linux+xlib"
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
	make -s -C app/android
apk:
	make -s -C app/apk
ipa:
	make -s -C app/ipa
###############################################################################




########################################help###################################
x86+linux+cli:
	make -C app/none+x86+linux+cli
x86+mac+cli:
	make -C app/none+x86+mac+cli
x86+win+cli:
	make -C app/none+x86+win+cli
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	gcc res/uac.res app/none+x86+win+cli/library.a -o a.exe




x86+linux+fb:
	make -C app/none+x86+linux+fb
x86+linux+xlib:
	make -C app/none+x86+linux+xlib
x86+linux+sdl:
	make -C app/none+x86+linux+sdl
x86+linux+web:
	make -C app/none+x86+linux+web




x86+mac+sdl:
	make -C app/none+x86+mac+sdl




x86+win+api:
	make -C app/none+x86+win+api
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	gcc -mwindows res/uac.res app/none+x86+win+api/library.a -o a.exe
x86+win+sdl:
	make -C app/none+x86+win+sdl
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	gcc -mwindows res/uac.res app/none+x86+win+api/library.a -o a.exe
x86+win+web:
	make -C app/none+x86+win+web
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	gcc -mwindows res/uac.res app/none+x86+win+api/library.a -o a.exe




push:
	git add --all .
	git commit -a
	git push
pull:
	git pull
clean:
	rm -f *.out *.exe
	rm -f */*.o
	rm -f */*.a
	rm -f */*.res
	make -s -C app clean
	make -s -C library clean
	make -s -C res clean
	make -s -C tool clean
