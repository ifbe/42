#include <stdint.h>
#include <efi.h>
#include <efilib.h>
#include "libboot.h"




//
static EFI_HANDLE H;
static EFI_SYSTEM_TABLE* T;
//tables
static u8 table_mps[]     = {0x2f, 0x2d, 0x9d, 0xeb, 0x88, 0x2d, 0xd3, 0x11, 0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d};
static u8 table_acpi[]    = {0x30, 0x2d, 0x9d, 0xeb, 0x88, 0x2d, 0xd3, 0x11, 0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d};
static u8 table_acpi2[]   = {0x71, 0xe8, 0x68, 0x88, 0xf1, 0xe4, 0xd3, 0x11, 0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81};
static u8 table_smbios[]  = {0x31, 0x2d, 0x9d, 0xeb, 0x88, 0x2d, 0xd3, 0x11, 0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d};
static u8 table_smbios3[] = {0x44, 0x15, 0xfd, 0xf2, 0x94, 0x97, 0x2c, 0x4a, 0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94};
static u8 table_sal[]     = {0x32, 0x2d, 0x9d, 0xeb, 0x88, 0x2d, 0xd3, 0x11, 0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d};
static void* rsdptr = 0;
//screen
static void* lfb = 0;
static u64 fmt;
static int w = 0;
static int h = 0;
static int fbw = 0;
static int fbh = 0;
//memmap
#define BYTE_PER_DESC 0x30
#define DESC_PER_UEFI 4096
static u8 memmap[BYTE_PER_DESC * DESC_PER_UEFI];




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
	*_buf = lfb;
	*_fmt = fmt;
	*_w = w;
	*_h = h;
	*_fbw = fbw;
	*_fbh = fbh;
}
void* getmemmap()
{
	return memmap;
}
void* getdevmap()
{
    return rsdptr;
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
	default:break;
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
		logtoall("error@LocateHandleBuffer\n");
		return 0;
	}

	ret = T->BootServices->HandleProtocol(
		hbuf[0],
		&gEfiGraphicsOutputProtocolGuid,
		(void**)&gop
	);
	if(ret != EFI_SUCCESS){
		logtoall("error@HandleProtocol\n");
		return 0;
	}
*/

	//prefer 1024x768
	num = 0;
	chosen = -1;
	while(1){
		ret = gop->QueryMode(gop, num, &size, &info);
		if(ret != EFI_SUCCESS)break;
/*
		if((1024 == info->HorizontalResolution) && (768 == info->VerticalResolution)){
			chosen = num;
		}
*/
		fmt = parseinfo(info->PixelFormat, info->PixelInformation);
		logtoall("%d: %d,%d, %d,%d, ver=%d,fmt=%.8s\n",
			num,
			info->HorizontalResolution, info->VerticalResolution,
			info->PixelsPerScanLine, 0,
			info->Version, &fmt
		);
		num++;
	}


	//fallback current
	if(chosen < 0){
		chosen = gop->Mode->Mode;
	}
	logtoall("chosen=%d\n",chosen);


	//query mode
	ret = gop->QueryMode(gop, chosen, &size, &info);
	if(ret != EFI_SUCCESS){
		logtoall("error@QueryMode:0\n");
		return 0;
	}
	w = info->HorizontalResolution;
	h = info->VerticalResolution;
	fbw = info->PixelsPerScanLine;
	//fbh = ?


	//some uefi are buggy, don't setmode!
