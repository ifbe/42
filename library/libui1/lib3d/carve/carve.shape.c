#include "actor.h"
double sine(double);
double cosine(double);
double squareroot(double);




//方块
void carvecubie(struct arena* win,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//width = |rvector|*2
	float fx, float fy, float fz,	//height = |fvector|*2
	float ux, float uy, float uz)	//upper = |uvector|*2
{
	u32 pcount = win->info[0];
	u32 icount = win->info[1];
	void* buf = (void*)(win->buf);

	float* point = buf + (pcount*12);
	u16* index = buf + 0x100000 + (icount*2);

	point[0] = cx - rx;
	point[1] = cy - fy;
	point[2] = cz - uz;

	point[3] = cx + rx;
	point[4] = cy - fy;
	point[5] = cz - uz;

	point[6] = cx - rx;
	point[7] = cy + fy;
	point[8] = cz - uz;

	point[9] = cx + rx;
	point[10] = cy + fy;
	point[11] = cz - uz;

	point[12] = cx - rx;
	point[13] = cy - fy;
	point[14] = cz + uz;

	point[15] = cx + rx;
	point[16] = cy - fy;
	point[17] = cz + uz;

	point[18] = cx - rx;
	point[19] = cy + fy;
	point[20] = cz + uz;

	point[21] = cx + rx;
	point[22] = cy + fy;
	point[23] = cz + uz;

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

	win->info[0] += 8;
	win->info[1] += 36;
}




//圆柱体
void carvecylinder(struct arena* win,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//radius = |rvector|
	float ux, float uy, float uz)	//height = |uvector|
{
}




//圆锥
void carvecone(struct arena* win,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//radius = |rvector|
	float ux, float uy, float uz)	//height = |uvector|
{
}




//球体
void carvesphere(struct arena* win,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz)	//radius = |rvector|
{
	float* buf = (void*)(win->buf);
}