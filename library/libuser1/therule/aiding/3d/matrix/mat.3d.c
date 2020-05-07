#include "libuser.h"
void world2view(mat4 m, struct fstyle* s);
void view2world(mat4 m, struct fstyle* s);
void view2clip_orth(mat4 m, struct fstyle* s);
void clip2view_orth(mat4 m, struct fstyle* s);
void view2clip_proj(mat4 m, struct fstyle* s);
void clip2view_proj(mat4 m, struct fstyle* s);




void matorth(mat4 mat, struct fstyle* sty)
{
	mat4 t;
	world2view(t, sty);

	view2clip_orth(mat, sty);
	mat4_multiply(mat, t);
}
void matorth_transpose(mat4 mat, struct fstyle* sty)
{
	matorth(mat, sty);
	mat4_transpose(mat);
}




void invorth(vec3 v, struct fstyle* sty)
{
}
void invorth_transpose(mat4 mat, struct fstyle* sty)
{
}




void matproj(mat4 mat, struct fstyle* sty)
{
	mat4 t;
	world2view(t, sty);

	view2clip_proj(mat, sty);
	mat4_multiply(mat, t);
}
void matproj_transpose(mat4 mat, struct fstyle* sty)
{
	matproj(mat, sty);
	mat4_transpose(mat);
}




void invproj(mat4 mat, struct fstyle* frus)
{
	mat4 q;
	clip2view_proj(q, frus);
	view2world(mat, frus);
	mat4_multiply(mat, q);
}
void invproj_transpose(mat4 mat, struct fstyle* frus)
{
	invproj(mat, frus);
	mat4_transpose(mat);
}
void worldxyzfromareauv(vec3 vec, struct fstyle* frus)
{
	vec4 t;
	t[0] = vec[0]*2.0 - 1.0;
	t[1] = vec[1]*2.0 - 1.0;
	t[2] = 0.5;
	//say("%f,%f\n",xyz[0],xyz[1]);

	mat4 m;
	invproj(m, frus);

	float f = m[3][0]*t[0] + m[3][1]*t[1] + m[3][2]*t[2] + m[3][3];
	vec[0] = (m[0][0]*t[0] + m[0][1]*t[1] + m[0][2]*t[2] + m[0][3]) / f;
	vec[1] = (m[1][0]*t[0] + m[1][1]*t[1] + m[1][2]*t[2] + m[1][3]) / f;
	vec[2] = (m[2][0]*t[0] + m[2][1]*t[1] + m[2][2]*t[2] + m[2][3]) / f;
}




void style2viewandclip(struct fstyle* frus, mat4 w2v, mat4 w2c)
{
	view2clip_proj(w2c, frus);
	world2view(w2v, frus);
	mat4_multiply(w2c, w2v);
}
void style2viewandclip_transpose(struct fstyle* frus, mat4 w2v, mat4 w2c)
{
	style2viewandclip(frus, w2v, w2c);
	mat4_transpose(w2v);
	mat4_transpose(w2c);
}
