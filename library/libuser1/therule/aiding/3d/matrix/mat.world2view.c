#include "libuser.h"




void world2view(mat4 m, struct fstyle* s)
{
	float x,y,z;
	float cx = s->vc[0];
	float cy = s->vc[1];
	float cz = s->vc[2];

	x = s->vr[0];
	y = s->vr[1];
	z = s->vr[2];
	m[0][0] = x;
	m[0][1] = y;
	m[0][2] = z;
	m[0][3] = - cx*x - cy*y - cz*z;

	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	m[1][0] = x;
	m[1][1] = y;
	m[1][2] = z;
	m[1][3] = - cx*x - cy*y - cz*z;

	x = -s->vf[0];
	y = -s->vf[1];
	z = -s->vf[2];
	m[2][0] = x;
	m[2][1] = y;
	m[2][2] = z;
	m[2][3] = - cx*x - cy*y - cz*z;

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
void world2view_transpose(mat4 m, struct fstyle* s)
{
	world2view(m, s);
	mat4_transpose(m);
}




void view2world(mat4 m, struct fstyle* s)
{
	m[0][0] = s->vr[0];
	m[0][1] = s->vt[0];
	m[0][2] =-s->vf[0];
	m[0][3] = s->vc[0];

	m[1][0] = s->vr[1];
	m[1][1] = s->vt[1];
	m[1][2] =-s->vf[1];
	m[1][3] = s->vc[1];

	m[2][0] = s->vr[2];
	m[2][1] = s->vt[2];
	m[2][2] =-s->vf[2];
	m[2][3] = s->vc[2];

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}
void view2world_transpose(mat4 m, struct fstyle* s)
{
	view2world(m, s);
	mat4_transpose(m);
}