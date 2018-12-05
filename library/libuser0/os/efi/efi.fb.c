#include <efi.h>
#include <efilib.h>
#include "libuser.h"
void gethandleandtable(void* H, void* T);




static EFI_BOOT_SERVICES* service;
static void* screen = 0;




void windowread()
{
}
void windowwrite(struct arena* src)
{
	int j;
	int bpp;
	u32* ibuf = src->buf;
	u32* obuf = screen;

	if(src->fmt == __bgra8888__)bpp = 4;
	else if(src->fmt == __bgra8880__)bpp = 3;
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
void windowstart()
{
}
void windowstop()
{
}
void windowdelete(struct arena* w)
{
}
void windowcreate(struct arena* w)
{
	int ret, num, chosen;
	u64 fmt;
	UINTN size = 0;
	UINTN hlen = 0;
	EFI_HANDLE* hbuf;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
	EFI_PIXEL_BITMASK* pix;

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

		screen = (void*)(gop->Mode->FrameBufferBase);
		pix = &(gop->Mode->Info->PixelInformation);
		if(pix->ReservedMask == 0)fmt = __bgra8880__;
		else fmt = __bgra8888__;

		w->type = __win__;
		w->fmt = fmt;

		w->width = w->stride = 1024;
		w->height = 768;

		w->buf = (void*)0x2000000;
		w->len = gop->Mode->FrameBufferSize;
	}
}




void initwindow()
{
	EFI_HANDLE H;
	EFI_SYSTEM_TABLE* T;
	gethandleandtable(&H, &T);
	service = T->BootServices;
}
void freewindow()
{
}
