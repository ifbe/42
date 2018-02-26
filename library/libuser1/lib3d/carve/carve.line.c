#include "actor.h"
#define accuracy 24
#define PI 3.1415926535897932384626433832795028841971693993151
struct texandobj
{
	GLuint obj;
	GLuint len;
	u8* buf;
};
struct eachone
{
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint tex0;
	GLuint tex1;
	GLfloat light0vertex[3];
	GLfloat light0color[3];
	GLfloat light1vertex[3];
	GLfloat light1color[3];
	GLfloat modmat[4][4];
};




void carveline(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += 2;
	win->normalcount  += 2;
	win->colorcount   += 2;
	win->texturecount += 2;
	win->linecount    += 2;

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
void carveline_bezier(
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

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += accuracy+1;
	win->normalcount  += accuracy+1;
	win->colorcount   += accuracy+1;
	win->texturecount += accuracy+1;
	win->linecount    += accuracy*2;

	for(j=0;j<=accuracy;j++)
	{
		t = (float)j / accuracy;

		vertex[3*j+0] = (1.0-t)*(1.0-t)*x1 + 2*t*(1.0-t)*xc + t*t*x2;
		vertex[3*j+1] = (1.0-t)*(1.0-t)*y1 + 2*t*(1.0-t)*yc + t*t*y2;
		vertex[3*j+2] = (1.0-t)*(1.0-t)*z1 + 2*t*(1.0-t)*zc + t*t*z2;

		normal[3*j+0] = 0.0;
		normal[3*j+1] = 0.0;
		normal[3*j+2] = 1.0;

		color[3*j+0] = rr;
		color[3*j+1] = gg;
		color[3*j+2] = bb;

		if(j >= accuracy)break;
		index[2*j+0] = pcount + j;
		index[2*j+1] = pcount + j+1;
	}
}
void carveline_special(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz,
	float sa, float da)
{
	int j;
	float s,t;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += accuracy+1;
	win->normalcount  += accuracy+1;
	win->colorcount   += accuracy+1;
	win->texturecount += accuracy+1;
	win->linecount    += accuracy*2;

	for(j=0;j<=accuracy;j++)
	{
		s = j * da / accuracy;
		t = j * rx / accuracy;

		vertex[j*3 + 0] = t * cosine(sa+s);
		vertex[j*3 + 1] = t * sine(sa+s);
		vertex[j*3 + 2] = uz * j / accuracy;

		normal[3*j+0] = 0.0;
		normal[3*j+1] = 0.0;
		normal[3*j+2] = 1.0;

		color[3*j+0] = rr;
		color[3*j+1] = gg;
		color[3*j+2] = bb;

		if(j >= accuracy)break;
		index[2*j+0] = pcount + j;
		index[2*j+1] = pcount + j+1;
	}
}




void carveline_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += 3;
	win->normalcount  += 3;
	win->colorcount   += 3;
	win->texturecount += 3;
	win->linecount    += 6;

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
	vertex[6] = x3;
	vertex[7] = y3;
	vertex[8] = z3;

	index[0] = pcount;
	index[1] = pcount+1;
	index[2] = pcount+1;
	index[3] = pcount+2;
	index[4] = pcount;
	index[5] = pcount+2;
}
void carveline_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += 4;
	win->normalcount  += 4;
	win->colorcount   += 4;
	win->texturecount += 4;
	win->linecount    += 8;

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
void carveline_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int j;
	float t;
	float q[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += accuracy;
	win->normalcount  += accuracy;
	win->colorcount   += accuracy;
	win->texturecount += accuracy;
	win->linecount    += accuracy*2;

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

		normal[(j*3)+0] = 0.0;
		normal[(j*3)+1] = 0.0;
		normal[(j*3)+2] = 1.0;

		color[(j*3)+0] = rr;
		color[(j*3)+1] = gg;
		color[(j*3)+2] = bb;

		index[(j*2)+0] = pcount+j;
		index[(j*2)+1] = pcount+(j+1)%accuracy;
	}
}




