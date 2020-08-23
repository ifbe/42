#include <efi.h>
#include <efilib.h>
#include "libboot.h"
#define _bgra8880_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '0')
#define _rgba8880_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '0')
#define _bgra8888_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '8')
#define _rgba8888_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '8')




//
static EFI_HANDLE H;
static EFI_SYSTEM_TABLE* T;
//
static void* screen = 0;
static u64 vfmt;
static int w = 0;
static int h = 0;
static int fbw = 0;
static int fbh = 0;
//
#define MEMMAP_SIZE 1024
static UINT8 memmap[MEMMAP_SIZE * sizeof(EFI_MEMORY_DESCRIPTOR)];




void gethandleandefitab(void** handle, void** table)
{
	*handle = H;
	*table = T;
}
void sethandleandefitab(void* handle, void* table)
{
	H = handle;
	T = table;
}
void getscreen(void** _buf, u64* _fmt, int* _w, int* _h, int* _fbw, int* _fbh)
{
	*_buf = screen;
	*_fmt = vfmt;
	*_w = w;
	*_h = h;
	*_fbw = fbw;
	*_fbh = fbh;
}
void getmemmap()
{
}




void runtimeservice_reboot()
{
	//wrong
	T->RuntimeServices->ResetSystem(0, 0, 0, 0);
}
void runtimeservice_gettime()
{
	//wrong
	T->RuntimeServices->GetTime(0, 0);
}




static u64 ev[4];
void* pollenv()
{
	if(0 == H)return 0;

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




int lowlevel_input()
{
	if(0 == H)return 0;

	int ret;
	EFI_INPUT_KEY key;
	while(1)
	{
		ret = T->ConIn->ReadKeyStroke(T->ConIn, &key);
		if(ret == EFI_SUCCESS)return key.UnicodeChar;	//key.ScanCode;
	}
}
int lowlevel_output(char* buf, int len)
{
	if(0 == H)return 0;

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
int bootservice_graphic()
{
	int ret, num, chosen;
	UINTN size = 0;
	UINTN hlen = 0;
	EFI_HANDLE* hbuf;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
	EFI_PIXEL_BITMASK* pix;


	//where is
	ret = T->BootServices->LocateHandleBuffer(
		ByProtocol,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		&hlen,
		&hbuf
	);
	if(ret != EFI_SUCCESS){
		say("error@LocateHandleBuffer\n");
		return 0;
	}

	ret = T->BootServices->HandleProtocol(
		hbuf[0],
		&gEfiGraphicsOutputProtocolGuid,
		(void**)&gop
	);
	if(ret != EFI_SUCCESS){
		say("error@HandleProtocol\n");
		return 0;
	}


	//prefer 1024x768
	num = 0;
	chosen = -1;
	while(1){
		ret = gop->QueryMode(gop, num, &size, &info);
		if(ret != EFI_SUCCESS)break;

		if((1024 == info->HorizontalResolution) && (768 == info->VerticalResolution)){
			w = 1024;
			h = 768;
			fbw = info->PixelsPerScanLine;
			//fbh = ?
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
		if(ret != EFI_SUCCESS){
			say("error@QueryMode:0\n");
			return 0;
		}

		w = info->HorizontalResolution;
		h = info->VerticalResolution;
		fbw = info->PixelsPerScanLine;
		//fbh = ?
		chosen = 0;
	}

/*
	//set mode
	ret = gop->SetMode(gop, chosen);
	if(ret != EFI_SUCCESS){
		say("error@SetMode:%d\n", chosen);
		return 0;
	};
	say("buf=%llx,len=%x, fmt=%x,inf=%x, w=%d,h=%d, fbw=%d,fbh=%d\n",
		gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize,
		gop->Mode->Info->PixelFormat, gop->Mode->Info->PixelInformation,
		gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution,
		gop->Mode->Info->PixelsPerScanLine, 0
		//gop->Mode->Info->Version
	);

	screen = (void*)(gop->Mode->FrameBufferBase);
	pix = &(gop->Mode->Info->PixelInformation);
	if(pix->ReservedMask == 0){
		vfmt = _bgra8880_;
		fbw *= 3;
		fbh = gop->Mode->FrameBufferSize;
	}
	else{
		vfmt = _bgra8888_;
		fbw *= 4;
		fbh = gop->Mode->FrameBufferSize;
	}
*/
	return 0;
}
int bootservice_exit()
{
	//GetMemoryMap
	UINTN memmap_size = MEMMAP_SIZE;
	UINTN map_key;
	UINTN descriptor_size;
	UINT32 descriptor_version;
	int ret = T->BootServices->GetMemoryMap(
		&memmap_size,
		(EFI_MEMORY_DESCRIPTOR*)memmap,
		&map_key,
		&descriptor_size,
		&descriptor_version
	);
	if(EFI_SUCCESS != ret){
		say("error:%d@GetMemoryMap\n", ret);
	}

	//ExitBootService
	ret = T->BootServices->ExitBootServices(H, map_key);
	if(EFI_SUCCESS != ret){
		say("error:%d@ExitBootServices\n", ret);
	}

	return 0;
}




void freeserial()
{
}
void initserial()
{
}
void freerunenv()
{
}
void initrunenv()
{
}
