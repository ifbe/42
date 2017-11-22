#include "actor.h"
#define accuracy 18
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




//正3棱柱
void carvesolid_prism3()
{
}
//正4棱柱
void carvesolid_prism4(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[14];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xe00000 + (icount*2);

	win->info[8] += 8;
	win->info[9] += 8;
	win->info[10] += 8;
	win->info[14] += 36;

	//color
	color[0] = rr;
	color[1] = gg;
	color[2] = bb;

	color[3] = rr;
	color[4] = gg;
	color[5] = bb;

	color[6] = rr;
	color[7] = gg;
	color[8] = bb;

	color[9] = rr;
	color[10] = gg;
	color[11] = bb;

	color[12] = rr;
	color[13] = gg;
	color[14] = bb;

	color[15] = rr;
	color[16] = gg;
	color[17] = bb;

	color[18] = rr;
	color[19] = gg;
	color[20] = bb;

	color[21] = rr;
	color[22] = gg;
	color[23] = bb;

	//normal
	normal[0] = - rx - fx - ux;
	normal[1] = - ry - fy - uy;
	normal[2] = - rz - fz - uz;

	normal[3] = rx - fx - ux;
	normal[4] = ry - fy - uy;
	normal[5] = rz - fz - uz;

	normal[6] = - rx + fx - ux;
	normal[7] = - ry + fy - uy;
	normal[8] = - rz + fz - uz;

	normal[9] = rx + fx - ux;
	normal[10] = ry + fy - uy;
	normal[11] = rz + fz - uz;

	normal[12] = - rx - fx + ux;
	normal[13] = - ry - fy + uy;
	normal[14] = - rz - fz + uz;

	normal[15] = rx - fx + ux;
	normal[16] = ry - fy + uy;
	normal[17] = rz - fz + uz;

	normal[18] = - rx + fx + ux;
	normal[19] = - ry + fy + uy;
	normal[20] = - rz + fz + uz;

	normal[21] = rx + fx + ux;
	normal[22] = ry + fy + uy;
	normal[23] = rz + fz + uz;

	//vertex
	vertex[ 0] = cx - rx - fx - ux;
	vertex[ 1] = cy - ry - fy - uy;
	vertex[ 2] = cz - rz - fz - uz;

	vertex[ 3] = cx + rx - fx - ux;
	vertex[ 4] = cy + ry - fy - uy;
	vertex[ 5] = cz + rz - fz - uz;

	vertex[ 6] = cx - rx + fx - ux;
	vertex[ 7] = cy - ry + fy - uy;
	vertex[ 8] = cz - rz + fz - uz;

	vertex[ 9] = cx + rx + fx - ux;
	vertex[10] = cy + ry + fy - uy;
	vertex[11] = cz + rz + fz - uz;

	vertex[12] = cx - rx - fx + ux;
	vertex[13] = cy - ry - fy + uy;
	vertex[14] = cz - rz - fz + uz;

	vertex[15] = cx + rx - fx + ux;
	vertex[16] = cy + ry - fy + uy;
	vertex[17] = cz + rz - fz + uz;

	vertex[18] = cx - rx + fx + ux;
	vertex[19] = cy - ry + fy + uy;
	vertex[20] = cz - rz + fz + uz;

	vertex[21] = cx + rx + fx + ux;
	vertex[22] = cy + ry + fy + uy;
	vertex[23] = cz + rz + fz + uz;

	//index
	index[0] = pcount + 0;
	index[1] = pcount + 1;
	index[2] = pcount + 2;
	index[3] = pcount + 1;
	index[4] = pcount + 2;
	index[5] = pcount + 3;

	index[6] = pcount + 0;
	index[7] = pcount + 1;
	index[8] = pcount + 4;
	index[9] = pcount + 1;
	index[10] = pcount + 4;
	index[11] = pcount + 5;

	index[12] = pcount + 1;
	index[13] = pcount + 3;
	index[14] = pcount + 5;
	index[15] = pcount + 3;
	index[16] = pcount + 5;
	index[17] = pcount + 7;

	index[18] = pcount + 2;
	index[19] = pcount + 3;
	index[20] = pcount + 6;
	index[21] = pcount + 3;
	index[22] = pcount + 6;
	index[23] = pcount + 7;

	index[24] = pcount + 0;
	index[25] = pcount + 2;
	index[26] = pcount + 4;
	index[27] = pcount + 2;
	index[28] = pcount + 4;
	index[29] = pcount + 6;

	index[30] = pcount + 4;
	index[31] = pcount + 5;
	index[32] = pcount + 6;
	index[33] = pcount + 5;
	index[34] = pcount + 6;
	index[35] = pcount + 7;
}
//正5棱柱
void carvesolid_prism5()
{
}
//正6棱柱
void carvesolid_prism6()
{
}
//圆柱
void carvesolid_cask(
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
	u32 icount = win->info[14];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xe00000 + (icount*2);

	win->info[8] += accuracy*2;
	win->info[9] += accuracy*2;
	win->info[10] += accuracy*2;
	win->info[14] += accuracy*3*2;

	for(j=0;j<accuracy;j++)
	{
		v[0] = ux;
		v[1] = uy;
		v[2] = uz;
		vectornormalize(v);

		t = j*PI/accuracy;
		v[0] *= sine(t);
		v[1] *= sine(t);
		v[2] *= sine(t);
		v[3] = cosine(t);

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

		index[a+0] = pcount + j*2;
		index[a+1] = pcount + ((j+1)%accuracy)*2;
		index[a+2] = pcount + 1 + j*2;

		index[a+3] = pcount + 1 + ((j+1)%accuracy)*2;
		index[a+4] = pcount + ((j+1)%accuracy)*2;
		index[a+5] = pcount + 1 + j*2;
	}
}
//圆柱
void carvesolid_cylinder(
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
	u32 icount = win->info[14];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xe00000 + (icount*2);

	win->info[8] += accuracy*2+2;
	win->info[9] += accuracy*2+2;
	win->info[10] += accuracy*2+2;
	win->info[14] += accuracy*3*2 + accuracy*3*2;

	for(j=0;j<accuracy;j++)
	{
		v[0] = ux;
		v[1] = uy;
		v[2] = uz;
		vectornormalize(v);

		t = j*PI/accuracy;
		v[0] *= sine(t);
		v[1] *= sine(t);
		v[2] *= sine(t);
		v[3] = cosine(t);

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

		index[a+0] = pcount + j*2;
		index[a+1] = pcount + ((j+1)%accuracy)*2;
		index[a+2] = pcount + 1 + j*2;

		index[a+3] = pcount + 1 + ((j+1)%accuracy)*2;
		index[a+4] = pcount + ((j+1)%accuracy)*2;
		index[a+5] = pcount + 1 + j*2;
	}

	a = accuracy*3*2;
	b = accuracy*3*2;

	vertex[a+0] = cx-ux;
	vertex[a+1] = cy-uy;
	vertex[a+2] = cz-uz;
	vertex[a+3] = cx+ux;
	vertex[a+4] = cy+uy;
	vertex[a+5] = cz+uz;

	normal[a+0] = -ux;
	normal[a+1] = -uy;
	normal[a+2] = -uz;
	normal[a+3] = ux;
	normal[a+4] = uy;
	normal[a+5] = uz;

	color[a+0] = rr;
	color[a+1] = gg;
	color[a+2] = bb;
	color[a+3] = rr;
	color[a+4] = gg;
	color[a+5] = bb;

	for(j=0;j<accuracy;j++)
	{
		index[b + (j*6) + 0] = pcount + accuracy*2;
		index[b + (j*6) + 1] = pcount + (j*2);
		index[b + (j*6) + 2] = pcount + ((j+1)%accuracy)*2;

		index[b + (j*6) + 3] = pcount + accuracy*2 + 1;
		index[b + (j*6) + 4] = pcount + 1 + (j*2);
		index[b + (j*6) + 5] = pcount + 1 + ((j+1)%accuracy)*2;
	}
}




