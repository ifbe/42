#include "actor.h"
#define accuracy 18
#define PI 3.1415926535897932384626433832795028841971693993151




void carveascii_area(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->rectcount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*8);
	u16* index     = buf + 0xe00000 + (icount*2);

	win->vertexcount  += 4;
	win->normalcount  += 4;
	win->colorcount   += 4;
	win->texturecount += 4;
	win->rectcount   += 6;

	vertex[ 0] = cx-rx-fx;
	vertex[ 1] = cy-ry-fy;
	vertex[ 2] = cz-rz-fz;
	vertex[ 3] = cx+rx-fx;
	vertex[ 4] = cy+ry-fy;
	vertex[ 5] = cz+rz-fz;
	vertex[ 6] = cx-rx+fx;
	vertex[ 7] = cy-ry+fy;
	vertex[ 8] = cz-rz+fz;
	vertex[ 9] = cx+rx+fx;
	vertex[10] = cy+ry+fy;
	vertex[11] = cz+rz+fz;

	color[ 0] = rr;
	color[ 1] = gg;
	color[ 2] = bb;
	color[ 3] = rr;
	color[ 4] = gg;
	color[ 5] = bb;
	color[ 6] = rr;
	color[ 7] = gg;
	color[ 8] = bb;
	color[ 9] = rr;
	color[10] = gg;
	color[11] = bb;

	texture[0] = 0.0;
	texture[1] = 1.0;
	texture[2] = 1.0;
	texture[3] = 1.0;
	texture[4] = 0.0;
	texture[5] = 0.0;
	texture[6] = 1.0;
	texture[7] = 0.0;

	index[0] = pcount+0;
	index[1] = pcount+1;
	index[2] = pcount+3;
	index[3] = pcount+0;
	index[4] = pcount+2;
	index[5] = pcount+3;
}
void carveascii(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u8 dat)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->rectcount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*8);
	u16* index     = buf + 0xe00000 + (icount*2);

	win->vertexcount  += 4;
	win->normalcount  += 4;
	win->colorcount   += 4;
	win->texturecount += 4;
	win->rectcount   += 6;

	vertex[ 0] = cx-rx-fx;
	vertex[ 1] = cy-ry-fy;
	vertex[ 2] = cz-rz-fz;
	vertex[ 3] = cx+rx-fx;
	vertex[ 4] = cy+ry-fy;
	vertex[ 5] = cz+rz-fz;
	vertex[ 6] = cx-rx+fx;
	vertex[ 7] = cy-ry+fy;
	vertex[ 8] = cz-rz+fz;
	vertex[ 9] = cx+rx+fx;
	vertex[10] = cy+ry+fy;
	vertex[11] = cz+rz+fz;

	color[ 0] = rr;
	color[ 1] = gg;
	color[ 2] = bb;
	color[ 3] = rr;
	color[ 4] = gg;
	color[ 5] = bb;
	color[ 6] = rr;
	color[ 7] = gg;
	color[ 8] = bb;
	color[ 9] = rr;
	color[10] = gg;
	color[11] = bb;

	texture[0] = dat/256.0;
	texture[1] = 1.0/256.0;
	texture[2] = (dat+1)/256.0;
	texture[3] = 1.0/256.0;
	texture[4] = dat/256.0;
	texture[5] = 0.0;
	texture[6] = (dat+1)/256.0;
	texture[7] = 0.0;

	index[0] = pcount+0;
	index[1] = pcount+1;
	index[2] = pcount+3;
	index[3] = pcount+0;
	index[4] = pcount+2;
	index[5] = pcount+3;
}
void carvedecimal(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u32 val)
{
	int j,len;
	float f;
	u8 str[8];

	for(len=0;len<8;len++)
	{
		if(0 == val)break;
		str[len] = 0x30 + (val%10);
		val = val/10;
	}
	if(len == 0)
	{
		len = 1;
		str[0] = '0';
	}

	for(j=0;j<len;j++)
	{
		f = (float)(j-len/2)*2;
		carveascii(
			win, rgb,
			cx + (rx*f), cy + (ry*f), cz + (rz*f),
			rx, ry, rz,
			fx, fy, fz,
			str[len-1-j]
		);
	}
}
void carvestring(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u8* buf, int len)
{
	int j;
	float f;
	if(0 == buf)return;
	if(0 == len)
	{
		while(buf[len] > 0x20)len++;
	}
	else
	{
		for(j=0;j<len;j++)
		{
			if(buf[j] < 0x20){len = j;break;}
		}
	}
	if(len == 0)return;

	for(j=0;j<len;j++)
	{
		f = (float)(j-len/2)/2;
		carveascii(
			win, rgb,
			cx + (rx*f), cy + (ry*f), cz + (rz*f),
			rx/2, ry/2, rz/2,
			fx, fy, fz,
			buf[j]
		);
	}
}