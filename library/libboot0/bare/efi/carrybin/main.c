//#include <efi.h>
//#include <efilib.h>
#include <lib/data.c>


extern unsigned char _binary_start_;
extern unsigned char _binary_end_;


EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* efitab)
{
	int size = _binary_end_ - _binary_start_;
	return EFI_SUCCESS;
}
//void atexit(){}
