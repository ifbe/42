#include <efi.h>
#include <efilib.h>
#include "arena.h"
void gethandleandtable(void* H, void* T);
void say(void*, ...);




static EFI_BOOT_SERVICES* service;




void windowread()
{
}
void windowwrite(struct window* dst, struct window* src)
{
	int j;
	u32* ibuf = src->buf;
	u32* obuf = dst->buf;
	for(j=0;j<1024*768-1;j++)
	{
		*obuf = ibuf[j];
		obuf = (void*)obuf + 3;
	}
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstart(struct window* this)
{
	int ret, num, chosen;
	UINTN size = 0;
	UINTN hlen = 0;
	EFI_HANDLE* hbuf;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;

	if(this->type == hex32('b','u','f',0))
	{
	}
	else
	{
		ret = service->LocateHandleBuffer(
			ByProtocol,
			&gEfiGraphicsOutputProtocolGuid,
			NULL,
			&hlen,
			&hbuf
		);
		if(ret != EFI_SUCCESS){say("@LocateHandleBuffer\n");return;}

		ret = service->HandleProtocol(
			hbuf[0],
			&gEfiGraphicsOutputProtocolGuid,
			(void**)&gop
		);
		if(ret != EFI_SUCCESS){say("@HandleProtocol\n");return;}

		num = 0;
		chosen = -1;
		while(1)
		{
			ret = gop->QueryMode(gop, num, &size, &info);
			if(ret != EFI_SUCCESS)break;

			if((info->HorizontalResolution==1024)
			&&(info->VerticalResolution == 768))
			{
				chosen = num;
				break;
			}

			say("%x,%x,%x\n",
				info->HorizontalResolution,
				info->VerticalResolution,
				info->PixelFormat
			);
			num++;
		}
		if(chosen >= 0)
		{
			ret = gop->SetMode(gop, chosen);
			if(ret != EFI_SUCCESS){say("@SetMode\n");return;};

			say("%x,%x\n",
				gop->Mode->FrameBufferBase,
				gop->Mode->FrameBufferSize
			);
		}

		this->type = hex32('w','i','n',0);
		this->fmt = hex64('b','g','r','a','8','8','8','8');
		this->buf = (void*)(gop->Mode->FrameBufferBase);
		this->len = gop->Mode->FrameBufferSize;
		this->w = 1024;
		this->h = 768;
	}
}
void windowstop()
{
}
void windowcreate()
{
	EFI_HANDLE H;
	EFI_SYSTEM_TABLE* T;
	gethandleandtable(&H, &T);
	service = T->BootServices;
}
void windowdelete()
{
}
