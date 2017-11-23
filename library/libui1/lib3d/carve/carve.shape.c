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
	int a,b,j,k;
	float s,t;
	float q[4];
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

	win->info[8] += accuracy+2;
	win->info[9] += accuracy+2;
	win->info[10] += accuracy+2;
	win->info[14] += accuracy*3*2;

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

		a = j*3;
		b = j*6;

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, q);

		vertex[a+0] = cx + r[0];
		vertex[a+1] = cy + r[1];
		vertex[a+2] = cz + r[2];

		normal[a+0] = vertex[a+0] - cx;
		normal[a+1] = vertex[a+1] - cy;
		normal[a+2] = vertex[a+2] - cz;

		color[a+0] = rr;
		color[a+1] = gg;
		color[a+2] = bb;

		//bottom
		index[b+0] = pcount+accuracy;
		index[b+1] = pcount+j;
		index[b+2] = pcount+(j+1)%accuracy;

		//upper
		index[b+3] = pcount+accuracy+1;
		index[b+4] = pcount+j;
		index[b+5] = pcount+(j+1)%accuracy;
	}

	a = accuracy*3;

	vertex[a+0] = cx;
	vertex[a+1] = cy;
	vertex[a+2] = cz;
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
	float r[3];
	float f[3];
	float u[3];

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

	//normal
	r[0] = rx;
	r[1] = ry;
	r[2] = rz;
	vectornormalize(r);
	rx = r[0];
	ry = r[1];
	rz = r[2];

	f[0] = fx;
	f[1] = fy;
	f[2] = fz;
	vectornormalize(f);
	fx = f[0];
	fy = f[1];
	fz = f[2];

	u[0] = ux;
	u[1] = uy;
	u[2] = uz;
	vectornormalize(u);
	ux = u[0]*2;
	uy = u[1]*2;
	uz = u[2]*2;

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
	int a,b,j,k;
	float s,t;
	float q[4];
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
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		vectornormalize(q);

		t = j*PI/accuracy;
		q[0] *= sine(t);
		q[1] *= sine(t);
		q[2] *= sine(t);
		q[3] = cosine(t);

		a = j*6;

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, q);

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
void carvesolid_cylinder(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int a,b,j,k;
	float s,t;
	float q[4];
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
	win->info[14] += accuracy*3*4;

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

		a = j*6;

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, q);

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

	win->info[8] += 20;
	win->info[9] += 20;
	win->info[10] += 20;
	win->info[14] += 12*3*3;

	//(+-1, +-1, +-1)
	vertex[ 0] = cx-rx-fx-ux;
	vertex[ 1] = cy-ry-fy-uy;
	vertex[ 2] = cz-rz-fz-uz;

	vertex[ 3] = cx+rx-fx-ux;
	vertex[ 4] = cy+ry-fy-uy;
	vertex[ 5] = cz+rz-fz-uz;

	vertex[ 6] = cx-rx+fx-ux;
	vertex[ 7] = cy-ry+fy-uy;
	vertex[ 8] = cz-rz+fz-uz;

	vertex[ 9] = cx+rx+fx-ux;
	vertex[10] = cy+ry+fy-uy;
	vertex[11] = cz+rz+fz-uz;

	vertex[12] = cx-rx-fx+ux;
	vertex[13] = cy-ry-fy+uy;
	vertex[14] = cz-rz-fz+uz;

	vertex[15] = cx+rx-fx+ux;
	vertex[16] = cy+ry-fy+uy;
	vertex[17] = cz+rz-fz+uz;

	vertex[18] = cx-rx+fx+ux;
	vertex[19] = cy-ry+fy+uy;
	vertex[20] = cz-rz+fz+uz;

	vertex[21] = cx+rx+fx+ux;
	vertex[22] = cy+ry+fy+uy;
	vertex[23] = cz+rz+fz+uz;

	//(0, +-a, +-b)
	vertex[24] = cx - a*fx - b*ux;
	vertex[25] = cy - a*fy - b*uy;
	vertex[26] = cz - a*fz - b*uz;

	vertex[27] = cx + a*fx - b*ux;
	vertex[28] = cy + a*fy - b*uy;
	vertex[29] = cz + a*fz - b*uz;

	vertex[30] = cx - a*fx + b*ux;
	vertex[31] = cy - a*fy + b*uy;
	vertex[32] = cz - a*fz + b*uz;

	vertex[33] = cx + a*fx + b*ux;
	vertex[34] = cy + a*fy + b*uy;
	vertex[35] = cz + a*fz + b*uz;

	//(+-b, 0, +-a)
	vertex[36] = cx - b*rx - a*ux;
	vertex[37] = cy - b*ry - a*uy;
	vertex[38] = cz - b*rz - a*uz;

	vertex[39] = cx + b*rx - a*ux;
	vertex[40] = cy + b*ry - a*uy;
	vertex[41] = cz + b*rz - a*uz;

	vertex[42] = cx - b*rx + a*ux;
	vertex[43] = cy - b*ry + a*uy;
	vertex[44] = cz - b*rz + a*uz;

	vertex[45] = cx + b*rx + a*ux;
	vertex[46] = cy + b*ry + a*uy;
	vertex[47] = cz + b*rz + a*uz;

	//(+-a, +-b, 0)
	vertex[48] = cx - a*rx - b*fx;
	vertex[49] = cy - a*ry - b*fy;
	vertex[50] = cz - a*rz - b*fz;

	vertex[51] = cx + a*rx - b*fx;
	vertex[52] = cy + a*ry - b*fy;
	vertex[53] = cz + a*rz - b*fz;

	vertex[54] = cx - a*rx + b*fx;
	vertex[55] = cy - a*ry + b*fy;
	vertex[56] = cz - a*rz + b*fz;

	vertex[57] = cx + a*rx + b*fx;
	vertex[58] = cy + a*ry + b*fy;
	vertex[59] = cz + a*rz + b*fz;

	for(j=0;j<20;j++)
	{
		normal[j*3 + 0] = vertex[j*3 + 0] - cx;
		normal[j*3 + 1] = vertex[j*3 + 1] - cy;
		normal[j*3 + 2] = vertex[j*3 + 2] - cz;

		color[j*3 + 0] = rr;
		color[j*3 + 1] = gg;
		color[j*3 + 2] = bb;
	}

	//front
	index[ 0] = pcount+0;
	index[ 1] = pcount+1;
	index[ 2] = pcount+8;
	index[ 3] = pcount+0;
	index[ 4] = pcount+1;
	index[ 5] = pcount+12;
	index[ 6] = pcount+12;
	index[ 7] = pcount+13;
	index[ 8] = pcount+1;

	index[ 9] = pcount+4;
	index[10] = pcount+5;
	index[11] = pcount+10;
	index[12] = pcount+4;
	index[13] = pcount+5;
	index[14] = pcount+14;
	index[15] = pcount+14;
	index[16] = pcount+15;
	index[17] = pcount+5;

	//back
	index[18] = pcount+2;
	index[19] = pcount+3;
	index[20] = pcount+9;
	index[21] = pcount+2;
	index[22] = pcount+3;
	index[23] = pcount+12;
	index[24] = pcount+12;
	index[25] = pcount+13;
	index[26] = pcount+3;

	index[27] = pcount+6;
	index[28] = pcount+7;
	index[29] = pcount+11;
	index[30] = pcount+6;
	index[31] = pcount+7;
	index[32] = pcount+14;
	index[33] = pcount+14;
	index[34] = pcount+15;
	index[35] = pcount+7;

	//bottom
	index[36] = pcount+0;
	index[37] = pcount+2;
	index[38] = pcount+12;
	index[39] = pcount+0;
	index[40] = pcount+2;
	index[41] = pcount+16;
	index[42] = pcount+16;
	index[43] = pcount+18;
	index[44] = pcount+2;

	index[45] = pcount+1;
	index[46] = pcount+3;
	index[47] = pcount+13;
	index[48] = pcount+1;
	index[49] = pcount+3;
	index[50] = pcount+17;
	index[51] = pcount+17;
	index[52] = pcount+19;
	index[53] = pcount+3;

	//upper
	index[54] = pcount+4;
	index[55] = pcount+6;
	index[56] = pcount+14;
	index[57] = pcount+4;
	index[58] = pcount+6;
	index[59] = pcount+16;
	index[60] = pcount+16;
	index[61] = pcount+18;
	index[62] = pcount+6;

	index[63] = pcount+5;
	index[64] = pcount+7;
	index[65] = pcount+15;
	index[66] = pcount+5;
	index[67] = pcount+7;
	index[68] = pcount+17;
	index[69] = pcount+17;
	index[70] = pcount+19;
	index[71] = pcount+7;

	//left
	index[72] = pcount+0;
	index[73] = pcount+4;
	index[74] = pcount+16;
	index[75] = pcount+0;
	index[76] = pcount+4;
	index[77] = pcount+8;
	index[78] = pcount+8;
	index[79] = pcount+10;
	index[80] = pcount+4;

	index[81] = pcount+2;
	index[82] = pcount+6;
	index[83] = pcount+18;
	index[84] = pcount+2;
	index[85] = pcount+6;
	index[86] = pcount+9;
	index[87] = pcount+9;
	index[88] = pcount+11;
	index[89] = pcount+6;

	//right
	index[90] = pcount+1;
	index[91] = pcount+5;
	index[92] = pcount+17;
	index[93] = pcount+1;
	index[94] = pcount+5;
	index[95] = pcount+8;
	index[96] = pcount+8;
	index[97] = pcount+10;
	index[98] = pcount+5;

	index[99] = pcount+3;
	index[100] = pcount+7;
	index[101] = pcount+19;
	index[102] = pcount+3;
	index[103] = pcount+7;
	index[104] = pcount+9;
	index[105] = pcount+9;
	index[106] = pcount+11;
	index[107] = pcount+7;
}
void carvesolid_icosahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz)
{
	int j;
	float m = 0.52573111211913360602566908484788;
	float n = 0.85065080835203993218154049706301;

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

	win->info[8] += 12;
	win->info[9] += 12;
	win->info[10] += 12;
	win->info[14] += 60;

	//(+-m, 0, +-n)
	vertex[ 0] = cx - m;
	vertex[ 1] = cy;
	vertex[ 2] = cz - n;

	vertex[ 3] = cx + m;
	vertex[ 4] = cy;
	vertex[ 5] = cz - n;

	vertex[ 6] = cx - m;
	vertex[ 7] = cy;
	vertex[ 8] = cz + n;

	vertex[ 9] = cx + m;
	vertex[10] = cy;
	vertex[11] = cz + n;

	//(0, +-n, +-m)
	vertex[12] = cx;
	vertex[13] = cy - n;
	vertex[14] = cz - m;

	vertex[15] = cx;
	vertex[16] = cy + n;
	vertex[17] = cz - m;

	vertex[18] = cx;
	vertex[19] = cy - n;
	vertex[20] = cz + m;

	vertex[21] = cx;
	vertex[22] = cy + n;
	vertex[23] = cz + m;

	//(+-n, +-m, 0)
	vertex[24] = cx - n;
	vertex[25] = cy - m;
	vertex[26] = cz;

	vertex[27] = cx + n;
	vertex[28] = cy - m;
	vertex[29] = cz;

	vertex[30] = cx - n;
	vertex[31] = cy + m;
	vertex[32] = cz;

	vertex[33] = cx + n;
	vertex[34] = cy + m;
	vertex[35] = cz;

	for(j=0;j<12;j++)
	{
		normal[j*3 + 0] = vertex[j*3 + 0] - cx;
		normal[j*3 + 1] = vertex[j*3 + 1] - cy;
		normal[j*3 + 2] = vertex[j*3 + 2] - cz;

		color[j*3 + 0] = rr;
		color[j*3 + 1] = gg;
		color[j*3 + 2] = bb;
	}

	index[ 0] = pcount+0;
	index[ 1] = pcount+1;
	index[ 2] = pcount+5;

	index[ 3] = pcount+0;
	index[ 4] = pcount+5;
	index[ 5] = pcount+10;

	index[ 6] = pcount+0;
	index[ 7] = pcount+10;
	index[ 8] = pcount+8;

	index[ 9] = pcount+0;
	index[10] = pcount+8;
	index[11] = pcount+4;

	index[12] = pcount+0;
	index[13] = pcount+1;
	index[14] = pcount+4;

	index[15] = pcount+3;
	index[16] = pcount+2;
	index[17] = pcount+6;

	index[18] = pcount+3;
	index[19] = pcount+6;
	index[20] = pcount+9;

	index[21] = pcount+3;
	index[22] = pcount+9;
	index[23] = pcount+11;

	index[24] = pcount+3;
	index[25] = pcount+11;
	index[26] = pcount+7;

	index[27] = pcount+3;
	index[28] = pcount+2;
	index[29] = pcount+7;

	index[30] = pcount+1;
	index[31] = pcount+11;
	index[32] = pcount+9;

	index[33] = pcount+5;
	index[34] = pcount+7;
	index[35] = pcount+11;

	index[36] = pcount+10;
	index[37] = pcount+2;
	index[38] = pcount+7;

	index[39] = pcount+8;
	index[40] = pcount+6;
	index[41] = pcount+2;

	index[42] = pcount+4;
	index[43] = pcount+9;
	index[44] = pcount+6;

	index[45] = pcount+1;
	index[46] = pcount+11;
	index[47] = pcount+5;

	index[48] = pcount+5;
	index[49] = pcount+7;
	index[50] = pcount+10;

	index[51] = pcount+10;
	index[52] = pcount+2;
	index[53] = pcount+8;

	index[54] = pcount+8;
	index[55] = pcount+6;
	index[56] = pcount+4;

	index[57] = pcount+4;
	index[58] = pcount+9;
	index[59] = pcount+1;
}
void carvesolid_sphere(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int a,b,j,k;
	float s,t;
	float q[4];
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
			q[0] = ux;
			q[1] = uy;
			q[2] = uz;
			vectornormalize(q);

			t = (j-accuracy/2)*PI/accuracy;
			q[0] *= sine(t);
			q[1] *= sine(t);
			q[2] *= sine(t);
			q[3] = cosine(t);

			a = (k*accuracy + j)*3;

			vertex[a+0] = temprx;
			vertex[a+1] = tempry;
			vertex[a+2] = temprz;
			quaternionrotate(&vertex[a], q);

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