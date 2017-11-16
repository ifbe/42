#include "actor.h"




void carvepoint(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

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

	win->info[8] += 1;
	win->info[9] += 1;
	win->info[10] += 1;
	win->info[12] += 1;

	color[0] = rr;
	color[1] = gg;
	color[2] = bb;

	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;

	vertex[0] = cx;
	vertex[1] = cy;
	vertex[2] = cz;

	index[0] = pcount;
}