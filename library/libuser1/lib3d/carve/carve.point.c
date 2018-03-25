#include "actor.h"
#define acc 18
#define pointv 0x21
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




void carvepoint(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	float* buf  = (mod[pointv].buf) + (24*mod[pointv].len);
	mod[pointv].len += 1;

	buf[0] = cx;
	buf[1] = cy;
	buf[2] = cz;
	buf[3] = rr;
	buf[4] = gg;
	buf[5] = bb;
}
void carvepoint_bezier(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float xc, float yc, float zc)
{
	int j;
	float t;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	float* buf  = (mod[pointv].buf) + (24*mod[pointv].len);
	mod[pointv].len += acc;

	for(j=0;j<=acc;j++)
	{
		t = (float)j / acc;

		buf[6*j+0] = (1.0-t)*(1.0-t)*x1 + 2*t*(1.0-t)*xc + t*t*x2;
		buf[6*j+1] = (1.0-t)*(1.0-t)*y1 + 2*t*(1.0-t)*yc + t*t*y2;
		buf[6*j+2] = (1.0-t)*(1.0-t)*z1 + 2*t*(1.0-t)*zc + t*t*z2;
		buf[6*j+3] = rr;
		buf[6*j+4] = gg;
		buf[6*j+5] = bb;
	}
}




void carvepoint_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
}
void carvepoint_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
}
void carvepoint_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int j,k;
	float s,t;
	float q[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	float* buf  = (mod[pointv].buf) + (24*mod[pointv].len);
	mod[pointv].len += acc;

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

		buf[6*j+0] = cx + r[0];
		buf[6*j+1] = cy + r[1];
		buf[6*j+2] = cz + r[2];
		buf[6*j+3] = rr;
		buf[6*j+4] = gg;
		buf[6*j+5] = bb;
	}
}




void carvepoint_pyramid3()
{
}
void carvepoint_pyramid4()
{
}
void carvepoint_pyramid5()
{
}
void carvepoint_pyramid6()
{
}
void carvepoint_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int j,k;
	float s,t;
	float q[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	float* buf  = (mod[pointv].buf) + (24*mod[pointv].len);
	mod[pointv].len += acc+2;

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

		buf[6*j+0] = cx + r[0];
		buf[6*j+1] = cy + r[1];
		buf[6*j+2] = cz + r[2];
		buf[6*j+3] = rr;
		buf[6*j+4] = gg;
		buf[6*j+5] = bb;
	}

	j = acc*6;

	buf[j+ 0] = cx;
	buf[j+ 1] = cy;
	buf[j+ 2] = cz;
	buf[j+ 3] = rr;
	buf[j+ 4] = gg;
	buf[j+ 5] = bb;

	buf[j+ 6] = cx+ux;
	buf[j+ 7] = cy+uy;
	buf[j+ 8] = cz+uz;
	buf[j+ 9] = rr;
	buf[j+10] = gg;
	buf[j+11] = bb;
}




void carvepoint_prism3()
{
}
void carvepoint_prism4()
{
}
void carvepoint_prism5()
{
}
void carvepoint_prism6()
{
}
void carvepoint_cask(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
}
void carvepoint_cylinder(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int j,k;
	float s,t;
	float q[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	float* buf  = (mod[pointv].buf) + (24*mod[pointv].len);
	mod[pointv].len += acc*2;

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

		k = j*12;

		buf[k+ 0] = cx - ux + r[0];
		buf[k+ 1] = cy - uy + r[1];
		buf[k+ 2] = cz - uz + r[2];
		buf[k+ 3] = rr;
		buf[k+ 4] = gg;
		buf[k+ 5] = bb;

		buf[k+ 6] = cx + ux + r[0];
		buf[k+ 7] = cy + uy + r[1];
		buf[k+ 8] = cz + uz + r[2];
		buf[k+ 9] = rr;
		buf[k+10] = gg;
		buf[k+11] = bb;
	}
}




