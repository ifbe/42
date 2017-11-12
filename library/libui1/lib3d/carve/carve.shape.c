#include "actor.h"
double sine(double);
double cosine(double);
double squareroot(double);




void carverect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//width = |rvector|*2
	float fx, float fy, float fz)	//height = |fvector|*2
{
	float rr, gg, bb;
	u32 icount = win->info[0];
	u32 pcount = win->info[1];
	u32 ncount = win->info[1];
	u32 ccount = win->info[1];

	void* buf = (void*)(win->buf);
	u16* index = buf + (icount*2);
	float* vertex = buf + 0x100000 + (pcount*12);
	float* normal = buf + 0x200000 + (ncount*12);
	float* color  = buf + 0x300000 + (ccount*12);

	win->info[0] += 6;
	win->info[1] += 4;
	win->info[2] += 4;
	win->info[3] += 4;

	//index
	index[0] = pcount + 0;
	index[1] = pcount + 1;
	index[2] = pcount + 2;

	index[3] = pcount + 1;
	index[4] = pcount + 2;
	index[5] = pcount + 3;

	//vertex
	vertex[0] = cx - rx;
	vertex[1] = cy - fy;
	vertex[2] = 0.0;

	vertex[3] = cx + rx;
	vertex[4] = cy - fy;
	vertex[5] = 0.0;

	vertex[6] = cx - rx;
	vertex[7] = cy + fy;
	vertex[8] = 0.0;

	vertex[9] = cx + rx;
	vertex[10] = cy + fy;
	vertex[11] = 0.0;

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

	//color
	bb = (float)(rgb&0xff) / 256.0;
	gg = (float)((rgb>>8)&0xff) / 256.0;
	rr = (float)((rgb>>16)&0xff) / 256.0;

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
}




//正三棱柱
void carveprism3()
{
}
//正四棱柱
void carveprism4(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//width = |rvector|*2
	float fx, float fy, float fz,	//height = |fvector|*2
	float ux, float uy, float uz)	//upper = |uvector|*2
{
	float rr, gg, bb;
	u32 icount = win->info[0];
	u32 pcount = win->info[1];
	u32 ncount = win->info[1];
	u32 ccount = win->info[1];

	void* buf = (void*)(win->buf);
	u16* index = buf + (icount*2);
	float* vertex = buf + 0x100000 + (pcount*12);
	float* normal = buf + 0x200000 + (ncount*12);
	float* color  = buf + 0x300000 + (ccount*12);

	win->info[0] += 36;
	win->info[1] += 8;
	win->info[2] += 8;
	win->info[3] += 8;

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

	//vertex
	vertex[0] = cx - rx;
	vertex[1] = cy - fy;
	vertex[2] = cz - uz;

	vertex[3] = cx + rx;
	vertex[4] = cy - fy;
	vertex[5] = cz - uz;

	vertex[6] = cx - rx;
	vertex[7] = cy + fy;
	vertex[8] = cz - uz;

	vertex[9] = cx + rx;
	vertex[10] = cy + fy;
	vertex[11] = cz - uz;

	vertex[12] = cx - rx;
	vertex[13] = cy - fy;
	vertex[14] = cz + uz;

	vertex[15] = cx + rx;
	vertex[16] = cy - fy;
	vertex[17] = cz + uz;

	vertex[18] = cx - rx;
	vertex[19] = cy + fy;
	vertex[20] = cz + uz;

	vertex[21] = cx + rx;
	vertex[22] = cy + fy;
	vertex[23] = cz + uz;

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

	//color
	bb = (float)(rgb&0xff) / 256.0;
	gg = (float)((rgb>>8)&0xff) / 256.0;
	rr = (float)((rgb>>16)&0xff) / 256.0;

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