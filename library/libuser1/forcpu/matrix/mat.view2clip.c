#include "libuser.h"




void view2clip_orth(mat4 m, struct fstyle* s)
{
	float l = s->vl[3];
	float r = s->vr[3];
	float b = s->vb[3];
	float t = s->vt[3];
	float n = s->vn[3];
	float f = s->vf[3];
	say("%f,%f,%f,%f,%f,%f\n",l,r,b,t,n,f);

	m[0][0] = 2.0 / (r-l);
	m[0][1] = 0.0;
	m[0][2] = 0.0;
	m[0][3] = (l+r) / (l-r);

	m[1][0] = 0.0;
	m[1][1] = 2.0 / (t-b);
	m[1][2] = 0.0;
	m[1][3] = (b+t) / (b-t);

	m[2][0] = 0.0;
	m[2][1] = 0.0;
	m[2][2] = 2.0 / (n-f);
	m[2][3] = (n+f) / (n-f);

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
void clip2view_orth(mat4 m, struct fstyle* s)
{
}




void view2clip_proj_n1p1(mat4 proj, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];
	//say("%f,%f,%f,%f,%f,%f\n",l,r,b,t,n,f);

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
void clip2view_proj_n1p1(mat4 mat, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];

	mat[0][0] = (r-l) / 2.0 / n;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = (r+l) / 2.0 / n;

	mat[1][0] = 0.0;
	mat[1][1] = (t-b) / 2.0 / n;
	mat[1][2] = 0.0;
	mat[1][3] = (t+b) / 2.0 / n;

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 0.0;
	mat[2][3] = -1.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = (n-f) / 2.0 / n / f;
	mat[3][3] = (n+f) / 2.0 / n / f;
}




void view2clip_proj(mat4 proj, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];
	//say("%f,%f,%f,%f,%f,%f\n",l,r,b,t,n,f);

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
	proj[2][2] = f / (n-f);
	proj[2][3] = f * n / (n-f);

	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = -1.0;
	proj[3][3] = 0.0;
}
void clip2view_proj(mat4 mat, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];

	mat[0][0] = (r-l) / 2.0 / n;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = (r+l) / 2.0 / n;

	mat[1][0] = 0.0;
	mat[1][1] = (t-b) / 2.0 / n;
	mat[1][2] = 0.0;
	mat[1][3] = (t+b) / 2.0 / n;

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 0.0;
	mat[2][3] = -1.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = (n-f) / n / f;
	mat[3][3] = 1.0 / n;
}