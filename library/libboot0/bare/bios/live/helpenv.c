//loader = github.com/ifbe/live
#include "libboot.h"
#define RSD_PTR_ hex64('R','S','D',' ','P','T','R',' ')




/*
[    0,  3ff] Real Mode IVT
[  400,  4ff] BDA (BIOS data area)
[  500,7ffff] usable
[80000,9ffff] EBDA (Extended BIOS Data Area) maybe used by smm mode
[a0000,BFFFF] Video display memory
[C0000,C7FFF] Video BIOS
[C8000,EFFFF] BIOS Expansions
[F0000,FFFFF] Motherboard BIOS
*/
void* getmemmap()
{
	return (void*)0x1000;
}
void* getdevmap()
{
	int j;
	u64* p = (void*)0xe0000;
	for(j=0;j<0x4000;j++){
		if(RSD_PTR_ == p[j])return &p[j];
	}
	return 0;
}




struct fbinfo{
	u32 buf;
	u32 pad0;

	u32 byteperpixel;
	u32 pad1;

	u16 w;
	u16 zerow;
	u32 padd2;

	u16 h;
	u16 zeroh;
	u32 padd3;
};
void getscreen(void** _buf, u64* _fmt, int* _w, int* _h, int* _fbw, int* _fbh)
{
#define screeninfo 0x2000
	struct fbinfo* fb = (struct fbinfo*)0x2000;

	*_buf = (void*)(u64)fb->buf;
	*_fmt = (fb->byteperpixel==4) ? _bgra8888_ : _bgra8880_;
	*_w = 1024;
	*_h = 768;
	*_fbw = fb->byteperpixel*1024;
	*_fbh = fb->byteperpixel*1024*768;
}




void freerunenv()
{
}
void initrunenv()
{
}
