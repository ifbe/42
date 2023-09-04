#include "libuser.h"
void printmat4(void*);
void world2view_rh2lh(mat4 m, struct fstyle* s);
void view2world_lh2rh(mat4 m, struct fstyle* s);
//
void view2clip_orthz0z1(mat4 m, struct fstyle* s);
void clip2view_orthz0z1(mat4 m, struct fstyle* s);
void view2clip_orthznzp(mat4 m, struct fstyle* s);
void clip2view_orthznzp(mat4 m, struct fstyle* s);
//
void view2clip_projz0z1(mat4 m, struct fstyle* s);
void clip2view_projz0z1(mat4 m, struct fstyle* s);
void view2clip_projznzp(mat4 m, struct fstyle* s);
void clip2view_projznzp(mat4 m, struct fstyle* s);




void world2clip_orthz0z1(mat4 mat, struct fstyle* sty)
{
	mat4 t;
	world2view_rh2lh(t, sty);

	view2clip_orthz0z1(mat, sty);
	mat4_multiply(mat, t);
}
void clip2world_orthz0z1(mat4 mat, struct fstyle* sty)
{
}
void world2clip_orthz0z1_transpose(mat4 mat, struct fstyle* sty)
{
	world2clip_orthz0z1(mat, sty);
	mat4_transpose(mat);
}
void clip2world_orthz0z1_transpose(mat4 mat, struct fstyle* sty)
{
	clip2world_orthz0z1(mat, sty);
	mat4_transpose(mat);
}




void world2clip_orthznzp(mat4 mat, struct fstyle* sty)
{
	mat4 t;
	world2view_rh2lh(t, sty);

	view2clip_orthznzp(mat, sty);
	mat4_multiply(mat, t);
}
void clip2world_orthznzp(mat4 mat, struct fstyle* sty)
{
}
void world2clip_orthznzp_transpose(mat4 mat, struct fstyle* sty)
{
	world2clip_orthznzp(mat, sty);
	mat4_transpose(mat);
}
void clip2world_orthznzp_transpose(mat4 mat, struct fstyle* sty)
{
	clip2world_orthznzp(mat, sty);
	mat4_transpose(mat);
}




void world2clip_projz0z1(mat4 mat, struct fstyle* sty)
{
	mat4 t;
	world2view_rh2lh(t, sty);

	view2clip_projz0z1(mat, sty);
	mat4_multiply(mat, t);
}
void world2clip_projz0z1_transpose(mat4 mat, struct fstyle* sty)
{
	world2clip_projz0z1(mat, sty);
	mat4_transpose(mat);
}




void clip2world_projz0z1(mat4 mat, struct fstyle* frus)
{
	mat4 q;
	clip2view_projz0z1(q, frus);
	view2world_lh2rh(mat, frus);
	mat4_multiply(mat, q);
}
void clip2world_projz0z1_transpose(mat4 mat, struct fstyle* frus)
{
	clip2world_projz0z1(mat, frus);
	mat4_transpose(mat);
}




void world2clip_projznzp(mat4 mat, struct fstyle* frus)
{
	mat4 t;
	world2view_rh2lh(t, frus);

	view2clip_projznzp(mat, frus);
	mat4_multiply(mat, t);
}
void world2clip_projznzp_transpose(mat4 mat, struct fstyle* frus)
{
	world2clip_projznzp(mat, frus);
	mat4_transpose(mat);
}




void clip2world_projznzp(mat4 mat, struct fstyle* frus)
{
	mat4 q;
	clip2view_projznzp(q, frus);
	view2world_lh2rh(mat, frus);
	mat4_multiply(mat, q);
}
void clip2world_projznzp_transpose(mat4 mat, struct fstyle* frus)
{
	clip2world_projznzp(mat, frus);
	mat4_transpose(mat);
}




void frustum2viewandclip(struct fstyle* frus, mat4 w2v, mat4 w2c)
{
	view2clip_projz0z1(w2c, frus);
	world2view_rh2lh(w2v, frus);
	mat4_multiply(w2c, w2v);
}
void frustum2viewandclip_transpose(struct fstyle* frus, mat4 w2v, mat4 w2c)
{
	frustum2viewandclip(frus, w2v, w2c);
	mat4_transpose(w2v);
	mat4_transpose(w2c);
}



/*
void testmats(struct fstyle* frus)
{
	mat4 m;
	mat4 n;

	view2world_lh2rh(m, frus);
	world2view_rh2lh(n, frus);
	mat4_multiply(n,m);
	printmat4(n);

	clip2view_projz0z1(m, frus);
	view2clip_projz0z1(n, frus);
	mat4_multiply(n,m);
	printmat4(n);
}*/
void worldxyz2areauv(vec3 vec, struct fstyle* frus)
{
	mat4 m;
	world2clip_projznzp(m, frus);
}




void areauv2worldxyz(vec3 vec, struct fstyle* frus)
{
	vec4 t;
	t[0] = vec[0]*2.0 - 1.0;
	t[1] = vec[1]*2.0 - 1.0;
	t[2] = 0.9;
	//say("%f,%f\n",xyz[0],xyz[1]);

	mat4 m;
	clip2world_projznzp(m, frus);

	float f = m[3][0]*t[0] + m[3][1]*t[1] + m[3][2]*t[2] + m[3][3];
	vec[0] = (m[0][0]*t[0] + m[0][1]*t[1] + m[0][2]*t[2] + m[0][3]) / f;
	vec[1] = (m[1][0]*t[0] + m[1][1]*t[1] + m[1][2]*t[2] + m[1][3]) / f;
	vec[2] = (m[2][0]*t[0] + m[2][1]*t[1] + m[2][2]*t[2] + m[2][3]) / f;
}