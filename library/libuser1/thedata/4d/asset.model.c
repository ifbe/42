#include "libuser.h"




void parsevec3fromobj(float* dst, int cnt, u8* buf, int len)
{
	int j,k = 0;
	float* v = &dst[cnt*3];
	say("parsevec3fromobj: %.*s\n",len,buf);

	for(j=0;j<3;j++){
		if(k >= len)break;
		k += 1+decstr2float(buf+k, &v[j]);
	}
	say("%d: %f,%f,%f\n", j, v[0], v[1], v[2]);
}
void parsefacefromobj(float* dst, int cnt, float* vv, int cv, float* vn, int cn, float* vt, int ct, u8* buf, int len)
{
	u16 f[9];
	int j,k;
	float* v;
	float* n;
	float* t;
	float* p;
	say("parsefacefromobj: %.*s\n",len,buf);

	//format>>	1/2/3 4/5/6 7/8/9	<<format//
	k = 0;
	p = &dst[cnt*27];
	for(j=0;j<9;j++){
		if(k >= len)break;
		k += 1+decstr2u16(buf+k, &f[j]);
		f[j] -= 1;
	}
	say("%d: %d,%d,%d %d,%d,%d %d,%d,%d\n", j, f[0],f[1],f[2], f[3],f[4],f[5], f[6],f[7],f[8]);

	//3 triangles
	for(j=0;j<3;j++){
		p = &dst[cnt*27 + j*9];

		k = f[j*3+0];
		v = &vv[k*3];
		p[0] = v[0];
		p[1] = v[1];
		p[2] = v[2];

		k = f[j*3+2];
		n = &vn[k*3];
		p[3] = n[0];
		p[4] = n[1];
		p[5] = n[2];

		k = f[j*3+1];
		t = &vt[k*3];
		p[6] = t[0];
		p[7] = t[1];
		p[8] = t[2];
	}

	p = &dst[cnt*27];
	//say("@a: %f,%f,%f %f,%f,%f %f,%f,%f\n", j, p[ 0],p[ 1],p[ 2], p[ 3],p[ 4],p[ 5], p[ 6],p[ 7],p[ 8]);
	//say("@b: %f,%f,%f %f,%f,%f %f,%f,%f\n", j, p[ 9],p[10],p[11], p[12],p[13],p[14], p[15],p[16],p[17]);
	//say("@c: %f,%f,%f %f,%f,%f %f,%f,%f\n", j, p[18],p[19],p[20], p[21],p[22],p[23], p[24],p[24],p[26]);
}
void parsevertfromobj(struct glsrc* ctx, struct fstyle* sty, u8* buf, int len)
{
	int j,k;
	int cv;
	int cn;
	int ct;
	int cnt;
	float* fv;
	float* fn;
	float* ft;
	float* dst;
	float* vl = sty->vl;	//left
	float* vr = sty->vr;	//right
	float* vn = sty->vn;	//near
	float* vf = sty->vf;	//far
	float* vb = sty->vb;	//bot
	float* vu = sty->vt;	//top
	float* vq = sty->vq;	//info
	float* vc = sty->vc;	//center
	//say("%s\n",buf);

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

	cv = 0;
	cn = 0;
	ct = 0;
	cnt = 0;
	fv = ctx->vbuf+0x80000;
	fn = ctx->vbuf+0xa0000;
	ft = ctx->vbuf+0xc0000;
	dst = ctx->vbuf;

	k = 0;
	for(j=0;j<len;j++){
		//say("%x\n",buf[j]);
		if((buf[j] <= 0xa)&&(j-k>1)) {
			//say("%.*s\n", j-k, buf+k);
			if('v' == buf[k]){
				if(' ' == buf[k+1]){
					parsevec3fromobj(fv, cv, buf+k+2, j-k-2);
					if(vl[0] > fv[cv*3+0])vl[0] = fv[cv*3+0];
					if(vr[0] < fv[cv*3+0])vr[0] = fv[cv*3+0];
					if(vn[1] > fv[cv*3+1])vn[1] = fv[cv*3+1];
					if(vf[1] < fv[cv*3+1])vf[1] = fv[cv*3+1];
					if(vb[2] > fv[cv*3+2])vb[2] = fv[cv*3+2];
					if(vu[2] < fv[cv*3+2])vu[2] = fv[cv*3+2];
					cv += 1;
				}
				else if('n' == buf[k+1]){
					parsevec3fromobj(fn, cn, buf+k+3, j-k-3);
					cn += 1;
				}
				else if('t' == buf[k+1]){
					parsevec3fromobj(ft, ct, buf+k+3, j-k-3);
					ct += 1;
				}
				else say("error@%d\n",j);
			}
			if('f' == buf[k]){
				parsefacefromobj(dst, cnt, fv, cv, fn, cn, ft, ct, buf+k+2, j-k-2);
				cnt += 1;
			}
			k = j+1;

			say("buf[%x] = %x\n", j, buf[j]);
			if(buf[j] < 0xa)break;
		}
	}
say("j=%d,len=%d,cnt=%d\n",j,len,cnt);

	vq[0] = vr[0] - vl[0];
	vq[1] = vf[1] - vn[1];
	vq[2] = vu[2] - vb[2];
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
		vq[0], vq[1], vq[2], vc[0], vc[1], vc[2]
	);

	//ctx->vbuf_w = 4*9;	//vx,vy,vz,nx,ny,nz,tx,ty,tz
	ctx->vbuf_h = cnt*3;	//3point = 1triangle
}




void parsevertfromstl(struct glsrc* ctx, struct fstyle* sty, u8* buf, int len)
{
	u8 tmp[0x100];
	int j,ret;
	float* src;
	float* dst;
	float* vl = sty->vl;	//left
	float* vr = sty->vr;	//right
	float* vn = sty->vn;	//near
	float* vf = sty->vf;	//far
	float* vb = sty->vb;	//bot
	float* vu = sty->vt;	//top
	float* vq = sty->vq;	//info
	float* vc = sty->vc;	//center

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
	ret = ret%(0x1000000/72);

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

	vq[0] = vr[0] - vl[0];
	vq[1] = vf[1] - vn[1];
	vq[2] = vu[2] - vb[2];
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
		vq[0], vq[1], vq[2], vc[0], vc[1], vc[2]
	);

	//ctx->vbuf_w = 4*6;	//vx,vy,vz,nx,ny,nz
	ctx->vbuf_h = ret*3;	//3point = 1triangle
}
