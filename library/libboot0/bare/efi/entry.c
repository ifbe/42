#include <stdint.h>
#include <efi.h>
#include <efilib.h>
#include "libboot.h"
void gethandleandefitab(void** handle, void** table);
void sethandleandefitab(void* handle, void* table);




EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* efitab)
{
	sethandleandefitab(handle, efitab);

	//init world, store args
	void* all = origin_alloc_fromarg(_efimain_, efi_main, 0 ,0);
	origin_create(all, 0, 0, 0);
	//call subcmd, until return
	void* thr = bootup_alloc_fromtype(_kernel_);
	bootup_create(thr, 0, 0, 0);

	bootup_delete(thr);
	origin_delete(all);

	return EFI_SUCCESS;
}
//void atexit(){}
