#include "actor.h"
int utf2unicode(u8* src,u32* dst);




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

	struct texandobj* mod = win->buf;
	int ilen = mod[0].ilen;
	int vlen = mod[0].vlen;
	u16* ibuf = (mod[0].ibuf) + (6*ilen);
	float* vbuf = (mod[0].vbuf) + (36*vlen);
	mod[0].ilen += 2;
	mod[0].vlen += 4;

	vbuf[ 0] = cx-rx-fx;
	vbuf[ 1] = cy-ry-fy;
	vbuf[ 2] = cz-rz-fz;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = (dat+0.0)/128.0;
	vbuf[ 7] = 1.0/128.0;

	vbuf[ 9] = cx+rx-fx;
	vbuf[10] = cy+ry-fy;
	vbuf[11] = cz+rz-fz;
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = (dat+1.0)/128.0;
	vbuf[16] = 1.0/128.0;

	vbuf[18] = cx-rx+fx;
	vbuf[19] = cy-ry+fy;
	vbuf[20] = cz-rz+fz;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = (dat+0.0)/128.0;
	vbuf[25] = 0.0;

	vbuf[27] = cx+rx+fx;
	vbuf[28] = cy+ry+fy;
	vbuf[29] = cz+rz+fz;
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = (dat+1.0)/128.0;
	vbuf[34] = 0.0;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+3;
	ibuf[3] = vlen+0;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}
void carveunicode(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u32 unicode)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	int vvv = (unicode&0xffff)/0x4000;
	struct texandobj* mod = win->buf;
	int ilen = mod[vvv].ilen;
	int vlen = mod[vvv].vlen;
	u16* ibuf = (mod[vvv].ibuf) + (6*ilen);
	float* vbuf = (mod[vvv].vbuf) + (36*vlen);
	mod[vvv].ilen += 2;
	mod[vvv].vlen += 4;

	unicode = unicode&0x3fff;
	vbuf[ 0] = cx-rx-fx;
	vbuf[ 1] = cy-ry-fy;
	vbuf[ 2] = cz-rz-fz;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = (unicode&0x7f)/128.0;
	vbuf[ 7] = ((unicode>>7)+1)/128.0;

	vbuf[ 9] = cx+rx-fx;
	vbuf[10] = cy+ry-fy;
	vbuf[11] = cz+rz-fz;
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = ((unicode&0x7f)+1)/128.0;
	vbuf[16] = ((unicode>>7)+1)/128.0;

	vbuf[18] = cx-rx+fx;
	vbuf[19] = cy-ry+fy;
	vbuf[20] = cz-rz+fz;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = (unicode&0x7f)/128.0;
	vbuf[25] = (unicode>>7)/128.0;

	vbuf[27] = cx+rx+fx;
	vbuf[28] = cy+ry+fy;
	vbuf[29] = cz+rz+fz;
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = ((unicode&0x7f)+1)/128.0;
	vbuf[34] = (unicode>>7)/128.0;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+3;
	ibuf[3] = vlen+0;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}




void carveutf8(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u8* buf, int len)
{
	u32 unicode;
	utf2unicode(buf, &unicode);
	carveunicode(
		win, rgb,
		cx, cy, cz,
		rx, ry, rz,
		fx, fy, fz,
		unicode
	);
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
void carvehexadecimal(
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
		str[len] = 0x30 + (val&0xf);
		if(str[len] > 0x39)str[len] += 7;
		val = val>>4;
	}
	if(len == 0)
	{
		len = 1;
		str[0] = '0';
	}

	for(j=0;j<len;j++)
	{
		f = (float)((j-len/2)*2+1);
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
		carveascii(
			win, rgb,
			cx + (rx*j/2), cy + (ry*j/2), cz + (rz*j/2),
			rx/2, ry/2, rz/2,
			fx, fy, fz,
			buf[j]
		);
	}
}
