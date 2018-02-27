#include "actor.h"
#define acc 18
#define PI 3.1415926535897932384626433832795028841971693993151
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




void carvesolid_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += 1;
	mod[0x25].len += 3;

	vbuf[ 0] = x1;
	vbuf[ 1] = y1;
	vbuf[ 2] = z1;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = 0.0;
	vbuf[ 7] = 0.0;
	vbuf[ 8] = 1.0;

	vbuf[ 9] = x2;
	vbuf[10] = y2;
	vbuf[11] = z2;
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = 0.0;
	vbuf[16] = 0.0;
	vbuf[17] = 1.0;

	vbuf[18] = x3;
	vbuf[19] = y3;
	vbuf[20] = z3;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = 0.0;
	vbuf[25] = 0.0;
	vbuf[26] = 1.0;

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}
void carvesolid_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += 2;
	mod[0x25].len += 4;

	vbuf[ 0] = cx - rx - fx;
	vbuf[ 1] = cy - ry - fy;
	vbuf[ 2] = cz - rz - fz;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = 0.0;
	vbuf[ 7] = 0.0;
	vbuf[ 8] = 1.0;

	vbuf[ 9] = cx + rx - fx;
	vbuf[10] = cy + ry - fy;
	vbuf[11] = cz + rz - fz;
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = 0.0;
	vbuf[16] = 0.0;
	vbuf[17] = 1.0;

	vbuf[18] = cx - rx + fx;
	vbuf[19] = cy - ry + fy;
	vbuf[20] = cz - rz + fz;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = 0.0;
	vbuf[25] = 0.0;
	vbuf[26] = 1.0;

	vbuf[27] = cx + rx + fx;
	vbuf[28] = cy + ry + fy;
	vbuf[29] = cz + rz + fz;
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = 0.0;
	vbuf[34] = 0.0;
	vbuf[35] = 1.0;

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}
void carvesolid_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int a,b,j;
	float s,t;
	float q[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += acc;
	mod[0x25].len += acc+1;

	for(j=0;j<acc;j++)
	{
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		vectornormalize(q);

		t = j*PI/acc;
		q[0] *= sine(t);
		q[1] *= sine(t);
		q[2] *= sine(t);
		q[3] = cosine(t);

		a = j*9;
		b = j*3;

		vbuf[a+0] = rx;
		vbuf[a+1] = ry;
		vbuf[a+2] = rz;
		quaternionrotate(&vbuf[a], q);

		vbuf[a+0] += cx;
		vbuf[a+1] += cy;
		vbuf[a+2] += cz;
		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;
		vbuf[a+6] = ux;
		vbuf[a+7] = uy;
		vbuf[a+8] = uz;

		ibuf[b+0] = vlen + acc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%acc;
	}

	a = acc*9;
	vbuf[a+0] = cx;
	vbuf[a+1] = cy;
	vbuf[a+2] = cz;
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
	vbuf[a+6] = ux;
	vbuf[a+7] = uy;
	vbuf[a+8] = uz;
}




void carvesolid_pyramid3()
{
}
void carvesolid_pyramid4()
{
}
void carvesolid_pyramid5()
{
}
void carvesolid_pyramid6()
{
}
void carvesolid_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int a,b,j;
	float s,t;
	float q[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += acc*2;
	mod[0x25].len += acc+2;

	for(j=0;j<acc;j++)
	{
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		vectornormalize(q);

		t = j*PI/acc;
		q[0] *= sine(t);
		q[1] *= sine(t);
		q[2] *= sine(t);
		q[3] = cosine(t);

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, q);

		a = j*9;
		b = j*6;

		vbuf[a+0] = cx + r[0];
		vbuf[a+1] = cy + r[1];
		vbuf[a+2] = cz + r[2];
		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;
		vbuf[a+6] = vbuf[a+0] - cx;
		vbuf[a+7] = vbuf[a+1] - cy;
		vbuf[a+8] = vbuf[a+2] - cz;

		//bottom
		ibuf[b+0] = vlen+acc;
		ibuf[b+1] = vlen+j;
		ibuf[b+2] = vlen+(j+1)%acc;

		//upper
		ibuf[b+3] = vlen+acc+1;
		ibuf[b+4] = vlen+j;
		ibuf[b+5] = vlen+(j+1)%acc;
	}

	a = acc*9;

	vbuf[a+ 0] = cx;
	vbuf[a+ 1] = cy;
	vbuf[a+ 2] = cz;
	vbuf[a+ 3] = rr;
	vbuf[a+ 4] = gg;
	vbuf[a+ 5] = bb;
	vbuf[a+ 6] = -ux;
	vbuf[a+ 7] = -uy;
	vbuf[a+ 8] = -uz;

	vbuf[a+ 9] = cx+ux;
	vbuf[a+10] = cy+uy;
	vbuf[a+11] = cz+uz;
	vbuf[a+12] = rr;
	vbuf[a+13] = gg;
	vbuf[a+14] = bb;
	vbuf[a+15] = ux;
	vbuf[a+16] = uy;
	vbuf[a+18] = uz;
}




