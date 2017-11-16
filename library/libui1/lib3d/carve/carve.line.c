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




void carveline(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[13];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xd00000 + (icount*2);

	win->info[8] += 2;
	win->info[9] += 2;
	win->info[10] += 2;
	win->info[13] += 2;

	color[0] = rr;
	color[1] = gg;
	color[2] = bb;
	color[3] = rr;
	color[4] = gg;
	color[5] = bb;

	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;
	normal[3] = 0.0;
	normal[4] = 0.0;
	normal[5] = 1.0;

	vertex[0] = x1;
	vertex[1] = y1;
	vertex[2] = z1;
	vertex[3] = x2;
	vertex[4] = y2;
	vertex[5] = z2;

	index[0] = pcount;
	index[1] = pcount+1;
}
void carvetriangle_frame(
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
	u32 icount = win->info[13];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xd00000 + (icount*2);

	win->info[8] += 3;
	win->info[9] += 3;
	win->info[10] += 3;
	win->info[13] += 6;

	color[0] = rr;
	color[1] = gg;
	color[2] = bb;
	color[3] = rr;
	color[4] = gg;
	color[5] = bb;
	color[6] = rr;
	color[7] = gg;
	color[8] = bb;

	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;
	normal[3] = 0.0;
	normal[4] = 0.0;
	normal[5] = 1.0;
	normal[6] = 0.0;
	normal[7] = 0.0;
	normal[8] = 1.0;

	vertex[0] = x1;
	vertex[1] = y1;
	vertex[2] = z1;
	vertex[3] = x2;
	vertex[4] = y2;
	vertex[5] = z2;
	vertex[3] = x3;
	vertex[4] = y3;
	vertex[5] = z3;

	index[0] = pcount;
	index[1] = pcount+1;
	index[2] = pcount+1;
	index[3] = pcount+2;
	index[4] = pcount;
	index[5] = pcount+2;
}
void carverect_frame(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[13];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xd00000 + (icount*2);

	win->info[8] += 4;
	win->info[9] += 4;
	win->info[10] += 4;
	win->info[13] += 8;

	color[ 0] = rr;
	color[ 1] = gg;
	color[ 2] = bb;
	color[ 3] = rr;
	color[ 4] = gg;
	color[ 5] = bb;
	color[ 6] = rr;
	color[ 7] = gg;
	color[ 8] = bb;
	color[ 9] = rr;
	color[10] = gg;
	color[11] = bb;

	normal[ 0] = 0.0;
	normal[ 1] = 0.0;
	normal[ 2] = 1.0;
	normal[ 3] = 0.0;
	normal[ 4] = 0.0;
	normal[ 5] = 1.0;
	normal[ 6] = 0.0;
	normal[ 7] = 0.0;
	normal[ 8] = 1.0;
	normal[ 9] = 0.0;
	normal[10] = 0.0;
	normal[11] = 1.0;

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

	index[0] = pcount;
	index[1] = pcount+1;
	index[2] = pcount+1;
	index[3] = pcount+3;
	index[4] = pcount+3;
	index[5] = pcount+2;
	index[6] = pcount+0;
	index[7] = pcount+2;
}
void carveprism4_frame(
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
	u32 icount = win->info[13];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xd00000 + (icount*2);

	win->info[8] += 8;
	win->info[9] += 8;
	win->info[10] += 8;
	win->info[13] += 24;

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
	normal[0] = -1.0;
	normal[1] = -1.0;
	normal[2] = -1.0;

	normal[3] = 1.0;
	normal[4] = -1.0;
	normal[5] = -1.0;

	normal[6] = -1.0;
	normal[7] = 1.0;
	normal[8] = -1.0;

	normal[9] = 1.0;
	normal[10] = 1.0;
	normal[11] = -1.0;

	normal[12] = -1.0;
	normal[13] = -1.0;
	normal[14] = 1.0;

	normal[15] = 1.0;
	normal[16] = -1.0;
	normal[17] = 1.0;

	normal[18] = -1.0;
	normal[19] = 1.0;
	normal[20] = 1.0;

	normal[21] = 1.0;
	normal[22] = 1.0;
	normal[23] = 1.0;

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

	index[ 0] = pcount;
	index[ 1] = pcount+1;
	index[ 2] = pcount+0;
	index[ 3] = pcount+2;
	index[ 4] = pcount+3;
	index[ 5] = pcount+1;
	index[ 6] = pcount+3;
	index[ 7] = pcount+2;

	index[ 8] = pcount;
	index[ 9] = pcount+4;
	index[10] = pcount+1;
	index[11] = pcount+5;
	index[12] = pcount+2;
	index[13] = pcount+6;
	index[14] = pcount+3;
	index[15] = pcount+7;

	index[16] = pcount+4;
	index[17] = pcount+5;
	index[18] = pcount+4;
	index[19] = pcount+6;
	index[20] = pcount+7;
	index[21] = pcount+5;
	index[22] = pcount+7;
	index[23] = pcount+6;
}
void carvecircle_frame(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int j;
	float t;
	float v[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[13];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xd00000 + (icount*2);

	win->info[8] += 36;
	win->info[9] += 36;
	win->info[10] += 36;
	win->info[13] += 72;

	for(j=0;j<36;j++)
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

		vertex[(j*3)+0] = rx;
		vertex[(j*3)+1] = ry;
		vertex[(j*3)+2] = rz;
		quaternionrotate(&vertex[j*3], v);

		vertex[(j*3)+0] += cx;
		vertex[(j*3)+1] += cy;
		vertex[(j*3)+2] += cz;

		normal[(j*3)+0] = 0.0;
		normal[(j*3)+1] = 0.0;
		normal[(j*3)+2] = 1.0;

		color[(j*3)+0] = rr;
		color[(j*3)+1] = gg;
		color[(j*3)+2] = bb;

		index[(j*2)+0] = pcount+j;
		index[(j*2)+1] = pcount+j+1;
	}
	index[71] = pcount;
}
void carvesphere_frame(
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
	u32 icount = win->info[13];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xd00000 + (icount*2);

	win->info[8] += 36*17;
	win->info[9] += 36*17;
	win->info[10] += 36*17;
	win->info[13] += 72*17+72*32;

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

			a = ((k+8)*36 + (j+18))*3;
			b = ((k+8)*36 + (j+18))*2;

			vertex[a+0] = temprx;
			vertex[a+1] = tempry;
			vertex[a+2] = temprz;
			quaternionrotate(&vertex[a], v);

			vertex[a+0] += tempcx;
			vertex[a+1] += tempcy;
			vertex[a+2] += tempcz;

			normal[a+0] = 0.0;
			normal[a+1] = 0.0;
			normal[a+2] = 1.0;

			color[a+0] = rr;
			color[a+1] = gg;
			color[a+2] = bb;

			index[b+0] = pcount+((k+8)*36)+(j+18);
			index[b+1] = pcount+((k+8)*36)+(j+18)+1;
		}
		index[b+1] = pcount+((k+8)*36);
	}
	for(k=0;k<36;k++)
	{
		a = 72*17 + k*32;
		for(j=0;j<16;j++)
		{
			index[a + 2*j + 0] = pcount+k+(j*36);
			index[a + 2*j + 1] = pcount+k+(j*36)+36;
		}
	}
}




void carvebezier(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float xc, float yc, float zc)
{
}




void carveaxis(struct arena* win)
{
	carveline(win, 0xff0000,
		0.0, 0.0, 0.0,
		10.0, 0.0, 0.0);
	carveline(win, 0xff00,
		0.0, 0.0, 0.0,
		0.0, 10.0, 0.0);
	carveline(win, 0xff,
		0.0, 0.0, 0.0,
		0.0, 0.0, 10.0);
}
void select_3d(struct arena* win, struct style* sty)
{
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;
	float d = (w+h)/2;

	carveprism4_frame(
		win, 0xff0000,
		cx, cy, d/2,
		w/2, 0.0, 0.0,
		0.0, h/2, 0.0,
		0.0, 0.0, d/2
	);
}