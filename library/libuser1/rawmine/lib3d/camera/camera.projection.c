#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void quaternion_operation(float*, float*, float);
void matrixmultiply_4(float*, float*);




void fixview(mat4 viewmatrix, struct fstyle* sty)
{
	float x,y,z;
	float cx = sty->vc[0];
	float cy = sty->vc[1];
	float cz = sty->vc[2];

	x = sty->vr[0];
	y = sty->vr[1];
	z = sty->vr[2];
	viewmatrix[0][0] = x;
	viewmatrix[0][1] = y;
	viewmatrix[0][2] = z;
	viewmatrix[0][3] = - cx*x - cy*y - cz*z;

	x = sty->vt[0];
	y = sty->vt[1];
	z = sty->vt[2];
	viewmatrix[1][0] = x;
	viewmatrix[1][1] = y;
	viewmatrix[1][2] = z;
	viewmatrix[1][3] = - cx*x - cy*y - cz*z;

	x = -sty->vf[0];
	y = -sty->vf[1];
	z = -sty->vf[2];
	viewmatrix[2][0] = x;
	viewmatrix[2][1] = y;
	viewmatrix[2][2] = z;
	viewmatrix[2][3] = - cx*x - cy*y - cz*z;

	viewmatrix[3][0] = 0.0;
	viewmatrix[3][1] = 0.0;
	viewmatrix[3][2] = 0.0;
	viewmatrix[3][3] = 1.0f;
	//mat4_print(viewmatrix);
}
void fixproj(mat4 proj, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];

	proj[0][0] = 2 * n / (r-l);
	proj[0][1] = 0.0;
	proj[0][2] = (r+l) / (r-l);
	proj[0][3] = 0.0;

	proj[1][0] = 0.0;
	proj[1][1] = 2 * n / (t-b);
	proj[1][2] = (t+b) / (t-b);
	proj[1][3] = 0.0;

	proj[2][0] = 0.0;
	proj[2][1] = 0.0;
	proj[2][2] = (n+f) / (n-f);
	proj[2][3] = 2 * f * n / (n-f);

	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = -1.0;
	proj[3][3] = 0.0;
}
void fixorie(mat4 orie)
{
	orie[0][0] = 0.0;
	orie[0][1] = -1.0;
	orie[0][2] = 0.0;
	orie[0][3] = 0.0;

	orie[1][0] = 1.0;
	orie[1][1] = 0.0;
	orie[1][2] = 0.0;
	orie[1][3] = 0.0;

	orie[2][0] = 0.0;
	orie[2][1] = 0.0;
	orie[2][2] = 1.0;
	orie[2][3] = 0.0;

	orie[3][0] = 0.0;
	orie[3][1] = 0.0;
	orie[3][2] = 0.0;
	orie[3][3] = 1.0;
}
void fixmatrix(mat4 m, struct fstyle* sty)
{
	mat4 t;
	fixproj(m, sty);

	//fixorie(t);
	//mat4_multiply(m, t);

	//fixproj(t, sty);
	//mat4_multiply(m, t);

	fixview(t, sty);
	mat4_multiply(m, t);
}




void invproj(mat4 proj, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];

	proj[0][0] = (r-l) / 2.0 / n;
	proj[0][1] = 0.0;
	proj[0][2] = 0.0;
	proj[0][3] = (r+l) / 2.0 / n;

	proj[1][0] = 0.0;
	proj[1][1] = (t-b) / 2.0 / n;
	proj[1][2] = 0.0;
	proj[1][3] = (t+b) / 2.0 / n;

	proj[2][0] = 0.0;
	proj[2][1] = 0.0;
	proj[2][2] = 0.0;
	proj[2][3] = -1.0;

	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = (n-f) / 2.0 / n / f;
	proj[3][3] = (n+f) / 2.0 / n / f;
}
void invview(mat4 viewmatrix, struct fstyle* sty)
{
	viewmatrix[0][0] = sty->vr[0];
	viewmatrix[0][1] = sty->vt[0];
	viewmatrix[0][2] =-sty->vf[0];
	viewmatrix[0][3] = sty->vc[0];

	viewmatrix[1][0] = sty->vr[1];
	viewmatrix[1][1] = sty->vt[1];
	viewmatrix[1][2] =-sty->vf[1];
	viewmatrix[1][3] = sty->vc[1];

	viewmatrix[2][0] = sty->vr[2];
	viewmatrix[2][1] = sty->vt[2];
	viewmatrix[2][2] =-sty->vf[2];
	viewmatrix[2][3] = sty->vc[2];

	viewmatrix[3][0] = 0.0f;
	viewmatrix[3][1] = 0.0f;
	viewmatrix[3][2] = 0.0f;
	viewmatrix[3][3] = 1.0f;
}
void invmvp(vec3 v, struct fstyle* sty)
{
	float f;
	mat4 p;
	mat4 q;
	vec4 t = {v[0], v[1], v[2], 1.0};

	invview(p, sty);
	invproj(q, sty);
	mat4_multiply(p, q);

	f = p[3][0]*t[0] + p[3][1]*t[1] + p[3][2]*t[2] + p[3][3]*t[3];
	v[0] = (p[0][0]*t[0] + p[0][1]*t[1] + p[0][2]*t[2] + p[0][3]*t[3]) / f;
	v[1] = (p[1][0]*t[0] + p[1][1]*t[1] + p[1][2]*t[2] + p[1][3]*t[3]) / f;
	v[2] = (p[2][0]*t[0] + p[2][1]*t[1] + p[2][2]*t[2] + p[2][3]*t[3]) / f;
}
