#include "libuser.h"
int openreadclose(void* name, int off, void* buf, int len);
int openwriteclose(void* name, int off, void* buf, int len);
//utf8
void initutf8(void*);
//nanojpeg
void njInit(void);
void njDone(void);
int njDecode(const void* jpeg, const int size);
int njGetWidth(void);
int njGetHeight(void);
int njIsColor(void);
int njGetImageSize(void);
unsigned char* njGetImage(void);




static u8* utf8data = 0;
static u16* bgmdata = 0;




void asset_create()
{
	int j,k;
	if(0 == utf8data)
	{
		utf8data = memorycreate(0x200000);
		if(0 != utf8data)
		{
			j = openreadclose("unicode.raw", 0, utf8data, 0x200000);
			if(j < 0x200000)say("error@unicode\n");

			for(j=0;j<0x200000;j++)
			{
				k = utf8data[j];
				k = ((k&0x0f)<<4) | ((k&0xf0)>>4);
				k = ((k&0x33)<<2) | ((k&0xcc)>>2);
				k = ((k&0x55)<<1) | ((k&0xaa)>>1);
				utf8data[j] = k;
			}
			initutf8(utf8data);
		}
	}
}
void asset_delete()
{
}








void actorcreatefromstl(struct actor* act, u8* buf, int len)
{
	float* p;
	float* vl = act->target.vl;	//left
	float* vr = act->target.vr;	//right
	float* vn = act->target.vn;	//near
	float* vf = act->target.vf;	//far
	float* vb = act->target.vb;	//bot
	float* vu = act->target.vu;	//top
	float* vv = act->target.vv;	//info
	float* vc = act->target.vc;	//center
	int j,ret;

	vl[0] = 100000.0;
	vl[1] = 0.0;
	vl[2] = 0.0;
	vr[0] = -100000.0;
	vr[1] = 0.0;
	vr[2] = 0.0;

	vn[0] = 0.0;
	vn[1] = 100000.0;
	vn[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = -100000.0;
	vf[2] = 0.0;

	vb[0] = 0.0;
	vb[1] = 0.0;
	vb[2] = 100000.0;
	vu[0] = 0.0;
	vu[1] = 0.0;
	vu[0] = -100000.0;

	ret = *(u32*)(buf+80);
	say("len=%x, count=%x\n", len, ret);
	ret = ret%(0x200000/36);

	for(j=0;j<ret;j++)
	{
		p = (void*)buf + 84 + j*50;

		if(p[ 3] < vl[0])vl[0] = p[3];
		if(p[ 3] > vr[0])vr[0] = p[3];
		if(p[ 4] < vn[1])vn[1] = p[4];
		if(p[ 4] > vf[1])vf[1] = p[4];
		if(p[ 5] < vb[2])vb[2] = p[5];
		if(p[ 5] > vu[2])vu[2] = p[5];

		if(p[ 6] < vl[0])vl[0] = p[6];
		if(p[ 6] > vr[0])vr[0] = p[6];
		if(p[ 7] < vn[1])vn[1] = p[7];
		if(p[ 7] > vf[1])vf[1] = p[7];
		if(p[ 8] < vb[2])vb[2] = p[8];
		if(p[ 8] > vu[2])vu[2] = p[8];

		if(p[ 9] < vl[0])vl[0] = p[9];
		if(p[ 9] > vr[0])vr[0] = p[9];
		if(p[10] < vn[1])vn[1] = p[10];
		if(p[10] > vf[1])vf[1] = p[10];
		if(p[11] < vb[2])vb[2] = p[11];
		if(p[11] > vu[2])vu[2] = p[11];
	}
	say(
		"l=%f, r=%f, n=%f, f=%f, b=%f, u=%f\n",
		vl[0], vr[0], vn[1], vf[1], vb[2], vu[2]
	);

	vv[0] = vr[0] - vl[0];
	vv[1] = vf[1] - vn[1];
	vv[2] = vu[2] - vb[2];
	vc[0] = (vl[0] + vr[0])/2;
	vc[1] = (vn[1] + vf[1])/2;
	vc[2] = (vb[2] + vu[2])/2;
	say(
		"w=%f, h=%f, d=%f, x=%f, y=%f, z=%f\n",
		vv[0], vv[0], vv[1], vc[1], vc[2], vc[2]
	);
}
void actorcreatefromjpeg(struct actor* act, u8* buf, int len)
{
	int ret;
	u8* rgb;

    njInit();
	ret = njDecode(buf, len);
	if(0 != ret)
	{
		say("error@njDecode:%d\n", ret);
		return;
	}

	rgb = njGetImage();
	len = (njGetImageSize())/3;
	for(ret=0;ret<len;ret++)
	{
		buf[ret*4+0] = rgb[ret*3+2];
		buf[ret*4+1] = rgb[ret*3+1];
		buf[ret*4+2] = rgb[ret*3+0];
		buf[ret*4+3] = 0;
	}

	act->width = njGetWidth();
	act->height = njGetHeight();
	act->buf = buf;
}
void actorcreatefromfile(struct actor* act, char* name)
{
	int len;
	u8* buf;
	if(0 == act)return;

	buf = memorycreate(0x400000);
	len = openreadclose(name, 0, buf, 0x400000);
	if(len <= 0)
	{
		say("len=%d\n", len);
		return;
	}

	actorcreatefromjpeg(act, buf, len);
	//actorcreatefromstl(act, buf, len);
}