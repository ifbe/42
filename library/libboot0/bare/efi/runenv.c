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




int bootservice_input(void* buf)
{
	if(0 == H)return 0;

	int ret;
	while(1)
	{
		ret = T->ConIn->ReadKeyStroke(T->ConIn, buf);
		if(ret == EFI_SUCCESS)return 1;
	}
}
int bootservice_output(char* buf, int len)
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
	EFI_PIXEL_BITMASK* pix;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;


	//locate protocol
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	T->BootServices->LocateProtocol(&gopGuid, NULL, (void**)&gop);
/*
	EFI_HANDLE* hbuf;

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
*/

	//prefer 1024x768
	num = 0;
	chosen = -1;
	while(1){
		ret = gop->QueryMode(gop, num, &size, &info);
		if(ret != EFI_SUCCESS)break;

		if((1 == info->HorizontalResolution) && (768 == info->VerticalResolution)){
			chosen = num;
		}

		say("%d: %d,%d,%d,%x\n",
			num,
			info->HorizontalResolution,
			info->VerticalResolution,
			info->PixelsPerScanLine,
			info->PixelFormat
		);
		num++;
	}


	//fallback current
	if(chosen < 0){
		chosen = gop->Mode->Mode;
	}
	say("chosen=%d\n",chosen);


	//query mode
	ret = gop->QueryMode(gop, chosen, &size, &info);
	if(ret != EFI_SUCCESS){
		say("error@QueryMode:0\n");
		return 0;
	}
	w = info->HorizontalResolution;
	h = info->VerticalResolution;
	fbw = info->PixelsPerScanLine;
	//fbh = ?
	say("cur: buf=%llx,len=%x\n", gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize);
	say("cur: fmt=%x,inf=%x, w=%d,h=%d, fbw=%d,fbh=%d\n",
		gop->Mode->Info->PixelFormat, gop->Mode->Info->PixelInformation,
		gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution,
		gop->Mode->Info->PixelsPerScanLine, 0
		//gop->Mode->Info->Version
	);


	//some uefi are buggy, don't setmode!
/*	ret = gop->SetMode(gop, chosen);
	if(ret != EFI_SUCCESS){
		say("error@SetMode:%d\n", chosen);
		return 0;
	}*/


	//preserve infomation
	if(gop->Mode){
		say("new: buf=%llx,len=%x\n", gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize);
		screen = (void*)(gop->Mode->FrameBufferBase);

		if(gop->Mode->Info){
			say("new: fmt=%x,inf=%x, w=%d,h=%d, fbw=%d,fbh=%d\n",
				gop->Mode->Info->PixelFormat, gop->Mode->Info->PixelInformation,
				gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution,
				gop->Mode->Info->PixelsPerScanLine, 0
				//gop->Mode->Info->Version
			);
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
		}
	}

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




void freerunenv()
{
}
void initrunenv()
{
	//screen 1024x768x32
	T->ConOut->OutputString(T->ConOut, L"1111\r\n");
	bootservice_graphic();
 
	//free bootservice
	T->ConOut->OutputString(T->ConOut, L"2222\r\n");
	bootservice_exit();

	//nomore bootservice
	H = 0;
}