//正3棱锥
void carvesolid_pyramid3()
{
}
//正4棱锥
void carvesolid_pyramid4()
{
}
//正5棱锥
void carvesolid_pyramid5()
{
}
//正6棱锥
void carvesolid_pyramid6()
{
}
//圆锥
void carvesolid_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//radius = |rvector|
	float ux, float uy, float uz)	//height = |uvector|
{
}




//正4面体
void carvesolid_tetrahedron()
{
}
//正8面体
void carvesolid_octahedron()
{
}
//正12面体
void cavesolid_dodecahedron()
{
}
//正20面体
void carvesolid_icosahedron()
{
}
//球体
void carvesolid_sphere(
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
	u32 icount = win->info[14];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xe00000 + (icount*2);

	win->info[8] += accuracy*17+2;
	win->info[9] += accuracy*17+2;
	win->info[10] += accuracy*17+2;
	win->info[14] += accuracy*16*6 + accuracy*3*2;

	for(k=0;k<17;k++)
	{
		s = (k-8)*PI/18;
		t = cosine(s);
		temprx = rx*t;
		tempry = ry*t;
		temprz = rz*t;

		t = sine(s);
		tempcx = cx + ux*t;
		tempcy = cy + uy*t;
		tempcz = cz + uz*t;

		for(j=0;j<accuracy;j++)
		{
			v[0] = ux;
			v[1] = uy;
			v[2] = uz;
			vectornormalize(v);

			t = (j-accuracy/2)*PI/accuracy;
			v[0] *= sine(t);
			v[1] *= sine(t);
			v[2] *= sine(t);
			v[3] = cosine(t);

			a = (k*accuracy + j)*3;

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
		}
	}
	for(k=0;k<16;k++)
	{
		a = k*accuracy*6;
		for(j=0;j<accuracy;j++)
		{
			index[a + 6*j + 0] = pcount+(k*accuracy)+j;
			index[a + 6*j + 1] = pcount+(k*accuracy)+(j+1)%accuracy;
			index[a + 6*j + 2] = pcount+(k*accuracy)+accuracy+j;

			index[a + 6*j + 3] = pcount+(k*accuracy)+(j+1)%accuracy;
			index[a + 6*j + 4] = pcount+(k*accuracy)+accuracy+j;
			index[a + 6*j + 5] = pcount+(k*accuracy)+accuracy+(j+1)%accuracy;
		}
	}

	//
	a = accuracy*17*3;
	b = accuracy*16*6;

	vertex[a+0] = cx-ux;
	vertex[a+1] = cy-uy;
	vertex[a+2] = cz-uz;
	vertex[a+3] = cx+ux;
	vertex[a+4] = cy+uy;
	vertex[a+5] = cz+uz;

	normal[a+0] = -ux;
	normal[a+1] = -uy;
	normal[a+2] = -uz;
	normal[a+3] = ux;
	normal[a+4] = uy;
	normal[a+5] = uz;

	color[a+0] = rr;
	color[a+1] = gg;
	color[a+2] = bb;
	color[a+3] = rr;
	color[a+4] = gg;
	color[a+5] = bb;

	for(j=0;j<accuracy;j++)
	{
		index[b + (3*j) +0] = pcount+accuracy*17;
		index[b + (3*j) +1] = pcount+j;
		index[b + (3*j) +2] = pcount+(j+1)%accuracy;

		index[b + (3*j) +(accuracy*3) + 0] = pcount+accuracy*17+1;
		index[b + (3*j) +(accuracy*3) + 1] = pcount+accuracy*16+j;
		index[b + (3*j) +(accuracy*3) + 2] = pcount+accuracy*16+(j+1)%accuracy;
	}
}