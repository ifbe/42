0:homepage
	http://www.tianocore.org/edk2
	https://github.com/tianocore/tianocore.github.io/wiki/Common-instructions-for-Unix

1:download
	sudo apt-get install build-essential texinfo bison flex		#ubuntu
	sudo apt-get install libgmp3-dev libmpfr-dev libuuid-dev	#ubuntu
	sudo apt-get install mingw-w64 acpica-tools
	git clone https://github.com/tianocore/edk2
	git clone https://github.com/tianocore/edk2-FatPkg

2:build
	cd edk2/
	make -C BaseTools

	#cd BaseTools/gcc
	#./mingw-gcc-build.py --arch=x64 --prefix=/opt/efi
	#cd ../../

	source ./edksetup.sh BaseTools

3:modify Conf/tools_def.txt:
	UNIXGCC_X64_PETOOLS_PREFIX=x86_64-w64-mingw32-

4:modify Conf/target.txt:
	ACTIVE_PLATFORM       = MdeModulePkg/MdeModulePkg.dsc
	TARGET_ARCH           = X64
	TOOL_CHAIN_TAG        = UNIXGCC

	#DEFINE UNIX_IASL_BIN  = /opt/iasl

5:environment
	export EDK_TOOLS_PATH=`pwd`/BaseTools
	source ./edksetup.sh BaseTools

6:sample
	build -m MdeModulePkg/Application/HelloWorld/HelloWorld.inf
	ls Build/MdeModule/DEBUG_UNIXGCC/X64/