void carvepoint_tetrahedron()
{
}
void carvepoint_octahedron()
{
}
void carvepoint_dodecahedron(
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
	float* buf  = (mod[pointv].buf) + (24*mod[pointv].len);
	mod[pointv].len += 20;

	for(j=0;j<20*6;j+=6)
	{
		buf[j + 3] = rr;
		buf[j + 4] = gg;
		buf[j + 5] = bb;
	}

	//(+-1, +-1, +-1)
	buf[  0] = cx-rx-fx-ux;
	buf[  1] = cy-ry-fy-uy;
	buf[  2] = cz-rz-fz-uz;

	buf[  6] = cx+rx-fx-ux;
	buf[  7] = cy+ry-fy-uy;
	buf[  8] = cz+rz-fz-uz;

	buf[ 12] = cx-rx+fx-ux;
	buf[ 13] = cy-ry+fy-uy;
	buf[ 14] = cz-rz+fz-uz;

	buf[ 18] = cx+rx+fx-ux;
	buf[ 19] = cy+ry+fy-uy;
	buf[ 20] = cz+rz+fz-uz;

	buf[ 24] = cx-rx-fx+ux;
	buf[ 25] = cy-ry-fy+uy;
	buf[ 26] = cz-rz-fz+uz;

	buf[ 30] = cx+rx-fx+ux;
	buf[ 31] = cy+ry-fy+uy;
	buf[ 32] = cz+rz-fz+uz;

	buf[ 36] = cx-rx+fx+ux;
	buf[ 37] = cy-ry+fy+uy;
	buf[ 38] = cz-rz+fz+uz;

	buf[ 42] = cx+rx+fx+ux;
	buf[ 43] = cy+ry+fy+uy;
	buf[ 44] = cz+rz+fz+uz;

	//(0, +-a, +-b)
	buf[ 48] = cx - a*fx - b*ux;
	buf[ 49] = cy - a*fy - b*uy;
	buf[ 50] = cz - a*fz - b*uz;

	buf[ 54] = cx + a*fx - b*ux;
	buf[ 55] = cy + a*fy - b*uy;
	buf[ 56] = cz + a*fz - b*uz;

	buf[ 60] = cx - a*fx + b*ux;
	buf[ 61] = cy - a*fy + b*uy;
	buf[ 62] = cz - a*fz + b*uz;

	buf[ 66] = cx + a*fx + b*ux;
	buf[ 67] = cy + a*fy + b*uy;
	buf[ 68] = cz + a*fz + b*uz;

	//(+-b, 0, +-a)
	buf[ 72] = cx - b*rx - a*ux;
	buf[ 73] = cy - b*ry - a*uy;
	buf[ 74] = cz - b*rz - a*uz;

	buf[ 78] = cx + b*rx - a*ux;
	buf[ 79] = cy + b*ry - a*uy;
	buf[ 80] = cz + b*rz - a*uz;

	buf[ 84] = cx - b*rx + a*ux;
	buf[ 85] = cy - b*ry + a*uy;
	buf[ 86] = cz - b*rz + a*uz;

	buf[ 90] = cx + b*rx + a*ux;
	buf[ 91] = cy + b*ry + a*uy;
	buf[ 92] = cz + b*rz + a*uz;

	//(+-a, +-b, 0)
	buf[ 96] = cx - a*rx - b*fx;
	buf[ 97] = cy - a*ry - b*fy;
	buf[ 98] = cz - a*rz - b*fz;

	buf[102] = cx + a*rx - b*fx;
	buf[103] = cy + a*ry - b*fy;
	buf[104] = cz + a*rz - b*fz;

	buf[108] = cx - a*rx + b*fx;
	buf[109] = cy - a*ry + b*fy;
	buf[110] = cz - a*rz + b*fz;

	buf[114] = cx + a*rx + b*fx;
	buf[115] = cy + a*ry + b*fy;
	buf[116] = cz + a*rz + b*fz;
}
void carvepoint_icosahedron(
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
	float* buf  = (mod[pointv].buf) + (24*mod[pointv].len);
	mod[pointv].len += 12;

	for(j=0;j<12*6;j+=6)
	{
		buf[j + 0] = rr;
		buf[j + 1] = gg;
		buf[j + 2] = bb;
	}

	//(+-m, 0, +-n)
	buf[ 0] = cx - m;
	buf[ 1] = cy;
	buf[ 2] = cz - n;

	buf[ 6] = cx + m;
	buf[ 7] = cy;
	buf[ 8] = cz - n;

	buf[12] = cx - m;
	buf[13] = cy;
	buf[14] = cz + n;

	buf[18] = cx + m;
	buf[19] = cy;
	buf[20] = cz + n;

	//(0, +-n, +-m)
	buf[24] = cx;
	buf[25] = cy - n;
	buf[26] = cz - m;

	buf[30] = cx;
	buf[31] = cy + n;
	buf[32] = cz - m;

	buf[36] = cx;
	buf[37] = cy - n;
	buf[38] = cz + m;

	buf[42] = cx;
	buf[43] = cy + n;
	buf[44] = cz + m;

	//(+-n, +-m, 0)
	buf[48] = cx - n;
	buf[49] = cy - m;
	buf[50] = cz;

	buf[54] = cx + n;
	buf[55] = cy - m;
	buf[56] = cz;

	buf[60] = cx - n;
	buf[61] = cy + m;
	buf[62] = cz;

	buf[66] = cx + n;
	buf[67] = cy + m;
	buf[68] = cz;
}
void carvepoint_sphere(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
#define odd ((acc&0xfffe)+1)
	int j,k,a;
	float s,t;
	float q[4];
	float tempcx,tempcy,tempcz;
	float temprx,tempry,temprz;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	float* buf  = (mod[pointv].buf) + (24*mod[pointv].len);
	mod[pointv].len += odd*(odd-2)+2;

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

			a = (k*odd + j)*6;

			buf[a+0] = temprx;
			buf[a+1] = tempry;
			buf[a+2] = temprz;
			quaternionrotate(&buf[a], q);

			buf[a+0] += tempcx;
			buf[a+1] += tempcy;
			buf[a+2] += tempcz;

			buf[a+3] = rr;
			buf[a+4] = gg;
			buf[a+5] = bb;
		}
	}

	a = odd*(odd/2)*6;

	buf[a+ 0] = cx-ux;
	buf[a+ 1] = cy-uy;
	buf[a+ 2] = cz-uz;
	buf[a+ 3] = rr;
	buf[a+ 4] = gg;
	buf[a+ 5] = bb;

	buf[a+ 6] = cx+ux;
	buf[a+ 7] = cy+uy;
	buf[a+ 8] = cz+uz;
	buf[a+ 9] = rr;
	buf[a+10] = gg;
	buf[a+11] = bb;
}