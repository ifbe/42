#include "actor.h"
double sine(double);
double cosine(double);
double squareroot(double);




//正三棱柱
void carveprism3()
{
}
//正四棱柱
void carveprism4(
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
//正五棱柱
void carveprism5()
{
}
//正六棱柱
void carveprism6()
{
}
//圆柱
void carvecylinder(
	struct arena* win,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//radius = |rvector|
	float ux, float uy, float uz)	//height = |uvector|
{
}




//正三棱锥
void carvepyramid3()
{
}
//正四棱锥
void carvepyramid4()
{
}
//正四棱锥
void carvepyramid5()
{
}
//正四棱锥
void carvepyramid6()
{
}
//圆锥
void carvecone(
	struct arena* win,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//radius = |rvector|
	float ux, float uy, float uz)	//height = |uvector|
{
}




//正四面体
void carvetetrahedron()
{
}
//正八面体
void carveoctahedron()
{
}
//正十二面体
void cavedodecahedron()
{
}
//正二十面体
void carveicosahedron()
{
}
//球体
void carvesphere(
	struct arena* win,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz)	//radius = |rvector|
{
	float* buf = (void*)(win->buf);
}