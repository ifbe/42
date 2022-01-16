#include <efi.h>
#include <efilib.h>
#include "libboot.h"
void gethandleandefitab(void** handle, void** table);
void sethandleandefitab(void* handle, void* table);




EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* efitab)
{
	sethandleandefitab(handle, efitab);

	void* all = origin_create(_efimain_, efi_main, 0, 0);
	void* wrk = bootup_create(_kernel_, 0, 0, 0);
	bootup_delete(wrk);
	origin_delete(all);

	return EFI_SUCCESS;
}
//void atexit(){}
