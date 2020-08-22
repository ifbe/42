#include <efi.h>
#include <efilib.h>
#include "libuser.h"
#define _bgra8880_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '0')
#define _rgba8880_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '0')
#define _bgra8888_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '8')
#define _rgba8888_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '8')
void gethandleandtable(void* H, void* T);
//
int rgbanode_read(void*,int, void*,int, void*,int, void*,int);
int rgbanode_write(void*,int, void*,int, void*,int, void*,int);




static EFI_BOOT_SERVICES* service;
static void* screen = 0;
static u64 vfmt;
static int stride = 0;
static int w = 0;
static int h = 0;




void windowread(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	rgbanode_read(wnd,foot, stack,sp, arg,key, buf,len);

	int bpp;
	switch(wnd->fmt){
	case _bgra8888_:
		bpp = 4;
		break;
	case _bgra8880_:
		bpp = 3;
		break;
	default:
		return;
	}

	int j;
	u32* ibuf = wnd->rgbabuf;
	u32* obuf = screen;
	for(j=0;j<1024*768-1;j++)
	{
		*obuf = ibuf[j];
		obuf = (void*)obuf + bpp;
	}
}
void windowwrite(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
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
void windowdelete(struct supply* wnd)
{
}
void windowcreate(struct supply* wnd)
{
	//wnd data
	wnd->fmt = _rgba_;
	wnd->vfmt = vfmt;

	wnd->width = w;
	wnd->height = h;

	wnd->fbwidth = w*4;
	//wnd->fbheight = 0;

	wnd->rgbabuf = (void*)0x2000000;
}




void initwindow()
{
	EFI_HANDLE H;
	EFI_SYSTEM_TABLE* T;
	gethandleandtable(&H, &T);

	service = T->BootServices;
	say("@initwindow\n");


	int ret, num, chosen;
	UINTN size = 0;
	UINTN hlen = 0;
	EFI_HANDLE* hbuf;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
	EFI_PIXEL_BITMASK* pix;


	//where is
	ret = service->LocateHandleBuffer(
		ByProtocol,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		&hlen,
		&hbuf
	);
	if(ret != EFI_SUCCESS){say("error@LocateHandleBuffer\n");return;}

	ret = service->HandleProtocol(
		hbuf[0],
		&gEfiGraphicsOutputProtocolGuid,
		(void**)&gop
	);
	if(ret != EFI_SUCCESS){say("error@HandleProtocol\n");return;}


	//prefer 1024x768
	num = 0;
	chosen = -1;
	while(1){
		ret = gop->QueryMode(gop, num, &size, &info);
		if(ret != EFI_SUCCESS)break;

		if((1024 == info->HorizontalResolution) && (768 == info->VerticalResolution)){
			w = 1024;
			h = 768;
			stride = info->PixelsPerScanLine;
			chosen = num;
		}

		say("%d,%d,%d,%x\n",
			info->HorizontalResolution,
			info->VerticalResolution,
			info->PixelsPerScanLine,
			info->PixelFormat
		);
		num++;
	}


	//fallback index0
	if(chosen < 0){
		ret = gop->QueryMode(gop, 0, &size, &info);
		if(ret != EFI_SUCCESS){say("error@QueryMode:0\n");return;}

		w = info->HorizontalResolution;
		h = info->VerticalResolution;
		stride = info->PixelsPerScanLine;
		chosen = 0;
	}


	//set mode
	ret = gop->SetMode(gop, chosen);
	if(ret != EFI_SUCCESS){say("error@SetMode:%d\n", chosen);return;};

	say("%x,%x\n", gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize);

	screen = (void*)(gop->Mode->FrameBufferBase);
	pix = &(gop->Mode->Info->PixelInformation);
	if(pix->ReservedMask == 0){
		vfmt = _bgra8880_;
		stride *= 3;
	}
	else{
		vfmt = _bgra8888_;
		stride *= 4;
	}
}
void freewindow()
{
}