void carveline_pyramid3()
{
}
void carveline_pyramid4()
{
}
void carveline_pyramid5()
{
}
void carveline_pyramid6()
{
}
void carveline_cone(
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

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += accuracy+2;
	win->normalcount  += accuracy+2;
	win->colorcount   += accuracy+2;
	win->texturecount += accuracy+2;
	win->linecount    += accuracy*2*3;

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

		//upper
		index[b+2] = pcount+accuracy+1;
		index[b+3] = pcount+j;

		//curve
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




void carveline_prism3()
{
}
void carveline_prism4(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += 8;
	win->normalcount  += 8;
	win->colorcount   += 8;
	win->texturecount += 8;
	win->linecount    += 24;

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
void carveline_prism5()
{
}
void carveline_prism6()
{
}
void carveline_cask()
{
}
void carveline_cylinder(
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

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += accuracy*2;
	win->normalcount  += accuracy*2;
	win->colorcount   += accuracy*2;
	win->texturecount += accuracy*2;
	win->linecount    += accuracy*6;

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

		index[a+4] = pcount + j*2;
		index[a+5] = pcount + 1 + j*2;
	}
}




void carveline_tetrahedron()
{
}
void carveline_octahedron()
{
}
void carveline_dodecahedron(
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

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += 20;
	win->normalcount  += 20;
	win->colorcount   += 20;
	win->texturecount += 20;
	win->linecount    += 60;

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
	}

	//front
	index[ 0] = pcount+0;
	index[ 1] = pcount+8;
	index[ 2] = pcount+1;
	index[ 3] = pcount+8;
	index[ 4] = pcount+8;
	index[ 5] = pcount+10;
	index[ 6] = pcount+4;
	index[ 7] = pcount+10;
	index[ 8] = pcount+5;
	index[ 9] = pcount+10;

	//back
	index[10] = pcount+2;
	index[11] = pcount+9;
	index[12] = pcount+3;
	index[13] = pcount+9;
	index[14] = pcount+9;
	index[15] = pcount+11;
	index[16] = pcount+6;
	index[17] = pcount+11;
	index[18] = pcount+7;
	index[19] = pcount+11;

	//bottom
	index[20] = pcount+0;
	index[21] = pcount+12;
	index[22] = pcount+2;
	index[23] = pcount+12;
	index[24] = pcount+12;
	index[25] = pcount+13;
	index[26] = pcount+1;
	index[27] = pcount+13;
	index[28] = pcount+3;
	index[29] = pcount+13;

	//upper
	index[30] = pcount+4;
	index[31] = pcount+14;
	index[32] = pcount+6;
	index[33] = pcount+14;
	index[34] = pcount+14;
	index[35] = pcount+15;
	index[36] = pcount+5;
	index[37] = pcount+15;
	index[38] = pcount+7;
	index[39] = pcount+15;

	//left
	index[40] = pcount+0;
	index[41] = pcount+16;
	index[42] = pcount+4;
	index[43] = pcount+16;
	index[44] = pcount+16;
	index[45] = pcount+18;
	index[46] = pcount+2;
	index[47] = pcount+18;
	index[48] = pcount+6;
	index[49] = pcount+18;

	//right
	index[50] = pcount+1;
	index[51] = pcount+17;
	index[52] = pcount+5;
	index[53] = pcount+17;
	index[54] = pcount+17;
	index[55] = pcount+19;
	index[56] = pcount+3;
	index[57] = pcount+19;
	index[58] = pcount+7;
	index[59] = pcount+19;
}
void carveline_icosahedron(
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

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += 12;
	win->normalcount  += 12;
	win->colorcount   += 12;
	win->texturecount += 12;
	win->linecount    += 60;

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
	}

	index[ 0] = pcount+2;
	index[ 1] = pcount+3;
	index[ 2] = pcount+3;
	index[ 3] = pcount+7;
	index[ 4] = pcount+2;
	index[ 5] = pcount+7;
	index[ 6] = pcount+7;
	index[ 7] = pcount+11;
	index[ 8] = pcount+3;
	index[ 9] = pcount+11;

	index[10] = pcount+3;
	index[11] = pcount+6;
	index[12] = pcount+3;
	index[13] = pcount+9;
	index[14] = pcount+6;
	index[15] = pcount+9;
	index[16] = pcount+6;
	index[17] = pcount+2;
	index[18] = pcount+9;
	index[19] = pcount+11;

	index[20] = pcount+0;
	index[21] = pcount+1;
	index[22] = pcount+0;
	index[23] = pcount+4;
	index[24] = pcount+0;
	index[25] = pcount+5;

	index[26] = pcount+0;
	index[27] = pcount+8;
	index[28] = pcount+0;
	index[29] = pcount+10;
	index[30] = pcount+1;
	index[31] = pcount+4;

	index[32] = pcount+4;
	index[33] = pcount+8;
	index[34] = pcount+1;
	index[35] = pcount+5;
	index[36] = pcount+5;
	index[37] = pcount+10;
	index[38] = pcount+8;
	index[39] = pcount+10;

	index[40] = pcount+1;
	index[41] = pcount+11;
	index[42] = pcount+5;
	index[43] = pcount+7;
	index[44] = pcount+10;
	index[45] = pcount+2;
	index[46] = pcount+8;
	index[47] = pcount+6;
	index[48] = pcount+4;
	index[49] = pcount+9;
	index[50] = pcount+1;
	index[51] = pcount+9;
	index[52] = pcount+5;
	index[53] = pcount+11;
	index[54] = pcount+10;
	index[55] = pcount+7;
	index[56] = pcount+8;
	index[57] = pcount+2;
	index[58] = pcount+4;
	index[59] = pcount+6;
}
void carveline_sphere(
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

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->linecount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*12);
	u16* index     = buf + 0xa00000 + (icount*2);

	win->vertexcount  += accuracy*17+2;
	win->normalcount  += accuracy*17+2;
	win->colorcount   += accuracy*17+2;
	win->texturecount += accuracy*17+2;
	win->linecount    += accuracy*17*2+accuracy*32+accuracy*4;

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

			t = (j-(accuracy/2))*PI/accuracy;
			v[0] *= sine(t);
			v[1] *= sine(t);
			v[2] *= sine(t);
			v[3] = cosine(t);

			a = (k*accuracy + j)*3;
			b = (k*accuracy + j)*2;

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

			index[b+0] = pcount+(k*accuracy)+j;
			index[b+1] = pcount+(k*accuracy)+(j+1)%accuracy;
		}
	}
	for(k=0;k<accuracy;k++)
	{
		a = accuracy*17*2 + k*32;
		for(j=0;j<16;j++)
		{
			index[a + 2*j + 0] = pcount+k+(j*accuracy);
			index[a + 2*j + 1] = pcount+k+(j*accuracy)+accuracy;
		}
	}

	//
	a = accuracy*17*3;
	b = accuracy*17*2+accuracy*32;

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

	for(j=0;j<accuracy;j++)
	{
		index[b + 2*j +0] = pcount+accuracy*17;
		index[b + 2*j +1] = pcount+j;

		index[b + 2*j + accuracy*2 + 0] = pcount+accuracy*17+1;
		index[b + 2*j + accuracy*2 + 1] = pcount+accuracy*16+j;
	}
}
