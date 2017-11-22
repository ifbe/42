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




void carvesolid_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
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
	u16* index =    buf + 0xe00000 + (icount*2);

	win->info[8] += 3;
	win->info[9] += 3;
	win->info[10] += 3;
	win->info[14] += 3;

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

	//normal
	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;

	normal[3] = 0.0;
	normal[4] = 0.0;
	normal[5] = 1.0;

	normal[6] = 0.0;
	normal[7] = 0.0;
	normal[8] = 1.0;

	//vertex
	vertex[0] = x1;
	vertex[1] = y1;
	vertex[2] = z1;

	vertex[3] = x2;
	vertex[4] = y2;
	vertex[5] = z2;

	vertex[6] = x3;
	vertex[7] = y3;
	vertex[8] = z3;

	//index
	index[0] = pcount + 0;
	index[1] = pcount + 1;
	index[2] = pcount + 2;
}
void carvesolid_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//width = |rvector|*2
	float fx, float fy, float fz)	//height = |fvector|*2
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
	u16* index =    buf + 0xe00000 + (icount*2);

	win->info[8] += 4;
	win->info[9] += 4;
	win->info[10] += 4;
	win->info[14] += 6;

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

	//normal
	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;

	normal[3] = 0.0;
	normal[4] = 0.0;
	normal[5] = 1.0;

	normal[6] = 0.0;
	normal[7] = 0.0;
	normal[8] = 1.0;

	normal[9] = 0.0;
	normal[10] = 0.0;
	normal[11] = 1.0;

	//vertex
	vertex[ 0] = cx - rx - fx;
	vertex[ 1] = cy - ry - fy;
	vertex[ 2] = cz - rz - fz;

	vertex[ 3] = cx + rx - fx;
	vertex[ 4] = cy + ry - fy;
	vertex[ 5] = cz + rz - fz;

	vertex[ 6] = cx - rx + fx;
	vertex[ 7] = cy - ry + fy;
	vertex[ 8] = cz - rz + fz;

	vertex[ 9] = cx + rx + fx;
	vertex[10] = cy + ry + fy;
	vertex[11] = cz + rz + fz;

	//index
	index[0] = pcount + 0;
	index[1] = pcount + 1;
	index[2] = pcount + 2;

	index[3] = pcount + 1;
	index[4] = pcount + 2;
	index[5] = pcount + 3;
}




void carvesolid_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int j;
	float s,t;
	float q[4];

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
	u16* index =    buf + 0xe00000 + (icount*2);

	win->info[8] += accuracy+1;
	win->info[9] += accuracy+1;
	win->info[10] += accuracy+1;
	win->info[14] += accuracy*3;

	for(j=0;j<accuracy;j++)
	{
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		vectornormalize(q);

		t = j*PI/accuracy;
		q[0] *= sine(t);
		q[1] *= sine(t);
		q[2] *= sine(t);
		q[3] = cosine(t);

		vertex[(j*3)+0] = rx;
		vertex[(j*3)+1] = ry;
		vertex[(j*3)+2] = rz;
		quaternionrotate(&vertex[j*3], q);

		vertex[(j*3)+0] += cx;
		vertex[(j*3)+1] += cy;
		vertex[(j*3)+2] += cz;

		normal[(j*3)+0] = ux;
		normal[(j*3)+1] = uy;
		normal[(j*3)+2] = uz;

		color[(j*3)+0] = rr;
		color[(j*3)+1] = gg;
		color[(j*3)+2] = bb;

		index[(j*3)+0] = pcount + accuracy;
		index[(j*3)+1] = pcount + j;
		index[(j*3)+2] = pcount + (j+1)%accuracy;
	}
	vertex[accuracy*3+0] = cx;
	vertex[accuracy*3+1] = cy;
	vertex[accuracy*3+2] = cz;

	normal[accuracy*3+0] = ux;
	normal[accuracy*3+1] = uy;
	normal[accuracy*3+2] = uz;

	normal[accuracy*3+0] = rr;
	normal[accuracy*3+1] = gg;
	normal[accuracy*3+2] = bb;
}