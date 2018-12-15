#include "libuser.h"




void actorcreatefromobj(struct actor* act, u8* buf, int len)
{
}
void actorcreatefromstl(struct actor* act, u8* buf, int len)
{
	u8 tmp[0x100];
	int j,ret;
	float* src;
	float* dst;
	float* vl = act->target.vl;	//left
	float* vr = act->target.vr;	//right
	float* vn = act->target.vn;	//near
	float* vf = act->target.vf;	//far
	float* vb = act->target.vb;	//bot
	float* vu = act->target.vu;	//top
	float* vv = act->target.vv;	//info
	float* vc = act->target.vc;	//center

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
	vu[2] = -100000.0;

	ret = *(u32*)(buf+80);
	say("len=%x, count=%x\n", len, ret);
	ret = ret%(0x800000/72);

	for(j=0;j<250;j++)tmp[j] = buf[84+j];
	for(j=ret*3-1;j>=0;j--)
	{
		dst = (void*)buf + 24*j;
		if(j < 15)src = (void*)tmp + (j/3)*50;
		else src = (void*)buf + 84 + (j/3)*50;

		dst[3] = src[0];
		dst[4] = src[1];
		dst[5] = src[2];

		src = (void*)src + 12 + 12*(j%3);
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];

		if(vl[0] > dst[0])vl[0] = dst[0];
		if(vr[0] < dst[0])vr[0] = dst[0];
		if(vn[1] > dst[1])vn[1] = dst[1];
		if(vf[1] < dst[1])vf[1] = dst[1];
		if(vb[2] > dst[2])vb[2] = dst[2];
		if(vu[2] < dst[2])vu[2] = dst[2];

		//say("%f,%f,%f,%f,%f,%f\n",dst[0],dst[1],dst[2],dst[3],dst[4],dst[5]);
	}

	vv[0] = vr[0] - vl[0];
	vv[1] = vf[1] - vn[1];
	vv[2] = vu[2] - vb[2];
	vc[0] = (vl[0] + vr[0])/2;
	vc[1] = (vn[1] + vf[1])/2;
	vc[2] = (vb[2] + vu[2])/2;
	vr[0] -= vc[0];
	vf[1] -= vc[1];
	vu[2] -= vc[2];
	say(
		"l=%f, r=%f, n=%f, f=%f, b=%f, u=%f\n",
		vl[0], vr[0], vn[1], vf[1], vb[2], vu[2]
	);
	say(
		"w=%f, h=%f, d=%f, x=%f, y=%f, z=%f\n",
		vv[0], vv[1], vv[2], vc[0], vc[1], vc[2]
	);

	act->width = 4*6;
	act->height = ret*3;
	act->buf = buf;
}