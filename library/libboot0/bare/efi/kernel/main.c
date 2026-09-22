#include <lib/data.c>
#include "libboot.h"
void setentryandstack(void* entry, void* stack);
void sethandleandefitab(void* handle, void* table);
//
void example_efi();


EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* efitab)
{
	u8 tmp;
	setentryandstack(efi_main, &tmp);
	sethandleandefitab(handle, efitab);

	example_efi();

	return EFI_SUCCESS;
}
void _relocate()
{
}