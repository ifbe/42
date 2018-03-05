#include <efi.h>
#include <efilib.h>
#include "arena.h"
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define __buf__ hex32('b','u','f',0)
#define __win__ hex32('w','i','n',0)
#define __bgra8888__ hex64('b','g','r','a','8','8','8','8')
#define __bgra8880__ hex64('b','g','r','a','8','8','8','0')
void gethandleandtable(void* H, void* T);




static EFI_BOOT_SERVICES* service;




void windowread()
{
}
void windowwrite(struct window* dst, struct window* src)
{
	int j;
	int bpp;
	u32* ibuf = src->buf;
	u32* obuf = dst->buf;

	if(dst->fmt == __bgra8888__)bpp = 4;
	else if(dst->fmt == __bgra8880__)bpp = 3;
	else return;

	for(j=0;j<1024*768-1;j++)
	{
		*obuf = ibuf[j];
		obuf = (void*)obuf + bpp;
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
	u64 fmt;
	UINTN size = 0;
	UINTN hlen = 0;
	EFI_HANDLE* hbuf;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
	EFI_PIXEL_BITMASK* pix;

	if(this->type == __buf__)
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

			pix = &(gop->Mode->Info->PixelInformation);
			if(pix->ReservedMask == 0)fmt = __bgra8880__;
			else fmt = __bgra8888__;

			this->w = 1024;
			this->h = 768;
			this->type = __win__;
			this->fmt = fmt;
			this->buf = (void*)(gop->Mode->FrameBufferBase);
			this->len = gop->Mode->FrameBufferSize;
		}
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
