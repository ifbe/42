edk ?=
check:
ifeq ($(edk), )
	@echo "usage:"
	@echo "	1.modify MdeModulePkg/MdeModulePkg.dsc"
	@echo "	2.make edk=/where/is/your/edk"
	@exit
else
	chmod +x build.sh
	./build.sh $(edk)
endif
