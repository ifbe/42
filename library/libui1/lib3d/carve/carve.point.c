#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void matrixmultiply_4(float*, float*);
void quaternionnormalize(float*);
void quaternionrotate(float*, float*);
//
void vectornormalize(float*);
void vectorcross(float*, float*);
float vectordot(float*, float*);
float vectorcosine(float*, float*);
//
double squareroot(double);
double cosine(double);
double sine(double);




void carvepoint(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[12];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xc00000 + (icount*2);

	win->info[8] += 1;
	win->info[9] += 1;
	win->info[10] += 1;
	win->info[12] += 1;

	color[0] = rr;
	color[1] = gg;
	color[2] = bb;

	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;

	vertex[0] = cx;
	vertex[1] = cy;
	vertex[2] = cz;

	index[0] = pcount;
}




void carvecylinder_point(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int a,b,j,k;
	float s,t;
	float v[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[12];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xc00000 + (icount*2);

	win->info[8] += 64;
	win->info[9] += 64;
	win->info[10] += 64;
	win->info[12] += 64;

	for(j=0;j<32;j++)
	{
		v[0] = ux;
		v[1] = uy;
		v[2] = uz;
		vectornormalize(v);

		t = j*PI/16.0;
		v[0] *= sine(t);
		v[1] *= sine(t);
		v[2] *= sine(t);
		v[3] = cosine(t);

		b = j*2;
		a = j*6;

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, v);

		vertex[a+0] = cx - ux + r[0];
		vertex[a+1] = cy - uy + r[1];
		vertex[a+2] = cz - uz + r[2];
		vertex[a+3] = cx + ux + r[0];
		vertex[a+4] = cy + uy + r[1];
		vertex[a+5] = cz + uz + r[2];

		normal[a+0] = vertex[a+0] - cx;
		normal[a+1] = vertex[a+1] - cy;
		normal[a+2] = vertex[a+2] - cz;
		normal[a+3] = vertex[a+3] - cx;
		normal[a+4] = vertex[a+4] - cy;
		normal[a+5] = vertex[a+5] - cz;

		color[a+0] = rr;
		color[a+1] = gg;
		color[a+2] = bb;
		color[a+3] = rr;
		color[a+4] = gg;
		color[a+5] = bb;

		index[b+0] = pcount + 2*j + 0;
		index[b+1] = pcount + 2*j + 1;
	}
}




void carvesphere_point(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int a,b,j,k;
	float s,t;
	float v[4];
	float tempcx,tempcy,tempcz;
	float temprx,tempry,temprz;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[12];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xc00000 + (icount*2);

	win->info[8] += 36*17+2;
	win->info[9] += 36*17+2;
	win->info[10] += 36*17+2;
	win->info[12] += 36*17+2;

	for(k=-8;k<9;k++)
	{
		s = k*PI/18.0;
		t = cosine(s);
		temprx = rx*t;
		tempry = ry*t;
		temprz = rz*t;

		t = sine(s);
		tempcx = cx + ux*t;
		tempcy = cy + uy*t;
		tempcz = cz + uz*t;

		for(j=-18;j<18;j++)
		{
			v[0] = ux;
			v[1] = uy;
			v[2] = uz;
			vectornormalize(v);

			t = j*PI/18.0;
			v[0] *= sine(t);
			v[1] *= sine(t);
			v[2] *= sine(t);
			v[3] = cosine(t);

			b = (k+8)*36 + (j+18);
			a = b*3;

			vertex[a+0] = temprx;
			vertex[a+1] = tempry;
			vertex[a+2] = temprz;
			quaternionrotate(&vertex[a], v);

			vertex[a+0] += tempcx;
			vertex[a+1] += tempcy;
			vertex[a+2] += tempcz;

			normal[a+0] = vertex[a+0] - cx;
			normal[a+1] = vertex[a+1] - cy;
			normal[a+2] = vertex[a+2] - cz;

			color[a+0] = rr;
			color[a+1] = gg;
			color[a+2] = bb;

			index[b] = pcount + ((k+8)*36) + (j+18);
		}
	}

	a = 36*17*3;
	b = 36*17;

	vertex[a+0] = cx-ux;
	vertex[a+1] = cy-uy;
	vertex[a+2] = cz-uz;
	vertex[a+3] = cx+ux;
	vertex[a+4] = cy+uy;
	vertex[a+5] = cz+uz;

	normal[a+0] = 0.0;
	normal[a+1] = 0.0;
	normal[a+2] = 1.0;
	normal[a+3] = 0.0;
	normal[a+4] = 0.0;
	normal[a+5] = 1.0;

	color[a+0] = rr;
	color[a+1] = gg;
	color[a+2] = bb;
	color[a+3] = rr;
	color[a+4] = gg;
	color[a+5] = bb;

	index[b+0] = pcount + (36*17) + 0;
	index[b+1] = pcount + (36*17) + 1;
}