void carvesolid_prism3()
{
}
void carvesolid_prism4(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz)
{
	int j;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += 12;
	mod[0x25].len += 24;

	for(j=0;j<24*9;j+=9)
	{
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
		vbuf[j+6] = 0.0;
		vbuf[j+7] = 0.0;
		vbuf[j+8] = 0.0;
	}

	vbuf[2*9+0] = vbuf[1*9+0] = vbuf[0*9+0] = cx - rx - fx - ux;
	vbuf[2*9+1] = vbuf[1*9+1] = vbuf[0*9+1] = cy - ry - fy - uy;
	vbuf[2*9+2] = vbuf[1*9+2] = vbuf[0*9+2] = cz - rz - fz - uz;
	vbuf[0*9+8] = -1.0;
	vbuf[1*9+7] = -1.0;
	vbuf[2*9+6] = -1.0;

	vbuf[5*9+0] = vbuf[4*9+0] = vbuf[3*9+0] = cx + rx - fx - ux;
	vbuf[5*9+1] = vbuf[4*9+1] = vbuf[3*9+1] = cy + ry - fy - uy;
	vbuf[5*9+2] = vbuf[4*9+2] = vbuf[3*9+2] = cz + rz - fz - uz;
	vbuf[3*9+8] = -1.0;
	vbuf[4*9+7] = -1.0;
	vbuf[5*9+6] = 1.0;

	vbuf[8*9+0] = vbuf[7*9+0] = vbuf[6*9+0] = cx - rx + fx - ux;
	vbuf[8*9+1] = vbuf[7*9+1] = vbuf[6*9+1] = cy - ry + fy - uy;
	vbuf[8*9+2] = vbuf[7*9+2] = vbuf[6*9+2] = cz - rz + fz - uz;
	vbuf[6*9+8] = -1.0;
	vbuf[7*9+7] = 1.0;
	vbuf[8*9+6] = -1.0;

	vbuf[11*9+0] = vbuf[10*9+0] = vbuf[9*9+0] = cx + rx + fx - ux;
	vbuf[11*9+1] = vbuf[10*9+1] = vbuf[9*9+1] = cy + ry + fy - uy;
	vbuf[11*9+2] = vbuf[10*9+2] = vbuf[9*9+2] = cz + rz + fz - uz;
	vbuf[ 9*9+8] = -1.0;
	vbuf[10*9+7] = 1.0;
	vbuf[11*9+6] = 1.0;

	vbuf[14*9+0] = vbuf[13*9+0] = vbuf[12*9+0] = cx - rx - fx + ux;
	vbuf[14*9+1] = vbuf[13*9+1] = vbuf[12*9+1] = cy - ry - fy + uy;
	vbuf[14*9+2] = vbuf[13*9+2] = vbuf[12*9+2] = cz - rz - fz + uz;
	vbuf[12*9+8] = 1.0;
	vbuf[13*9+7] = -1.0;
	vbuf[14*9+6] = 1.0;

	vbuf[17*9+0] = vbuf[16*9+0] = vbuf[15*9+0] = cx + rx - fx + ux;
	vbuf[17*9+1] = vbuf[16*9+1] = vbuf[15*9+1] = cy + ry - fy + uy;
	vbuf[17*9+2] = vbuf[16*9+2] = vbuf[15*9+2] = cz + rz - fz + uz;
	vbuf[15*9+8] = 1.0;
	vbuf[16*9+7] = -1.0;
	vbuf[17*9+6] = 1.0;

	vbuf[20*9+0] = vbuf[19*9+0] = vbuf[18*9+0] = cx - rx + fx + ux;
	vbuf[20*9+1] = vbuf[19*9+1] = vbuf[18*9+1] = cy - ry + fy + uy;
	vbuf[20*9+2] = vbuf[19*9+2] = vbuf[18*9+2] = cz - rz + fz + uz;
	vbuf[18*9+8] = 1.0;
	vbuf[19*9+7] = 1.0;
	vbuf[20*9+6] = -1.0;

	vbuf[23*9+0] = vbuf[22*9+0] = vbuf[21*9+0] = cx + rx + fx + ux;
	vbuf[23*9+1] = vbuf[22*9+1] = vbuf[21*9+1] = cy + ry + fy + uy;
	vbuf[23*9+2] = vbuf[22*9+2] = vbuf[21*9+2] = cz + rz + fz + uz;
	vbuf[21*9+8] = 1.0;
	vbuf[22*9+7] = 1.0;
	vbuf[23*9+6] = 1.0;

	//bottom
	ibuf[ 0] = vlen + 0 + 0;
	ibuf[ 1] = vlen + 0 + 3;
	ibuf[ 2] = vlen + 0 + 6;
	ibuf[ 3] = vlen + 0 + 3;
	ibuf[ 4] = vlen + 0 + 6;
	ibuf[ 5] = vlen + 0 + 9;

	//near
	ibuf[ 6] = vlen + 1 + 0;
	ibuf[ 7] = vlen + 1 + 3;
	ibuf[ 8] = vlen + 1 + 12;
	ibuf[ 9] = vlen + 1 + 3;
	ibuf[10] = vlen + 1 + 12;
	ibuf[11] = vlen + 1 + 15;

	//right
	ibuf[12] = vlen + 2 + 3;
	ibuf[13] = vlen + 2 + 9;
	ibuf[14] = vlen + 2 + 15;
	ibuf[15] = vlen + 2 + 9;
	ibuf[16] = vlen + 2 + 15;
	ibuf[17] = vlen + 2 + 21;

	//far
	ibuf[18] = vlen + 1 + 6;
	ibuf[19] = vlen + 1 + 9;
	ibuf[20] = vlen + 1 + 18;
	ibuf[21] = vlen + 1 + 9;
	ibuf[22] = vlen + 1 + 18;
	ibuf[23] = vlen + 1 + 21;

	//left
	ibuf[24] = vlen + 2 + 0;
	ibuf[25] = vlen + 2 + 6;
	ibuf[26] = vlen + 2 + 12;
	ibuf[27] = vlen + 2 + 6;
	ibuf[28] = vlen + 2 + 12;
	ibuf[29] = vlen + 2 + 18;

	//top
	ibuf[30] = vlen + 0 + 12;
	ibuf[31] = vlen + 0 + 15;
	ibuf[32] = vlen + 0 + 18;
	ibuf[33] = vlen + 0 + 15;
	ibuf[34] = vlen + 0 + 18;
	ibuf[35] = vlen + 0 + 21;
}
void carvesolid_prism5()
{
}
void carvesolid_prism6()
{
}
void carvesolid_cask(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int a,b,j;
	float s,t;
	float q[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += acc*2;
	mod[0x25].len += acc*2;

	for(j=0;j<acc;j++)
	{
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		vectornormalize(q);

		t = j*PI/acc;
		q[0] *= sine(t);
		q[1] *= sine(t);
		q[2] *= sine(t);
		q[3] = cosine(t);

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, q);

		a = j*18;
		b = j*6;

		vbuf[a+0] = cx - ux + r[0];
		vbuf[a+1] = cy - uy + r[1];
		vbuf[a+2] = cz - uz + r[2];
		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;
		vbuf[a+6] = vbuf[a+0] - cx;
		vbuf[a+7] = vbuf[a+1] - cy;
		vbuf[a+8] = vbuf[a+2] - cz;

		vbuf[a+ 9] = cx + ux + r[0];
		vbuf[a+10] = cy + uy + r[1];
		vbuf[a+11] = cz + uz + r[2];
		vbuf[a+12] = rr;
		vbuf[a+13] = gg;
		vbuf[a+14] = bb;
		vbuf[a+15] = vbuf[a+ 9] - cx;
		vbuf[a+16] = vbuf[a+10] - cy;
		vbuf[a+17] = vbuf[a+11] - cz;

		ibuf[b+0] = vlen + j*2;
		ibuf[b+1] = vlen + ((j+1)%acc)*2;
		ibuf[b+2] = vlen + 1 + j*2;

		ibuf[b+3] = vlen + 1 + ((j+1)%acc)*2;
		ibuf[b+4] = vlen + ((j+1)%acc)*2;
		ibuf[b+5] = vlen + 1 + j*2;
	}
}
void carvesolid_cylinder(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	carvesolid_circle(
		win, rgb,
		cx+ux, cy+uy, cz+uz,
		rx, ry, rz,
		ux, uy, uz
	);
	carvesolid_circle(
		win, rgb,
		cx-ux, cy-uy, cz-uz,
		rx, ry, rz,
		-ux, -uy, -uz
	);
	carvesolid_cask(
		win, rgb,
		cx, cy, cz,
		rx, ry, rz,
		ux, uy, uz
	);
/*
	int a,b,j,k;
	float s,t;
	float q[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += 4*acc;
	mod[0x25].len += 2*acc + 2;

	for(j=0;j<acc;j++)
	{
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		vectornormalize(q);

		t = j*PI/acc;
		q[0] *= sine(t);
		q[1] *= sine(t);
		q[2] *= sine(t);
		q[3] = cosine(t);

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, q);

		a = j*18;
		b = j*6;

		vbuf[a+0] = cx - ux + r[0];
		vbuf[a+1] = cy - uy + r[1];
		vbuf[a+2] = cz - uz + r[2];
		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;
		vbuf[a+6] = vbuf[a+0] - cx;
		vbuf[a+7] = vbuf[a+1] - cy;
		vbuf[a+8] = vbuf[a+2] - cz;

		vbuf[a+ 9] = cx + ux + r[0];
		vbuf[a+10] = cy + uy + r[1];
		vbuf[a+11] = cz + uz + r[2];
		vbuf[a+12] = rr;
		vbuf[a+13] = gg;
		vbuf[a+14] = bb;
		vbuf[a+15] = vbuf[a+ 9] - cx;
		vbuf[a+16] = vbuf[a+10] - cy;
		vbuf[a+17] = vbuf[a+11] - cz;

		ibuf[b+0] = vlen + j*2;
		ibuf[b+1] = vlen + ((j+1)%acc)*2;
		ibuf[b+2] = vlen + 1 + j*2;

		ibuf[b+3] = vlen + 1 + ((j+1)%acc)*2;
		ibuf[b+4] = vlen + ((j+1)%acc)*2;
		ibuf[b+5] = vlen + 1 + j*2;
	}

	a = acc*18;
	b = acc*6;

	vbuf[a+0] = cx-ux;
	vbuf[a+1] = cy-uy;
	vbuf[a+2] = cz-uz;
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
	vbuf[a+6] = -ux;
	vbuf[a+7] = -uy;
	vbuf[a+8] = -uz;

	vbuf[a+ 9] = cx+ux;
	vbuf[a+10] = cy+uy;
	vbuf[a+11] = cz+uz;
	vbuf[a+12] = rr;
	vbuf[a+13] = gg;
	vbuf[a+14] = bb;
	vbuf[a+15] = ux;
	vbuf[a+16] = uy;
	vbuf[a+17] = uz;

	for(j=0;j<acc;j++)
	{
		ibuf[b + (j*6) + 0] = vlen + acc*2;
		ibuf[b + (j*6) + 1] = vlen + (j*2);
		ibuf[b + (j*6) + 2] = vlen + ((j+1)%acc)*2;

		ibuf[b + (j*6) + 3] = vlen + acc*2 + 1;
		ibuf[b + (j*6) + 4] = vlen + 1 + (j*2);
		ibuf[b + (j*6) + 5] = vlen + 1 + ((j+1)%acc)*2;
	}
*/
}




