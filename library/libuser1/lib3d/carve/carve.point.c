#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 18
#define pointv 0x81
void quaternionoperation(float*, float*, float);




void carvepoint(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += 1;

	vbuf[0] = cx;
	vbuf[1] = cy;
	vbuf[2] = cz;
	vbuf[3] = rr;
	vbuf[4] = gg;
	vbuf[5] = bb;
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

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += acc;

	for(j=0;j<=acc;j++)
	{
		t = (float)j / acc;

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*x1 + 2*t*(1.0-t)*xc + t*t*x2;
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*y1 + 2*t*(1.0-t)*yc + t*t*y2;
		vbuf[6*j+2] = (1.0-t)*(1.0-t)*z1 + 2*t*(1.0-t)*zc + t*t*z2;
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
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

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += acc;

	for(j=0;j<acc;j++)
	{
		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		quaternionoperation(r, q, j*tau/acc);

		vbuf[6*j+0] = cx + r[0];
		vbuf[6*j+1] = cy + r[1];
		vbuf[6*j+2] = cz + r[2];
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
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

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += acc+2;

	for(j=0;j<acc;j++)
	{
		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		quaternionoperation(r, q, j*tau/acc);

		vbuf[6*j+0] = cx + r[0];
		vbuf[6*j+1] = cy + r[1];
		vbuf[6*j+2] = cz + r[2];
		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;
	}

	j = acc*6;

	vbuf[j+ 0] = cx;
	vbuf[j+ 1] = cy;
	vbuf[j+ 2] = cz;
	vbuf[j+ 3] = rr;
	vbuf[j+ 4] = gg;
	vbuf[j+ 5] = bb;

	vbuf[j+ 6] = cx+ux;
	vbuf[j+ 7] = cy+uy;
	vbuf[j+ 8] = cz+uz;
	vbuf[j+ 9] = rr;
	vbuf[j+10] = gg;
	vbuf[j+11] = bb;
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

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += acc*2;

	for(j=0;j<acc;j++)
	{
		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		quaternionoperation(r, q, j*tau/acc);

		k = j*12;

		vbuf[k+ 0] = cx - ux + r[0];
		vbuf[k+ 1] = cy - uy + r[1];
		vbuf[k+ 2] = cz - uz + r[2];
		vbuf[k+ 3] = rr;
		vbuf[k+ 4] = gg;
		vbuf[k+ 5] = bb;

		vbuf[k+ 6] = cx + ux + r[0];
		vbuf[k+ 7] = cy + uy + r[1];
		vbuf[k+ 8] = cz + uz + r[2];
		vbuf[k+ 9] = rr;
		vbuf[k+10] = gg;
		vbuf[k+11] = bb;
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

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += 20;

	for(j=0;j<20*6;j+=6)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}

	//(+-1, +-1, +-1)
	vbuf[  0] = cx-rx-fx-ux;
	vbuf[  1] = cy-ry-fy-uy;
	vbuf[  2] = cz-rz-fz-uz;

	vbuf[  6] = cx+rx-fx-ux;
	vbuf[  7] = cy+ry-fy-uy;
	vbuf[  8] = cz+rz-fz-uz;

	vbuf[ 12] = cx-rx+fx-ux;
	vbuf[ 13] = cy-ry+fy-uy;
	vbuf[ 14] = cz-rz+fz-uz;

	vbuf[ 18] = cx+rx+fx-ux;
	vbuf[ 19] = cy+ry+fy-uy;
	vbuf[ 20] = cz+rz+fz-uz;

	vbuf[ 24] = cx-rx-fx+ux;
	vbuf[ 25] = cy-ry-fy+uy;
	vbuf[ 26] = cz-rz-fz+uz;

	vbuf[ 30] = cx+rx-fx+ux;
	vbuf[ 31] = cy+ry-fy+uy;
	vbuf[ 32] = cz+rz-fz+uz;

	vbuf[ 36] = cx-rx+fx+ux;
	vbuf[ 37] = cy-ry+fy+uy;
	vbuf[ 38] = cz-rz+fz+uz;

	vbuf[ 42] = cx+rx+fx+ux;
	vbuf[ 43] = cy+ry+fy+uy;
	vbuf[ 44] = cz+rz+fz+uz;

	//(0, +-a, +-b)
	vbuf[ 48] = cx - a*fx - b*ux;
	vbuf[ 49] = cy - a*fy - b*uy;
	vbuf[ 50] = cz - a*fz - b*uz;

	vbuf[ 54] = cx + a*fx - b*ux;
	vbuf[ 55] = cy + a*fy - b*uy;
	vbuf[ 56] = cz + a*fz - b*uz;

	vbuf[ 60] = cx - a*fx + b*ux;
	vbuf[ 61] = cy - a*fy + b*uy;
	vbuf[ 62] = cz - a*fz + b*uz;

	vbuf[ 66] = cx + a*fx + b*ux;
	vbuf[ 67] = cy + a*fy + b*uy;
	vbuf[ 68] = cz + a*fz + b*uz;

	//(+-b, 0, +-a)
	vbuf[ 72] = cx - b*rx - a*ux;
	vbuf[ 73] = cy - b*ry - a*uy;
	vbuf[ 74] = cz - b*rz - a*uz;

	vbuf[ 78] = cx + b*rx - a*ux;
	vbuf[ 79] = cy + b*ry - a*uy;
	vbuf[ 80] = cz + b*rz - a*uz;

	vbuf[ 84] = cx - b*rx + a*ux;
	vbuf[ 85] = cy - b*ry + a*uy;
	vbuf[ 86] = cz - b*rz + a*uz;

	vbuf[ 90] = cx + b*rx + a*ux;
	vbuf[ 91] = cy + b*ry + a*uy;
	vbuf[ 92] = cz + b*rz + a*uz;

	//(+-a, +-b, 0)
	vbuf[ 96] = cx - a*rx - b*fx;
	vbuf[ 97] = cy - a*ry - b*fy;
	vbuf[ 98] = cz - a*rz - b*fz;

	vbuf[102] = cx + a*rx - b*fx;
	vbuf[103] = cy + a*ry - b*fy;
	vbuf[104] = cz + a*rz - b*fz;

	vbuf[108] = cx - a*rx + b*fx;
	vbuf[109] = cy - a*ry + b*fy;
	vbuf[110] = cz - a*rz + b*fz;

	vbuf[114] = cx + a*rx + b*fx;
	vbuf[115] = cy + a*ry + b*fy;
	vbuf[116] = cz + a*rz + b*fz;
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

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += 12;

	for(j=0;j<12*6;j+=6)
	{
		vbuf[j + 0] = rr;
		vbuf[j + 1] = gg;
		vbuf[j + 2] = bb;
	}

	//(+-m, 0, +-n)
	vbuf[ 0] = cx - m;
	vbuf[ 1] = cy;
	vbuf[ 2] = cz - n;

	vbuf[ 6] = cx + m;
	vbuf[ 7] = cy;
	vbuf[ 8] = cz - n;

	vbuf[12] = cx - m;
	vbuf[13] = cy;
	vbuf[14] = cz + n;

	vbuf[18] = cx + m;
	vbuf[19] = cy;
	vbuf[20] = cz + n;

	//(0, +-n, +-m)
	vbuf[24] = cx;
	vbuf[25] = cy - n;
	vbuf[26] = cz - m;

	vbuf[30] = cx;
	vbuf[31] = cy + n;
	vbuf[32] = cz - m;

	vbuf[36] = cx;
	vbuf[37] = cy - n;
	vbuf[38] = cz + m;

	vbuf[42] = cx;
	vbuf[43] = cy + n;
	vbuf[44] = cz + m;

	//(+-n, +-m, 0)
	vbuf[48] = cx - n;
	vbuf[49] = cy - m;
	vbuf[50] = cz;

	vbuf[54] = cx + n;
	vbuf[55] = cy - m;
	vbuf[56] = cz;

	vbuf[60] = cx - n;
	vbuf[61] = cy + m;
	vbuf[62] = cz;

	vbuf[66] = cx + n;
	vbuf[67] = cy + m;
	vbuf[68] = cz;
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

	struct texandobj* mod = win->mod;
	float* vbuf  = (mod[pointv].vbuf) + (24*mod[pointv].vlen);
	mod[pointv].vlen += odd*(odd-2)+2;

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
			a = (k*odd + j)*6;
			vbuf[a+0] = temprx;
			vbuf[a+1] = tempry;
			vbuf[a+2] = temprz;
			q[0] = ux;
			q[1] = uy;
			q[2] = uz;
			quaternionoperation(&vbuf[a], q, j*tau/odd);

			vbuf[a+0] += tempcx;
			vbuf[a+1] += tempcy;
			vbuf[a+2] += tempcz;

			vbuf[a+3] = rr;
			vbuf[a+4] = gg;
			vbuf[a+5] = bb;
		}
	}

	a = odd*(odd/2)*6;

	vbuf[a+ 0] = cx-ux;
	vbuf[a+ 1] = cy-uy;
	vbuf[a+ 2] = cz-uz;
	vbuf[a+ 3] = rr;
	vbuf[a+ 4] = gg;
	vbuf[a+ 5] = bb;

	vbuf[a+ 6] = cx+ux;
	vbuf[a+ 7] = cy+uy;
	vbuf[a+ 8] = cz+uz;
	vbuf[a+ 9] = rr;
	vbuf[a+10] = gg;
	vbuf[a+11] = bb;
}