#include "actor.h"
#define accuracy 18
#define PI 3.1415926535897932384626433832795028841971693993151
u32 getrandom();




void carveaxis(struct arena* win)
{
	carveline(win, 0xff0000, 0.0, 0.0, 0.0, 10000.0, 0.0, 0.0);
	carveline(win, 0x00ff00, 0.0, 0.0, 0.0, 0.0, 10000.0, 0.0);
	carveline(win, 0x0000ff, 0.0, 0.0, 0.0, 0.0, 0.0, 10000.0);
}
void carvestarry_random(struct arena* win)
{
	u32 j,k;
	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->pointcount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (ccount*12);
	u16* index     = buf + 0x800000 + (icount*2);

	win->vertexcount  += 256;
	win->normalcount  += 256;
	win->colorcount   += 256;
	win->texturecount += 256;
	win->pointcount   += 256;

	for(j=0;j<256;j++)
	{
		vertex[j*3+0] = (getrandom() % 32768) / 32768.0 - 0.5;
		vertex[j*3+1] = (getrandom() % 32768) / 32768.0 - 0.5;
		vertex[j*3+2] = (getrandom() % 32768) / 32768.0;

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
	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->pointcount;
	u32 lcount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (ccount*12);
	u16* iindex    = buf + 0x800000 + (icount*2);
	u16* lindex    = buf + 0xa00000 + (lcount*2);

	win->vertexcount  += 256;
	win->normalcount  += 256;
	win->colorcount   += 256;
	win->texturecount += 256;
	win->pointcount   += 256;
	win->linecount    += 256;

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
