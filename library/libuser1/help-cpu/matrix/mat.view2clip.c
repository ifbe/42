#include "libuser.h"




//out ndc: x[-1,1], y[-1,1], z[-1,1]
//in frus: l,r, b,t, n,f
void view2clip_orthznzp(mat4 m, struct fstyle* s)
{
	float l = s->vl[3];
	float r = s->vr[3];
	float b = s->vb[3];
	float t = s->vt[3];
	float n = s->vn[3];
	float f = s->vf[3];
	//logtoall("%f,%f,%f,%f,%f,%f\n",l,r,b,t,n,f);

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
	m[2][2] = 2.0 / (f-n);
	m[2][3] = (n+f) / (n-f);

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
void clip2view_orthznzp(mat4 m, struct fstyle* s)
{
}




//out ndc: x[-1,1], y[-1,1], z[0,1]
//in frus: l,r, b,t, n,f
/*
n->0
f->1
z->(z-n)/(f-n)=z/(f-n)+n/(n-f)
*/
void view2clip_orthz0z1(mat4 m, struct fstyle* s)
{
	float l = s->vl[3];
	float r = s->vr[3];
	float b = s->vb[3];
	float t = s->vt[3];
	float n = s->vn[3];
	float f = s->vf[3];
	//logtoall("%f,%f,%f,%f,%f,%f\n",l,r,b,t,n,f);

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
	m[2][2] = 1.0 / (f-n);
	m[2][3] = n / (n-f);

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
void clip2view_orthz0z1(mat4 m, struct fstyle* s)
{
}

//out ndc: x[-1,1], y[-1,1], z[1,0]		//warning: near=1,far=0
//in frus: l,r, b,t, n,f
/*
n->1
f->0
z->(f-z)/(f-n)=z/(n-f)+f/(f-n)
*/
void view2clip_orthz0z1_reversez(mat4 m, struct fstyle* s)
{
	float l = s->vl[3];
	float r = s->vr[3];
	float b = s->vb[3];
	float t = s->vt[3];
	float n = s->vn[3];
	float f = s->vf[3];
	//logtoall("%f,%f,%f,%f,%f,%f\n",l,r,b,t,n,f);

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
	m[2][2] = 1.0 / (n-f);
	m[2][3] = f / (f-n);

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
void clip2view_orthz0z1_reversez(mat4 m, struct fstyle* s)
{
}




//out ndc: x[-1,1], y[-1,1], z[-1,1]
//in frus: l,r, b,t, n,f
void view2clip_projznzp(mat4 proj, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];
	//logtoall("%f,%f,%f,%f,%f,%f\n",l,r,b,t,n,f);

	proj[0][0] = 2 * n / (r-l);
	proj[0][1] = 0.0;
	proj[0][2] = (r+l) / (l-r);
	proj[0][3] = 0.0;

	proj[1][0] = 0.0;
	proj[1][1] = 2 * n / (t-b);
	proj[1][2] = (t+b) / (b-t);
	proj[1][3] = 0.0;

	proj[2][0] = 0.0;
	proj[2][1] = 0.0;
	proj[2][2] = (n+f) / (f-n);
	proj[2][3] = 2*f*n / (n-f);

	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = 1.0;
	proj[3][3] = 0.0;
}
void clip2view_projznzp(mat4 mat, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];

	mat[0][0] = 0.5 * (r-l) / n;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.5 * (r+l) / n;

	mat[1][0] = 0.0;
	mat[1][1] = 0.5 * (t-b) / n;
	mat[1][2] = 0.0;
	mat[1][3] = 0.5 * (t+b) / n;

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 0.0;
	mat[2][3] = 1.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.5 * (n-f) / n / f;
	mat[3][3] = 0.5 * (n+f) / n / f;
}




//out ndc: x[-1,1], y[-1,1], z[0,1]
//in frus: l,r, b,t, n,f
void view2clip_projz0z1(mat4 proj, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];
	//logtoall("%f,%f,%f,%f,%f,%f\n",l,r,b,t,n,f);

	proj[0][0] = 2 * n / (r-l);
	proj[0][1] = 0.0;
	proj[0][2] = (r+l) / (l-r);
	proj[0][3] = 0.0;

	proj[1][0] = 0.0;
	proj[1][1] = 2 * n / (t-b);
	proj[1][2] = (t+b) / (b-t);
	proj[1][3] = 0.0;

	proj[2][0] = 0.0;
	proj[2][1] = 0.0;
	proj[2][2] = f / (f-n);
	proj[2][3] = f * n / (n-f);

	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = 1.0;
	proj[3][3] = 0.0;
}
void clip2view_projz0z1(mat4 mat, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];

	mat[0][0] = 0.5 * (r-l) / n;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.5 * (r+l) / n;

	mat[1][0] = 0.0;
	mat[1][1] = 0.5 * (t-b) / n;
	mat[1][2] = 0.0;
	mat[1][3] = 0.5 * (t+b) / n;

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 0.0;
	mat[2][3] = 1.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = (n-f) / n / f;
	mat[3][3] = 1.0 / n;
}

//out ndc: x[-1,1], y[-1,1], z[1,0]		//warning: near=1,far=0
//in frus: l,r, b,t, n,f
void view2clip_projz0z1_reversez(mat4 proj, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];
	//logtoall("%f,%f,%f,%f,%f,%f\n",l,r,b,t,n,f);

	proj[0][0] = 2 * n / (r-l);
	proj[0][1] = 0.0;
	proj[0][2] = (r+l) / (l-r);
	proj[0][3] = 0.0;

	proj[1][0] = 0.0;
	proj[1][1] = 2 * n / (t-b);
	proj[1][2] = (t+b) / (b-t);
	proj[1][3] = 0.0;

	proj[2][0] = 0.0;
	proj[2][1] = 0.0;
	proj[2][2] = n / (n-f);
	proj[2][3] = f * n / (f-n);

	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = 1.0;
	proj[3][3] = 0.0;
}
void clip2view_projz0z1_reversez(mat4 mat, struct fstyle* sty)
{
	float l = sty->vl[3];
	float r = sty->vr[3];
	float b = sty->vb[3];
	float t = sty->vt[3];
	float n = sty->vn[3];
	float f = sty->vf[3];

	mat[0][0] = 0.5 * (r-l) / n;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.5 * (r+l) / n;

	mat[1][0] = 0.0;
	mat[1][1] = 0.5 * (t-b) / n;
	mat[1][2] = 0.0;
	mat[1][3] = 0.5 * (t+b) / n;

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 0.0;
	mat[2][3] = 1.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = (f-n) / n / f;
	mat[3][3] = 1.0 / f;
}