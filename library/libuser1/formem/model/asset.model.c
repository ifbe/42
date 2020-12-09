#include "libuser.h"




void computetangent(float* p, int c)
{
	int j;
	float u1,v1, u2,v2, u3,v3;
	float x1,y1,z1, x2,y2,z2, x3,y3,z3;
	float u1to2,v1to2, u1to3,v1to3;
	float x1to2,y1to2,z1to2, x1to3,y1to3,z1to3;
	float x,y,z,tmp;
	for(j=0;j<c;j++){	//foreach trigon
		x1 = p[j*36 + 0];
		y1 = p[j*36 + 1];
		z1 = p[j*36 + 2];
		u1 = p[j*36 + 3];
		v1 = p[j*36 + 4];

		x2 = p[j*36 + 12];
		y2 = p[j*36 + 13];
		z2 = p[j*36 + 14];
		u2 = p[j*36 + 15];
		v2 = p[j*36 + 16];

		x3 = p[j*36 + 24];
		y3 = p[j*36 + 25];
		z3 = p[j*36 + 26];
		u3 = p[j*36 + 27];
		v3 = p[j*36 + 28];

		x1to2 = x2 - x1;
		y1to2 = y2 - y1;
		z1to2 = z2 - z1;

		x1to3 = x3 - x1;
		y1to3 = y3 - y1;
		z1to3 = z3 - z1;

		u1to2 = u2 - u1;
		v1to2 = v2 - v1;

		u1to3 = u3 - u1;
		v1to3 = v3 - v1;

		tmp = 1.0f / (u1to2 * v1to3 - u1to3 * v1to2);
		x = tmp * (v1to3 * x1to2 - v1to2 * x1to3);
		y = tmp * (v1to3 * y1to2 - v1to2 * y1to3);
		z = tmp * (v1to3 * z1to2 - v1to2 * z1to3);
say("%d:%f,%f,%f\n",j,x,y,z);
		p[j*36 +  9] = x;
		p[j*36 + 10] = y;
		p[j*36 + 11] = z;

		p[j*36 + 21] = x;
		p[j*36 + 22] = y;
		p[j*36 + 23] = z;

		p[j*36 + 33] = x;
		p[j*36 + 34] = y;
		p[j*36 + 35] = z;
	}
}




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
void parsefacefromobj(
	float* dst, int cnt, u8* buf, int len,
	float* vv, int cv, float* vt, int ct, float* vn, int cn, float* vp, int cp)
{
	u16 f[9];
	int j,k;
	float* p;
	float* vtx;
	float* tex;
	float* nor;
	say("parsefacefromobj: %.*s\n",len,buf);

	//format>>	1/2/3 4/5/6 7/8/9	<<format
	k = 0;
	p = &dst[cnt*12*3];
	for(j=0;j<9;j++){
		if(k >= len)break;
		k += 1+decstr2u16(buf+k, &f[j]);
		f[j] -= 1;
	}
	say("parsed value: %d,%d,%d %d,%d,%d %d,%d,%d\n", f[0],f[1],f[2], f[3],f[4],f[5], f[6],f[7],f[8]);

	//3 triangles
	for(j=0;j<3;j++){
		p = &dst[(cnt*3 + j) * 12];

		k = f[j*3+0];
		vtx = &vv[k*3];
		p[0] = vtx[0];
		p[1] = vtx[1];
		p[2] = vtx[2];

		k = f[j*3+1];
		tex = &vt[k*3];
		p[3] = tex[0];
		p[4] = tex[1];
		p[5] = tex[2];

		k = f[j*3+2];
		nor = &vn[k*3];
		p[6] = nor[0];
		p[7] = nor[1];
		p[8] = nor[2];
	}

	p = &dst[cnt*12*3];
	say("@a: %f,%f,%f %f,%f,%f %f,%f,%f\n", j, p[ 0],p[ 1],p[ 2], p[ 3],p[ 4],p[ 5], p[ 6],p[ 7],p[ 8]);
	say("@b: %f,%f,%f %f,%f,%f %f,%f,%f\n", j, p[12],p[13],p[14], p[15],p[16],p[17], p[18],p[19],p[20]);
	say("@c: %f,%f,%f %f,%f,%f %f,%f,%f\n", j, p[24],p[25],p[26], p[27],p[28],p[29], p[30],p[31],p[32]);
}
void parsevertfromobj(struct mysrc* ctx, struct fstyle* sty, u8* buf, int len)
{
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


	int j,k,cnt = 0;		//how many trigon
	int cv = 0;
	int ct = 0;
	int cn = 0;
	int cp = 0;
	float* fv = ctx->vtx[0].vbuf+0x100000;
	float* ft = ctx->vtx[0].vbuf+0x140000;
	float* fn = ctx->vtx[0].vbuf+0x180000;
	float* fp = ctx->vtx[0].vbuf+0x1c0000;
	float* dst = ctx->vtx[0].vbuf;

	k = 0;
	for(j=0;j<len;j++){
		//say("%x\n",buf[j]);
		if((buf[j] <= 0xa)&&(j-k>1)) {
			say("[%x,%x]%.*s\n", k,j, j-k,buf+k);
			if('v' == buf[k]){
				if(' ' == buf[k+1]){		//vertex
					parsevec3fromobj(fv, cv, buf+k+2, j-k-2);
					if(vl[0] > fv[cv*3+0])vl[0] = fv[cv*3+0];
					if(vr[0] < fv[cv*3+0])vr[0] = fv[cv*3+0];
					if(vn[1] > fv[cv*3+1])vn[1] = fv[cv*3+1];
					if(vf[1] < fv[cv*3+1])vf[1] = fv[cv*3+1];
					if(vb[2] > fv[cv*3+2])vb[2] = fv[cv*3+2];
					if(vu[2] < fv[cv*3+2])vu[2] = fv[cv*3+2];
					cv += 1;
				}
				else if('t' == buf[k+1]){	//tex uv
					parsevec3fromobj(ft, ct, buf+k+3, j-k-3);
					ct += 1;
				}
				else if('n' == buf[k+1]){	//normal
					parsevec3fromobj(fn, cn, buf+k+3, j-k-3);
					cn += 1;
				}
				else if('p' == buf[k+1]){	//parameter
					parsevec3fromobj(fp, cp, buf+k+3, j-k-3);
					cp += 1;
				}
				else say("error@%d\n",j);
			}
			if('f' == buf[k]){
				parsefacefromobj(
					dst,cnt, buf+k+2,j-k-2,
					fv,cv, ft,ct, fn,cn, fp,cp
				);
				cnt += 1;
			}
			k = j+1;

			//say("buf[%x] = %x\n", j, buf[j]);
			if(buf[j] < 0xa)break;
		}
	}
say("j=%d,len=%d,cnt=%d\n",j,len,cnt);

	//calculate tangent
	computetangent(dst, cnt);

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
	ctx->vtx[0].vbuf_h = cnt*3;	//3point = 1triangle
}




void parsevertfromstl(struct fstyle* sty, int* vbuf_h, u8* buf, int len)
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

	//*vbuf_w = 4*6;	//vx,vy,vz,nx,ny,nz
	*vbuf_h = ret*3;	//3point = 1triangle
}
