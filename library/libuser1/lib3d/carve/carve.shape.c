#include "actor.h"
#define accuracy 18
#define PI 3.1415926535897932384626433832795028841971693993151
u32 getrandom();




void select_3d(struct arena* win, struct style* sty)
{
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;
	float d = (w+h)/2;

	carveline_prism4(
		win, 0xff0000,
		cx, cy, d/2,
		w/2, 0.0, 0.0,
		0.0, h/2, 0.0,
		0.0, 0.0, d/2
	);
}
void carveaxis(struct arena* win)
{
	carveline(win, 0xff0000,
		0.0, 0.0, 0.0,
		10.0, 0.0, 0.0);
	carveline(win, 0xff00,
		0.0, 0.0, 0.0,
		0.0, 10.0, 0.0);
	carveline(win, 0xff,
		0.0, 0.0, 0.0,
		0.0, 0.0, 10.0);
}
void carvestarry_random(struct arena* win)
{
	u32 j,k;
	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[12];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xc00000 + (icount*2);

	win->info[8] += 256;
	win->info[9] += 256;
	win->info[10] += 256;
	win->info[12] += 256;

	for(j=0;j<256;j++)
	{
		vertex[j*3+0] = (getrandom() % 65536) / 65536.0 - 0.5;
		vertex[j*3+1] = (getrandom() % 65536) / 65536.0 - 0.5;
		vertex[j*3+2] = (getrandom() % 65536) / 65536.0;

		normal[j*3+0] = 0.0;
		normal[j*3+1] = 0.0;
		normal[j*3+2] = 1.0;

		k = getrandom();
		color[j*3+0] = (float)(k&0xff) / 256.0;
		color[j*3+1] = (float)((k>>8)&0xff) / 256.0;
		color[j*3+2] = (float)((k>>16)&0xff) / 256.0;

		index[j] = pcount+j;
	}
}
void carveneural_random(struct arena* win)
{
	u32 j,k;
	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[12];
	u32 lcount = win->info[13];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* iindex   = buf + 0xc00000 + (icount*2);
	u16* lindex   = buf + 0xd00000 + (lcount*2);

	win->info[8] += 256;
	win->info[9] += 256;
	win->info[10] += 256;
	win->info[12] += 256;
	win->info[13] += 256;

	for(j=0;j<256;j++)
	{
		vertex[j*3+0] = (getrandom() % 65536) / 65536.0 - 0.5;
		vertex[j*3+1] = (getrandom() % 65536) / 65536.0 - 0.5;
		vertex[j*3+2] = (getrandom() % 65536) / 65536.0;

		normal[j*3+0] = 0.0;
		normal[j*3+1] = 0.0;
		normal[j*3+2] = 1.0;

		k = getrandom();
		color[j*3+0] = (float)(k&0xff) / 256.0;
		color[j*3+1] = (float)((k>>8)&0xff) / 256.0;
		color[j*3+2] = (float)((k>>16)&0xff) / 256.0;

		iindex[j] = pcount+j;
		lindex[j] = pcount+j;
	}
}
