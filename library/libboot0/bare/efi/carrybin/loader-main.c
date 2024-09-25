//#include <efi.h>
//#include <efilib.h>
#include <lib/data.c>
typedef EFI_STATUS (*jumpfunc)(EFI_HANDLE handle, EFI_SYSTEM_TABLE* efitab);

extern unsigned char _binary_start_;
extern unsigned char _binary_end_;

EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* efitab)
{
	efitab->ConOut->OutputString(efitab->ConOut, L"Hello World\r\n");

	unsigned char* dst = (unsigned char*)0x100000;
	int j;
	for(j=0;j<0xf00000;j++)dst[j] = 0;

	efitab->ConOut->OutputString(efitab->ConOut, L"33333333\r\n");

	unsigned char* src = &_binary_start_;
	int size = &_binary_end_ - &_binary_start_;
	for(j=0;j<size;j++)dst[j] = src[j];

	efitab->ConOut->OutputString(efitab->ConOut, L"44444444\r\n");

	jumpfunc realmain = (jumpfunc)0x100000;
	realmain(handle, efitab);

	efitab->ConOut->OutputString(efitab->ConOut, L"22222222\r\n");

	return EFI_SUCCESS;
}