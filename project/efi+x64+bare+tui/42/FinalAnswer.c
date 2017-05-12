#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mStringHelpTokenId = STRING_TOKEN (STR_HELLO_WORLD_HELP_INFORMATION);

EFI_STATUS EFIAPI UefiMain (
IN EFI_HANDLE        ImageHandle,
IN EFI_SYSTEM_TABLE  *SystemTable)
{
	UINT32 j=0;
	UINT16 str[11] = {
		'h',0,
		'a',0,
		'h',0,
		'a',0,
		'\n',0,
		0
	};
	for(j=0; j<16; j++)
	{
    		Print(str);
	}
	return EFI_SUCCESS;
}