void carvesolid_tetrahedron()
{
}
void carvesolid_octahedron()
{
}
void carvesolid_dodecahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz)
{
	int j;
	float a = 1.618;
	float b = 1.0/1.618;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += 36;
	mod[0x25].len += 20;

	//(+-1, +-1, +-1)
	vbuf[ 0] = cx-rx-fx-ux;
	vbuf[ 1] = cy-ry-fy-uy;
	vbuf[ 2] = cz-rz-fz-uz;

	vbuf[ 9] = cx+rx-fx-ux;
	vbuf[10] = cy+ry-fy-uy;
	vbuf[11] = cz+rz-fz-uz;

	vbuf[18] = cx-rx+fx-ux;
	vbuf[19] = cy-ry+fy-uy;
	vbuf[20] = cz-rz+fz-uz;

	vbuf[27] = cx+rx+fx-ux;
	vbuf[28] = cy+ry+fy-uy;
	vbuf[29] = cz+rz+fz-uz;

	vbuf[36] = cx-rx-fx+ux;
	vbuf[37] = cy-ry-fy+uy;
	vbuf[38] = cz-rz-fz+uz;

	vbuf[45] = cx+rx-fx+ux;
	vbuf[46] = cy+ry-fy+uy;
	vbuf[47] = cz+rz-fz+uz;

	vbuf[54] = cx-rx+fx+ux;
	vbuf[55] = cy-ry+fy+uy;
	vbuf[56] = cz-rz+fz+uz;

	vbuf[63] = cx+rx+fx+ux;
	vbuf[64] = cy+ry+fy+uy;
	vbuf[65] = cz+rz+fz+uz;

	//(0, +-a, +-b)
	vbuf[72] = cx - a*fx - b*ux;
	vbuf[73] = cy - a*fy - b*uy;
	vbuf[74] = cz - a*fz - b*uz;

	vbuf[81] = cx + a*fx - b*ux;
	vbuf[82] = cy + a*fy - b*uy;
	vbuf[83] = cz + a*fz - b*uz;

	vbuf[90] = cx - a*fx + b*ux;
	vbuf[91] = cy - a*fy + b*uy;
	vbuf[92] = cz - a*fz + b*uz;

	vbuf[ 99] = cx + a*fx + b*ux;
	vbuf[100] = cy + a*fy + b*uy;
	vbuf[101] = cz + a*fz + b*uz;

	//(+-b, 0, +-a)
	vbuf[108] = cx - b*rx - a*ux;
	vbuf[109] = cy - b*ry - a*uy;
	vbuf[110] = cz - b*rz - a*uz;

	vbuf[117] = cx + b*rx - a*ux;
	vbuf[118] = cy + b*ry - a*uy;
	vbuf[119] = cz + b*rz - a*uz;

	vbuf[126] = cx - b*rx + a*ux;
	vbuf[127] = cy - b*ry + a*uy;
	vbuf[128] = cz - b*rz + a*uz;

	vbuf[135] = cx + b*rx + a*ux;
	vbuf[136] = cy + b*ry + a*uy;
	vbuf[137] = cz + b*rz + a*uz;

	//(+-a, +-b, 0)
	vbuf[144] = cx - a*rx - b*fx;
	vbuf[145] = cy - a*ry - b*fy;
	vbuf[146] = cz - a*rz - b*fz;

	vbuf[153] = cx + a*rx - b*fx;
	vbuf[154] = cy + a*ry - b*fy;
	vbuf[155] = cz + a*rz - b*fz;

	vbuf[162] = cx - a*rx + b*fx;
	vbuf[163] = cy - a*ry + b*fy;
	vbuf[164] = cz - a*rz + b*fz;

	vbuf[171] = cx + a*rx + b*fx;
	vbuf[172] = cy + a*ry + b*fy;
	vbuf[173] = cz + a*rz + b*fz;

	for(j=0;j<20*9;j+=9)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
		vbuf[j + 6] = vbuf[j + 0] - cx;
		vbuf[j + 7] = vbuf[j + 1] - cy;
		vbuf[j + 8] = vbuf[j + 2] - cz;
	}

	//front
	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+8;
	ibuf[ 3] = vlen+0;
	ibuf[ 4] = vlen+1;
	ibuf[ 5] = vlen+12;
	ibuf[ 6] = vlen+12;
	ibuf[ 7] = vlen+13;
	ibuf[ 8] = vlen+1;

	ibuf[ 9] = vlen+4;
	ibuf[10] = vlen+5;
	ibuf[11] = vlen+10;
	ibuf[12] = vlen+4;
	ibuf[13] = vlen+5;
	ibuf[14] = vlen+14;
	ibuf[15] = vlen+14;
	ibuf[16] = vlen+15;
	ibuf[17] = vlen+5;

	//back
	ibuf[18] = vlen+2;
	ibuf[19] = vlen+3;
	ibuf[20] = vlen+9;
	ibuf[21] = vlen+2;
	ibuf[22] = vlen+3;
	ibuf[23] = vlen+12;
	ibuf[24] = vlen+12;
	ibuf[25] = vlen+13;
	ibuf[26] = vlen+3;

	ibuf[27] = vlen+6;
	ibuf[28] = vlen+7;
	ibuf[29] = vlen+11;
	ibuf[30] = vlen+6;
	ibuf[31] = vlen+7;
	ibuf[32] = vlen+14;
	ibuf[33] = vlen+14;
	ibuf[34] = vlen+15;
	ibuf[35] = vlen+7;

	//bottom
	ibuf[36] = vlen+0;
	ibuf[37] = vlen+2;
	ibuf[38] = vlen+12;
	ibuf[39] = vlen+0;
	ibuf[40] = vlen+2;
	ibuf[41] = vlen+16;
	ibuf[42] = vlen+16;
	ibuf[43] = vlen+18;
	ibuf[44] = vlen+2;

	ibuf[45] = vlen+1;
	ibuf[46] = vlen+3;
	ibuf[47] = vlen+13;
	ibuf[48] = vlen+1;
	ibuf[49] = vlen+3;
	ibuf[50] = vlen+17;
	ibuf[51] = vlen+17;
	ibuf[52] = vlen+19;
	ibuf[53] = vlen+3;

	//upper
	ibuf[54] = vlen+4;
	ibuf[55] = vlen+6;
	ibuf[56] = vlen+14;
	ibuf[57] = vlen+4;
	ibuf[58] = vlen+6;
	ibuf[59] = vlen+16;
	ibuf[60] = vlen+16;
	ibuf[61] = vlen+18;
	ibuf[62] = vlen+6;

	ibuf[63] = vlen+5;
	ibuf[64] = vlen+7;
	ibuf[65] = vlen+15;
	ibuf[66] = vlen+5;
	ibuf[67] = vlen+7;
	ibuf[68] = vlen+17;
	ibuf[69] = vlen+17;
	ibuf[70] = vlen+19;
	ibuf[71] = vlen+7;

	//left
	ibuf[72] = vlen+0;
	ibuf[73] = vlen+4;
	ibuf[74] = vlen+16;
	ibuf[75] = vlen+0;
	ibuf[76] = vlen+4;
	ibuf[77] = vlen+8;
	ibuf[78] = vlen+8;
	ibuf[79] = vlen+10;
	ibuf[80] = vlen+4;

	ibuf[81] = vlen+2;
	ibuf[82] = vlen+6;
	ibuf[83] = vlen+18;
	ibuf[84] = vlen+2;
	ibuf[85] = vlen+6;
	ibuf[86] = vlen+9;
	ibuf[87] = vlen+9;
	ibuf[88] = vlen+11;
	ibuf[89] = vlen+6;

	//right
	ibuf[90] = vlen+1;
	ibuf[91] = vlen+5;
	ibuf[92] = vlen+17;
	ibuf[93] = vlen+1;
	ibuf[94] = vlen+5;
	ibuf[95] = vlen+8;
	ibuf[96] = vlen+8;
	ibuf[97] = vlen+10;
	ibuf[98] = vlen+5;

	ibuf[99] = vlen+3;
	ibuf[100] = vlen+7;
	ibuf[101] = vlen+19;
	ibuf[102] = vlen+3;
	ibuf[103] = vlen+7;
	ibuf[104] = vlen+9;
	ibuf[105] = vlen+9;
	ibuf[106] = vlen+11;
	ibuf[107] = vlen+7;
}
void carvesolid_icosahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz)
{
	int j;
	float m = 0.52573111211913360602566908484788;
	float n = 0.85065080835203993218154049706301;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += 20;
	mod[0x25].len += 12;

	//(+-m, 0, +-n)
	vbuf[ 0] = cx - m;
	vbuf[ 1] = cy;
	vbuf[ 2] = cz - n;

	vbuf[ 9] = cx + m;
	vbuf[10] = cy;
	vbuf[11] = cz - n;

	vbuf[18] = cx - m;
	vbuf[19] = cy;
	vbuf[20] = cz + n;

	vbuf[27] = cx + m;
	vbuf[28] = cy;
	vbuf[29] = cz + n;

	//(0, +-n, +-m)
	vbuf[36] = cx;
	vbuf[37] = cy - n;
	vbuf[38] = cz - m;

	vbuf[45] = cx;
	vbuf[46] = cy + n;
	vbuf[47] = cz - m;

	vbuf[54] = cx;
	vbuf[55] = cy - n;
	vbuf[56] = cz + m;

	vbuf[63] = cx;
	vbuf[64] = cy + n;
	vbuf[65] = cz + m;

	//(+-n, +-m, 0)
	vbuf[72] = cx - n;
	vbuf[73] = cy - m;
	vbuf[74] = cz;

	vbuf[81] = cx + n;
	vbuf[82] = cy - m;
	vbuf[83] = cz;

	vbuf[90] = cx - n;
	vbuf[91] = cy + m;
	vbuf[92] = cz;

	vbuf[99] = cx + n;
	vbuf[100] = cy + m;
	vbuf[101] = cz;

	for(j=0;j<12*9;j+=9)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
		vbuf[j + 6] = vbuf[j + 0] - cx;
		vbuf[j + 7] = vbuf[j + 1] - cy;
		vbuf[j + 8] = vbuf[j + 2] - cz;
	}

	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+5;

	ibuf[ 3] = vlen+0;
	ibuf[ 4] = vlen+5;
	ibuf[ 5] = vlen+10;

	ibuf[ 6] = vlen+0;
	ibuf[ 7] = vlen+10;
	ibuf[ 8] = vlen+8;

	ibuf[ 9] = vlen+0;
	ibuf[10] = vlen+8;
	ibuf[11] = vlen+4;

	ibuf[12] = vlen+0;
	ibuf[13] = vlen+1;
	ibuf[14] = vlen+4;

	ibuf[15] = vlen+3;
	ibuf[16] = vlen+2;
	ibuf[17] = vlen+6;

	ibuf[18] = vlen+3;
	ibuf[19] = vlen+6;
	ibuf[20] = vlen+9;

	ibuf[21] = vlen+3;
	ibuf[22] = vlen+9;
	ibuf[23] = vlen+11;

	ibuf[24] = vlen+3;
	ibuf[25] = vlen+11;
	ibuf[26] = vlen+7;

	ibuf[27] = vlen+3;
	ibuf[28] = vlen+2;
	ibuf[29] = vlen+7;

	ibuf[30] = vlen+1;
	ibuf[31] = vlen+11;
	ibuf[32] = vlen+9;

	ibuf[33] = vlen+5;
	ibuf[34] = vlen+7;
	ibuf[35] = vlen+11;

	ibuf[36] = vlen+10;
	ibuf[37] = vlen+2;
	ibuf[38] = vlen+7;

	ibuf[39] = vlen+8;
	ibuf[40] = vlen+6;
	ibuf[41] = vlen+2;

	ibuf[42] = vlen+4;
	ibuf[43] = vlen+9;
	ibuf[44] = vlen+6;

	ibuf[45] = vlen+1;
	ibuf[46] = vlen+11;
	ibuf[47] = vlen+5;

	ibuf[48] = vlen+5;
	ibuf[49] = vlen+7;
	ibuf[50] = vlen+10;

	ibuf[51] = vlen+10;
	ibuf[52] = vlen+2;
	ibuf[53] = vlen+8;

	ibuf[54] = vlen+8;
	ibuf[55] = vlen+6;
	ibuf[56] = vlen+4;

	ibuf[57] = vlen+4;
	ibuf[58] = vlen+9;
	ibuf[59] = vlen+1;
}
void carvesolid_sphere(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
#define odd ((acc&0xfffe)+1)
	int a,b,j,k;
	float s,t;
	float q[4];
	float tempcx,tempcy,tempcz;
	float temprx,tempry,temprz;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += odd*(odd-2)*2;
	mod[0x25].len += odd*(odd-2)+2;

	for(k=0;k<(odd-2);k++)
	{
		s = (k+1-(odd/2))*PI/(odd-1);
		t = cosine(s);
		temprx = rx*t;
		tempry = ry*t;
		temprz = rz*t;

		t = sine(s);
		tempcx = cx + ux*t;
		tempcy = cy + uy*t;
		tempcz = cz + uz*t;

		for(j=0;j<odd;j++)
		{
			q[0] = ux;
			q[1] = uy;
			q[2] = uz;
			vectornormalize(q);

			t = j*PI/odd;
			q[0] *= sine(t);
			q[1] *= sine(t);
			q[2] *= sine(t);
			q[3] = cosine(t);

			a = (k*odd + j)*9;

			vbuf[a+0] = temprx;
			vbuf[a+1] = tempry;
			vbuf[a+2] = temprz;
			quaternionrotate(&vbuf[a], q);

			vbuf[a+0] += tempcx;
			vbuf[a+1] += tempcy;
			vbuf[a+2] += tempcz;
			vbuf[a+3] = rr;
			vbuf[a+4] = gg;
			vbuf[a+5] = bb;
			vbuf[a+6] = vbuf[a+0] - cx;
			vbuf[a+7] = vbuf[a+1] - cy;
			vbuf[a+8] = vbuf[a+2] - cz;
		}
	}
	for(k=0;k<(odd-3);k++)
	{
		a = k*odd*6;
		for(j=0;j<odd;j++)
		{
			ibuf[a + 6*j + 0] = vlen+(k*odd)+j;
			ibuf[a + 6*j + 1] = vlen+(k*odd)+(j+1)%odd;
			ibuf[a + 6*j + 2] = vlen+(k*odd)+odd+j;

			ibuf[a + 6*j + 3] = vlen+(k*odd)+(j+1)%odd;
			ibuf[a + 6*j + 4] = vlen+(k*odd)+odd+j;
			ibuf[a + 6*j + 5] = vlen+(k*odd)+odd+(j+1)%odd;
		}
	}

	//
	a = odd*(odd-2)*9;
	b = odd*(odd-3)*6;

	vbuf[a+0] = cx-ux;
	vbuf[a+1] = cy-uy;
	vbuf[a+2] = cz-uz;
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
	vbuf[a+6] = -ux;
	vbuf[a+7] = -uy;
	vbuf[a+8] = -uz;

	vbuf[a+ 9] = cx+ux;
	vbuf[a+10] = cy+uy;
	vbuf[a+11] = cz+uz;
	vbuf[a+12] = rr;
	vbuf[a+13] = gg;
	vbuf[a+14] = bb;
	vbuf[a+15] = ux;
	vbuf[a+16] = uy;
	vbuf[a+17] = uz;

	for(j=0;j<odd;j++)
	{
		ibuf[b + (3*j) +0] = vlen+odd*(odd-2);
		ibuf[b + (3*j) +1] = vlen+j;
		ibuf[b + (3*j) +2] = vlen+(j+1)%odd;

		ibuf[b + (3*j) +(odd*3) + 0] = vlen+odd*(odd-2)+1;
		ibuf[b + (3*j) +(odd*3) + 1] = vlen+odd*(odd-3)+j;
		ibuf[b + (3*j) +(odd*3) + 2] = vlen+odd*(odd-3)+(j+1)%odd;
	}
}
void carvesolid_tokamak(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[0x24].len;
	int vlen = mod[0x25].len;
	u16* ibuf = (mod[0x24].buf) + (6*ilen);
	float* vbuf = (mod[0x25].buf) + (36*vlen);
	mod[0x24].len += acc*acc*2;
	mod[0x25].len += acc*acc;
}
