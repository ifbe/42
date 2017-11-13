#include "actor.h"




void carveline(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[14];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xe00000 + (icount*2);

	win->info[8] += 2;
	win->info[9] += 2;
	win->info[10] += 2;
	win->info[13] += 2;

	color[0] = rr;
	color[1] = gg;
	color[2] = bb;
	color[3] = rr;
	color[4] = gg;
	color[5] = bb;

	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;
	normal[3] = 0.0;
	normal[4] = 0.0;
	normal[5] = 1.0;

	vertex[0] = x1;
	vertex[1] = y1;
	vertex[2] = z1;
	vertex[3] = x2;
	vertex[4] = y2;
	vertex[5] = z2;

	index[0] = pcount;
	index[1] = pcount+1;
}
void carvebezier(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float xc, float yc, float zc)
{
}