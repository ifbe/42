#include "libuser.h"
void world2camera(mat4 m, struct fstyle* s);
void camera2world(mat4 m, struct fstyle* s);
void camera2ndc_orth(mat4 m, struct fstyle* s);
void ndc2camera_orth(mat4 m, struct fstyle* s);
void camera2ndc_proj(mat4 m, struct fstyle* s);
void ndc2camera_proj(mat4 m, struct fstyle* s);




void matorth(mat4 mat, struct fstyle* sty)
{
	mat4 t;
	world2camera(t, sty);

	camera2ndc_orth(mat, sty);
	mat4_multiply(mat, t);
}
void invorth(vec3 v, struct fstyle* sty)
{
}


void matorth_transpose(mat4 mat, struct fstyle* sty)
{
	matorth(mat, sty);
	mat4_transpose(mat);
}
void invorth_transpose(mat4 mat, struct fstyle* sty)
{
}




void matproj(mat4 mat, struct fstyle* sty)
{
	mat4 t;
	world2camera(t, sty);

	camera2ndc_proj(mat, sty);
	mat4_multiply(mat, t);
}
void invproj(vec3 v, struct fstyle* sty)
{
	mat4 q;
	ndc2camera_proj(q, sty);

	mat4 p;
	camera2world(p, sty);

	mat4_multiply(p, q);

	vec4 t = {v[0], v[1], v[2], 1.0};
	float f = p[3][0]*t[0] + p[3][1]*t[1] + p[3][2]*t[2] + p[3][3]*t[3];
	v[0] = (p[0][0]*t[0] + p[0][1]*t[1] + p[0][2]*t[2] + p[0][3]*t[3]) / f;
	v[1] = (p[1][0]*t[0] + p[1][1]*t[1] + p[1][2]*t[2] + p[1][3]*t[3]) / f;
	v[2] = (p[2][0]*t[0] + p[2][1]*t[1] + p[2][2]*t[2] + p[2][3]*t[3]) / f;
}


void matproj_transpose(mat4 mat, struct fstyle* sty)
{
	matproj(mat, sty);
	mat4_transpose(mat);
}
void invproj_transpose(mat4 mat, struct fstyle* sty)
{
}




void style2matrix2(struct fstyle* frus, mat4 v_, mat4 vp)
{
	camera2ndc_proj(vp, frus);
	world2camera(v_, frus);
	mat4_multiply(vp, v_);
}
void style2matrix2_transpose(struct fstyle* frus, mat4 v_, mat4 vp)
{
	style2matrix2(frus, v_, vp);
	mat4_transpose(v_);
	mat4_transpose(vp);
}
