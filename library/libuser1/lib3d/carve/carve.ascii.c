#include "actor.h"
int utf2unicode(u8* src,u32* dst);




void carveascii(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8 dat)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[0].ilen;
	int vlen = mod[0].vlen;
	u16* ibuf = (mod[0].ibuf) + (6*ilen);
	float* vbuf = (mod[0].vbuf) + (36*vlen);
	mod[0].ilen += 2;
	mod[0].vlen += 4;

	vbuf[ 0] = vc[0]-vr[0]-vf[0];
	vbuf[ 1] = vc[1]-vr[1]-vf[1];
	vbuf[ 2] = vc[2]-vr[2]-vf[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = (dat+0.0)/128.0;
	vbuf[ 7] = 1.0/128.0;

	vbuf[ 9] = vc[0]+vr[0]-vf[0];
	vbuf[10] = vc[1]+vr[1]-vf[1];
	vbuf[11] = vc[2]+vr[2]-vf[2];
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = (dat+1.0)/128.0;
	vbuf[16] = 1.0/128.0;

	vbuf[18] = vc[0]-vr[0]+vf[0];
	vbuf[19] = vc[1]-vr[1]+vf[1];
	vbuf[20] = vc[2]-vr[2]+vf[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = (dat+0.0)/128.0;
	vbuf[25] = 0.0;

	vbuf[27] = vc[0]+vr[0]+vf[0];
	vbuf[28] = vc[1]+vr[1]+vf[1];
	vbuf[29] = vc[2]+vr[2]+vf[2];
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
void carveunicode(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 unicode)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	int vvv = (unicode&0xffff)/0x4000;
	struct texandobj* mod = win->mod;
	int ilen = mod[vvv].ilen;
	int vlen = mod[vvv].vlen;
	u16* ibuf = (mod[vvv].ibuf) + (6*ilen);
	float* vbuf = (mod[vvv].vbuf) + (36*vlen);
	mod[vvv].ilen += 2;
	mod[vvv].vlen += 4;

	unicode = unicode&0x3fff;
	vbuf[ 0] = vc[0]-vr[0]-vf[0];
	vbuf[ 1] = vc[1]-vr[1]-vf[1];
	vbuf[ 2] = vc[2]-vr[2]-vf[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = (unicode&0x7f)/128.0;
	vbuf[ 7] = ((unicode>>7)+1)/128.0;

	vbuf[ 9] = vc[0]+vr[0]-vf[0];
	vbuf[10] = vc[1]+vr[1]-vf[1];
	vbuf[11] = vc[2]+vr[2]-vf[2];
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = ((unicode&0x7f)+1)/128.0;
	vbuf[16] = ((unicode>>7)+1)/128.0;

	vbuf[18] = vc[0]-vr[0]+vf[0];
	vbuf[19] = vc[1]-vr[1]+vf[1];
	vbuf[20] = vc[2]-vr[2]+vf[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = (unicode&0x7f)/128.0;
	vbuf[25] = (unicode>>7)/128.0;

	vbuf[27] = vc[0]+vr[0]+vf[0];
	vbuf[28] = vc[1]+vr[1]+vf[1];
	vbuf[29] = vc[2]+vr[2]+vf[2];
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




void carveutf8(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	u32 unicode;
	utf2unicode(buf, &unicode);
	carveunicode(win, rgb, vc, vr, vf, unicode);
}
void carvedecimal(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 val)
{
	int j,len;
	u8 str[8];
	float f;
	vec3 tc;

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
		tc[0] = vc[0] + (vr[0]*f);
		tc[1] = vc[1] + (vr[1]*f);
		tc[2] = vc[2] + (vr[2]*f);
		carveascii(win, rgb, tc, vr, vf, str[len-1-j]);
	}
}
void carvehexadecimal(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 val)
{
	int j,len;
	u8 str[8];
	float f;
	vec3 tc;

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
		tc[0] = vc[0] + (vr[0]*f);
		tc[1] = vc[1] + (vr[1]*f);
		tc[2] = vc[2] + (vr[2]*f);
		carveascii(win, rgb, tc, vr, vf, str[len-1-j]);
	}
}
void carvestring(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	int j;
	vec3 tc;
	vec3 tr;

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
		tc[0] = vc[0] + (vr[0]/2)*(j+1);
		tc[1] = vc[1] + (vr[1]/2)*(j+1);
		tc[2] = vc[2] + (vr[2]/2)*(j+1);
		tr[0] = vr[0]/2;
		tr[1] = vr[1]/2;
		tr[2] = vr[2]/2;
		carveascii(win, rgb, tc, tr, vf, buf[j]);
	}
}
void carvestring_center(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	int j;
	vec3 tc;
	vec3 tr;

	if(0 == buf)return;
	if(0 == len)
	{
		while(buf[len] >= 0x20)len++;
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
		tc[0] = vc[0] + (vr[0]/4)*(2*j-len+2),
		tc[1] = vc[1] + (vr[1]/4)*(2*j-len+2),
		tc[2] = vc[2] + (vr[2]/4)*(2*j-len+2),
		tr[0] = vr[0]/2;
		tr[1] = vr[1]/2;
		tr[2] = vr[2]/2;
		carveascii(win, rgb, tc, tr, vf, buf[j]);
	}
}