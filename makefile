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
	make -s -C app/android --no-print-directory
apk:
	make -s -C app/apk --no-print-directory
ipa:
	make -s -C app/ipa --no-print-directory
uwp:
	make -s -C app/uwp --no-print-directory
###############################################################################




########################################help###################################
arm+linux+cli:
	make -C app/none+arm+linux+cli --no-print-directory
	cp app/none+arm+linux+cli/a.out .
mips+linux+cli:
	make -C app/none+mips+linux+cli --no-print-directory
	cp app/none+mips+linux+cli/a.out .
x86+linux+cli:
	make -C app/none+x86+linux+cli --no-print-directory
	cp app/none+x86+linux+cli/a.out .
x86+mac+cli:
	make -C app/none+x86+mac+cli --no-print-directory
	cp app/none+x86+mac+cli/a.exe .
x86+win+cli:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x86+win+cli --no-print-directory
	cp app/none+x86+win+cli/a.out .




x86+linux+fb:
	make -C app/none+x86+linux+fb --no-print-directory
	cp app/none+x86+linux+fb/a.out .
x86+linux+xlib:
	make -C app/none+x86+linux+xlib --no-print-directory
	cp app/none+x86+linux+xlib/a.out .
x86+linux+sdl:
	make -C app/none+x86+linux+sdl --no-print-directory
	cp app/none+x86+linux+sdl/a.out .
x86+linux+web:
	make -C app/none+x86+linux+web --no-print-directory
	cp app/none+x86+linux+web/a.out .




x86+mac+sdl:
	make -C app/none+x86+mac+sdl --no-print-directory
	cp app/none+x86+mac+sdl/a.out .




x86+win+api:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x86+win+api --no-print-directory
	cp app/none+x86+win+api/a.exe .
x86+win+sdl:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x86+win+sdl --no-print-directory
	cp app/none+x86+win+sdl/a.exe .
x86+win+web:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x86+win+web --no-print-directory
	cp app/none+x86+win+web/a.exe .




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
