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
			ev[1] = hex32('k','b','d',0);
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




static u64 parseinfo(EFI_GRAPHICS_PIXEL_FORMAT format, EFI_PIXEL_BITMASK bitmask)
{
	switch(format){
	case PixelRedGreenBlueReserved8BitPerColor:
		return _rgba8888_;
	case PixelBlueGreenRedReserved8BitPerColor:
		return _bgra8888_;
	case PixelBitMask:
		if(bitmask.ReservedMask){
			return _rgba8888_;
		}
		else{
			return _rgba8880_;
		}
	}
	return 0;
}
int bootservice_graphic()
{
	int ret, num, chosen;
	UINTN size = 0, hlen = 0;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;


	//locate protocol
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = 0;
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	T->BootServices->LocateProtocol(&gopGuid, NULL, (void**)&gop);
	if(0 == gop)return 0;

	//query all
	num = 0;
	while(1){
		ret = gop->QueryMode(gop, num, &size, &info);
		if(ret != EFI_SUCCESS)break;

		u64 fmt = parseinfo(info->PixelFormat, info->PixelInformation);
		say("%d: %d,%d, %d,%d, ver=%d,fmt=%.8s\n",
			num,
			info->HorizontalResolution, info->VerticalResolution,
			info->PixelsPerScanLine, 0,
			info->Version, &fmt
		);
		num++;
	}
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
	bootservice_graphic();
}
