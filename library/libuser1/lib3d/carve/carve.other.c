#include "actor.h"
#define acc 18
#define stlv 0x25
#define PI 3.1415926535897932384626433832795028841971693993151
u32 getrandom();




struct texandobj
{
	u32 obj;
	u32 len;
	void* buf;
};
struct eachone
{
	u32 program;
	u32 vao;
	u32 vbo;
	u32 ibo;
	u32 tex0;
	u32 tex1;
	float light0vertex[3];
	float light0color[3];
	float light1vertex[3];
	float light1color[3];
	float modmat[4][4];
};




void carveaxis(struct arena* win)
{
	carveline(win, 0xff0000, 0.0, 0.0, 0.0, 10000.0, 0.0, 0.0);
	carveline(win, 0x00ff00, 0.0, 0.0, 0.0, 0.0, 10000.0, 0.0);
	carveline(win, 0x0000ff, 0.0, 0.0, 0.0, 0.0, 0.0, 10000.0);
}
void carvestarry_random(struct arena* win)
{
/*
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
*/
}
void carveneural_random(struct arena* win)
{
/*
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
*/
}
void carvestl(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float sx, float sy, float sz,
	void* stlbuf, int stllen, float f)
{
	float* p;
	int j, k, ret;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int vlen = mod[stlv].len;
	float* vbuf = (mod[stlv].buf) + (36*vlen);

	ret = *(u32*)(stlbuf+80);
	ret = ret%(0x1000000/36);
	mod[stlv].len += ret*3;

	for(j=0;j<ret;j++)
	{
		p = (void*)stlbuf + 84 + j*50;
		k = j*27;

		vbuf[k+ 0] = cx + (p[3]-sx)*f;
		vbuf[k+ 1] = cy + (p[4]-sy)*f;
		vbuf[k+ 2] = (p[5]-sz)*f;
		vbuf[k+ 3] = 1.0;
		vbuf[k+ 4] = 1.0;
		vbuf[k+ 5] = 1.0;
		vbuf[k+ 6] = p[0];
		vbuf[k+ 7] = p[1];
		vbuf[k+ 8] = p[2];

		vbuf[k+ 9] = cx + (p[6]-sx)*f;
		vbuf[k+10] = cy + (p[7]-sy)*f;
		vbuf[k+11] = (p[8]-sz)*f;
		vbuf[k+12] = 1.0;
		vbuf[k+13] = 1.0;
		vbuf[k+14] = 1.0;
		vbuf[k+15] = p[0];
		vbuf[k+16] = p[1];
		vbuf[k+17] = p[2];

		vbuf[k+18] = cx + (p[9]-sx)*f;
		vbuf[k+19] = cy + (p[10]-sy)*f;
		vbuf[k+20] = (p[11]-sz)*f;
		vbuf[k+21] = 1.0;
		vbuf[k+22] = 1.0;
		vbuf[k+23] = 1.0;
		vbuf[k+24] = p[0];
		vbuf[k+25] = p[1];
		vbuf[k+26] = p[2];
	}
}