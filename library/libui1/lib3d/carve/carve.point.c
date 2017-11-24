#include "actor.h"
#define accuracy 18
#define PI 3.1415926535897932384626433832795028841971693993151




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
void carvepoint_bezier()
{
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
	u32 icount = win->info[12];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xc00000 + (icount*2);

	win->info[8] += accuracy;
	win->info[9] += accuracy;
	win->info[10] += accuracy;
	win->info[12] += accuracy;

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

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, q);

		vertex[a+0] = cx + r[0];
		vertex[a+1] = cy + r[1];
		vertex[a+2] = cz + r[2];

		normal[a+0] = ux;
		normal[a+1] = uy;
		normal[a+2] = uz;

		color[a+0] = rr;
		color[a+1] = gg;
		color[a+2] = bb;

		index[j] = pcount+j;
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
	u32 icount = win->info[12];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xc00000 + (icount*2);

	win->info[8] += accuracy+2;
	win->info[9] += accuracy+2;
	win->info[10] += accuracy+2;
	win->info[12] += accuracy+2;

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

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, q);

		vertex[a+0] = cx + r[0];
		vertex[a+1] = cy + r[1];
		vertex[a+2] = cz + r[2];

		normal[a+0] = ux;
		normal[a+1] = uy;
		normal[a+2] = uz;

		color[a+0] = rr;
		color[a+1] = gg;
		color[a+2] = bb;

		index[j] = pcount+j;
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

	index[accuracy+0] = pcount+accuracy;
	index[accuracy+1] = pcount+accuracy+1;
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
void carvepoint_cask()
{
}
void carvepoint_cylinder(
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
	u32 icount = win->info[12];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xc00000 + (icount*2);

	win->info[8] += accuracy*2;
	win->info[9] += accuracy*2;
	win->info[10] += accuracy*2;
	win->info[12] += accuracy*2;

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

		b = j*2;
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

		index[b+0] = pcount + 2*j + 0;
		index[b+1] = pcount + 2*j + 1;
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

	win->info[8] += 20;
	win->info[9] += 20;
	win->info[10] += 20;
	win->info[12] += 20;

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
		normal[j*3 + 0] = 0.0;
		normal[j*3 + 1] = 0.0;
		normal[j*3 + 2] = 1.0;

		color[j*3 + 0] = rr;
		color[j*3 + 1] = gg;
		color[j*3 + 2] = bb;

		index[j] = pcount+j;
	}
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

	win->info[8] += 12;
	win->info[9] += 12;
	win->info[10] += 12;
	win->info[12] += 12;

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
		normal[j*3 + 0] = 0.0;
		normal[j*3 + 1] = 0.0;
		normal[j*3 + 2] = 1.0;

		color[j*3 + 0] = rr;
		color[j*3 + 1] = gg;
		color[j*3 + 2] = bb;

		index[j] = pcount+j;
	}
}
void carvepoint_sphere(
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
	u32 icount = win->info[12];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index    = buf + 0xc00000 + (icount*2);

	win->info[8] += accuracy*17+2;
	win->info[9] += accuracy*17+2;
	win->info[10] += accuracy*17+2;
	win->info[12] += accuracy*17+2;

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

			t = (j-(accuracy/2))*PI/accuracy;
			q[0] *= sine(t);
			q[1] *= sine(t);
			q[2] *= sine(t);
			q[3] = cosine(t);

			b = k*accuracy + j;
			a = b*3;

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

			index[b] = pcount + (k*accuracy) + j;
		}
	}

	a = accuracy*17*3;
	b = accuracy*17;

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

	index[b+0] = pcount + (accuracy*17) + 0;
	index[b+1] = pcount + (accuracy*17) + 1;
}