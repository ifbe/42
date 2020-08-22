#include <efi.h>
#include <efilib.h>
#include "libboot.h"
#define MEMMAP_SIZE 1024
static UINT8 memmap[MEMMAP_SIZE * sizeof(EFI_MEMORY_DESCRIPTOR)];




static u64 ev[4];
static EFI_HANDLE H;
static EFI_SYSTEM_TABLE* T;
EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *table)
{
	H = handle;
	T = table;

	int ret = table->ConOut->OutputString(table->ConOut, L"42!!\r\n");
	if(EFI_ERROR(ret))return ret;

	ret = table->ConIn->Reset(table->ConIn, FALSE);
	if(EFI_ERROR(ret))return ret;


	void* all = origincreate(_efimain_, efi_main, 0, 0);

    UINTN memmap_size = MEMMAP_SIZE;
    UINTN map_key;
	UINTN descriptor_size;
    UINT32 descriptor_version;
	ret = T->BootServices->GetMemoryMap(
		&memmap_size,
		(EFI_MEMORY_DESCRIPTOR*)memmap,
		&map_key,
		&descriptor_size,
		&descriptor_version
	);
    if(EFI_SUCCESS != ret){
		say("error:%d@GetMemoryMap\n", ret);
	}

	ret = T->BootServices->ExitBootServices(handle, map_key);
	if(EFI_SUCCESS != ret){
		say("error:%d@ExitBootServices\n", ret);
	}
	H = 0;
	T = 0;


	void* wrk = workercreate(_kernel_, 0, 0, 0);
	workerdelete(wrk);
	origindelete(all);

	return EFI_SUCCESS;
}
void gethandleandtable(void** handle, void** table)
{
	*handle = H;
	*table = T;
}
void atexit(){}




int lowlevel_input(void* buf)
{
	if(0==H)return 0;
	if(0==T)return 0;

	int ret;
	while(1)
	{
		ret = T->ConIn->ReadKeyStroke(T->ConIn, buf);
		if(ret == EFI_SUCCESS)return 1;
	}
}
int lowlevel_output(char* buf, int len)
{
	if(0==H)return 0;
	if(0==T)return 0;

	int j;
	unsigned short temp[2] = {0,0};
	for(j=0;j<len;j++)
	{
		if(buf[j] == '\n')
		{
			T->ConOut->OutputString(T->ConOut, L"\r\n");
		}
		else
		{
			temp[0] = buf[j];
			T->ConOut->OutputString(T->ConOut, temp);
		}
	}
	return 0;
}



/*
void* birth()
{
	int j;
	u8* mem = (u8*)0x1000000;

	for(j=0;j<0x1000000;j++)mem[j] = 0;
	return mem;
}
void death()
{
}*/




void* pollenv()
{
	if(0==H)return 0;
	if(0==T)return 0;

	int ret = T->ConIn->ReadKeyStroke(T->ConIn, (void*)ev);
	if(ret != EFI_SUCCESS)return 0;

	if(ev[0] <= 0x10000)
	{
		if(ev[0] == 0x17)
		{
			ev[0] = 0x1b;
			ev[1] = hex32('c','h','a','r');
			return ev;
		}
		else if((ev[0]>=1)&&(ev[0]<=4))
		{
			if(ev[0] == 1)ev[0] = 0x48;
			else if(ev[0] == 2)ev[0] = 0x50;
			else if(ev[0] == 3)ev[0] = 0x4d;
			else if(ev[0] == 4)ev[0] = 0x4b;
			ev[1] = hex32('k','b','d',0);
			return ev;
		}
		else if((ev[0]>=0xb)&&(ev[0]<=0x16))
		{
			ev[0] = ev[0]-0xb+0xf1;
			ev[1] = hex32('k','b','d',0);
			return ev;
		}
		else
		{
			//say("%x\n",ev[0]);
			return 0;
		}
	}

	ev[0] >>= 16;
	ev[1] = hex32('c','h','a','r');
	return ev;
}
void* waitenv()
{
	return 0;
}



/*
int arg2utf8(u8* src, u8* dst)
{
	int j = 0;
	while(src[j] >= 0x20)
	{
		dst[j] = src[j];
		j++;
	}

	dst[j] = 0;
	return j;
}*/
