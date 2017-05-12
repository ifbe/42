ln -s `pwd`/42 $1/MdeModulePkg/Application/42
ln -s `pwd`/../../library $1/MdeModulePkg/Application/42/library
cd $1
export EDK_TOOLS_PATH=`pwd`/BaseTools
source ./edksetup.sh BaseTools
build -m MdeModulePkg/Application/42/FinalAnswer.inf
ls Build/MdeModule/DEBUG_UNIXGCC/X64/
rm MdeModulePkg/Application/42/library
rm MdeModulePkg/Application/42
