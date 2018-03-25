#include "actor.h"
#define acc 18
#define fonti 0x28
#define fontv 0x29
#define PI 3.1415926535897932384626433832795028841971693993151
int utf2unicode(u8* src,u32* dst);




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




void carveascii_area(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float x0, float y0, float x1, float y1)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[fonti].len;
	int vlen = mod[fontv].len;
	u16* ibuf = (mod[fonti].buf) + (6*ilen);
	float* vbuf = (mod[fontv].buf) + (36*vlen);
	mod[fonti].len += 2;
	mod[fontv].len += 4;

	vbuf[ 0] = cx-rx-fx;
	vbuf[ 1] = cy-ry-fy;
	vbuf[ 2] = cz-rz-fz;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = x0;
	vbuf[ 7] = y1;

	vbuf[ 9] = cx+rx-fx;
	vbuf[10] = cy+ry-fy;
	vbuf[11] = cz+rz-fz;
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = x1;
	vbuf[16] = y1;

	vbuf[18] = cx-rx+fx;
	vbuf[19] = cy-ry+fy;
	vbuf[20] = cz-rz+fz;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = x0;
	vbuf[25] = y0;

	vbuf[27] = cx+rx+fx;
	vbuf[28] = cy+ry+fy;
	vbuf[29] = cz+rz+fz;
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = x1;
	vbuf[34] = y0;

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

	int q = ((unicode&0xffff)/0x4000)*2;
	struct texandobj* mod = win->buf;
	int ilen = mod[fonti+q].len;
	int vlen = mod[fontv+q].len;
	u16* ibuf = (mod[fonti+q].buf) + (6*ilen);
	float* vbuf = (mod[fontv+q].buf) + (36*vlen);
	mod[fonti+q].len += 2;
	mod[fontv+q].len += 4;

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
	int ilen = mod[fonti].len;
	int vlen = mod[fontv].len;
	u16* ibuf = (mod[fonti].buf) + (6*ilen);
	float* vbuf = (mod[fontv].buf) + (36*vlen);
	mod[fonti].len += 2;
	mod[fontv].len += 4;

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