/*	ret = gop->SetMode(gop, chosen);
	if(ret != EFI_SUCCESS){
		logtoall("error@SetMode:%d\n", chosen);
		return 0;
	}*/


	//preserve infomation
	if(gop->Mode){
		logtoall("buf=%llx,len=%x\n", gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize);
		lfb = (void*)(gop->Mode->FrameBufferBase);
		fbh = gop->Mode->FrameBufferSize;

		if(gop->Mode->Info){
			fmt = parseinfo(gop->Mode->Info->PixelFormat, gop->Mode->Info->PixelInformation);
			switch(fmt){
				case _bgra8888_:
				case _rgba8888_:
					fbw *= 4;
					break;
				case _bgra8880_:
				case _rgba8880_:
					fbw *= 3;
					break;
				default:
					lfb = 0;
			}

			logtoall("w=%d,h=%d, fbw=%d,fbh=%d, ver=%d,fmt=%.8s\n",
				gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution,
				gop->Mode->Info->PixelsPerScanLine, 0,
				gop->Mode->Info->Version, &fmt
			);
		}
	}

	return 0;
}
int bootservice_exit()
{
	//GetMemoryMap
	UINT32 ver;
	UINTN key;
	UINTN byteperuefi = BYTE_PER_DESC * DESC_PER_UEFI;
	UINTN byteperdesc;
	int ret = T->BootServices->GetMemoryMap(
		&byteperuefi,
		(EFI_MEMORY_DESCRIPTOR*)memmap,
		&key,
		&byteperdesc,
		&ver
	);
	if(EFI_SUCCESS != ret){
		logtoall("error:%d@GetMemoryMap\n", ret);
		return -1;
	}
	else{
		logtoall("@GetMemoryMap:buf=%p,len=%x,key=%x,sz=%x,ver=%x\n",
			memmap, byteperuefi,
			key,
			byteperdesc,
			ver
		);

		int j;
		EFI_MEMORY_DESCRIPTOR* desc;
		for(j=0;j<byteperuefi / byteperdesc;j++){
			desc = (void*)(memmap + byteperdesc*j);
			logtoall("%04x: type=%x, pbuf=%llx, vbuf=%llx, size=%llx, attr=%llx\n",
				j,
				desc->Type,
				desc->PhysicalStart,
				desc->VirtualStart,
				desc->NumberOfPages << 12,
				desc->Attribute
			);
		}
	}

	//ExitBootService
	int j;
	for(j=0;j<3;j++){
		//key may change after each call to exitbootservice

		byteperuefi = BYTE_PER_DESC * DESC_PER_UEFI;
		ret = T->BootServices->GetMemoryMap(
			&byteperuefi,
			(EFI_MEMORY_DESCRIPTOR*)memmap,
			&key,
			&byteperdesc,
			&ver
		);
		if(EFI_SUCCESS != ret){
			logtoall("error:%d,retry:%d@GetMemoryMap\n", ret, j);
			continue;
		}

		ret = T->BootServices->ExitBootServices(H, key);
		if(EFI_SUCCESS != ret){
			logtoall("error:%d,retry:%d@ExitBootServices\n", ret, j);
			continue;
		}

		break;
	}

	return 0;
}
int bootservice_input(void* buf)
{
	if(0 == H)return 0;

	int ret;
	while(1){
		ret = T->ConIn->ReadKeyStroke(T->ConIn, buf);
		if(ret == EFI_SUCCESS)return 1;
	}
	return 0;
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




void uefi_version()
{
	logtoall("efiver=%x\n", T->Hdr.Revision);
}
void uefi_tables()
{
	//uefi version
	int j;
	for(j=0;j<T->NumberOfTableEntries;j++){
		if(0 == ncmp(&T->ConfigurationTable[j].VendorGuid, &table_mps, 16)){
			logtoall("@%p: mps\n", T->ConfigurationTable[j].VendorTable);
			printmemory(T->ConfigurationTable[j].VendorTable, 16);
			continue;
		}
		if(0 == ncmp(&T->ConfigurationTable[j].VendorGuid, &table_acpi, 16)){
			logtoall("@%p: acpi\n", T->ConfigurationTable[j].VendorTable);
			printmemory(T->ConfigurationTable[j].VendorTable, 16);
			rsdptr = T->ConfigurationTable[j].VendorTable;
			continue;
		}
		if(0 == ncmp(&T->ConfigurationTable[j].VendorGuid, &table_acpi2, 16)){
			logtoall("@%p: acpi2\n", T->ConfigurationTable[j].VendorTable);
			printmemory(T->ConfigurationTable[j].VendorTable, 16);
			rsdptr = T->ConfigurationTable[j].VendorTable;
			continue;
		}
		if(0 == ncmp(&T->ConfigurationTable[j].VendorGuid, &table_smbios, 16)){
			logtoall("@%p: smbios\n", T->ConfigurationTable[j].VendorTable);
			printmemory(T->ConfigurationTable[j].VendorTable, 16);
			continue;
		}
		if(0 == ncmp(&T->ConfigurationTable[j].VendorGuid, &table_smbios3, 16)){
			logtoall("@%p: smbios3\n", T->ConfigurationTable[j].VendorTable);
			printmemory(T->ConfigurationTable[j].VendorTable, 16);
			continue;
		}
		if(0 == ncmp(&T->ConfigurationTable[j].VendorGuid, &table_sal, 16)){
			logtoall("@%p: sal\n", T->ConfigurationTable[j].VendorTable);
			printmemory(T->ConfigurationTable[j].VendorTable, 16);
			continue;
		}
		else{
			u32* p = (u32*)&T->ConfigurationTable[j].VendorGuid;
			logtoall("@%p: unknown: 0x%x,0x%x,0x%x,0x%x\n", T->ConfigurationTable[j].VendorTable, p[0], p[1], p[2], p[3]);
			printmemory(T->ConfigurationTable[j].VendorTable, 16);
		}
	}
}




void freerunenv()
{
}
void initrunenv()
{
	uefi_version();

	uefi_tables();

	//screen 1024x768x32
	bootservice_graphic();
 
	//free bootservice
	bootservice_exit();

	//nomore bootservice
	H = 0;
}
