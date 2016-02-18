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
x64+linux+cli:
	make -C app/none+x64+linux+cli --no-print-directory
	cp app/none+x64+linux+cli/a.out .
x64+mac+cli:
	make -C app/none+x64+mac+cli --no-print-directory
	cp app/none+x64+mac+cli/a.exe .
x64+win+cli:
	windres --input-format=rc -O coff -i res/uac.rc -o res/uac.res
	make -C app/none+x64+win+cli --no-print-directory
	cp app/none+x64+win+cli/a.out .




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




x64+mac+sdl:
	make -C app/none+x64+mac+sdl --no-print-directory
	cp app/none+x64+mac+sdl/a.out .




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
	rm -f *.out *.exe
	rm -f */*.o
	rm -f */*.a
	rm -f */*.res
	make -s -C app clean
	make -s -C library clean
	make -s -C res clean
	make -s -C tool clean
