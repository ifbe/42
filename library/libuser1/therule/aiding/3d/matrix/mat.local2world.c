#include "libuser.h"




void local2world(mat4 mat, struct fstyle* local, struct fstyle* world)
{
	float x,y,z,max;
	float nr, nf, nu;
	mat4 tmp;

	//move: (0,0,0) -> world
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = world->vc[0];
	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = world->vc[1];
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = world->vc[2];
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	//rotate: 1 -> world

	//scale: 1 -> world
	x = world->vr[0];
	y = world->vr[1];
	z = world->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = world->vf[0];
	y = world->vf[1];
	z = world->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = world->vt[0];
	y = world->vt[1];
	z = world->vt[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nr;
	if(max < nf)max = nf;
	if(max < nu)max = nu;

	tmp[0][0] = max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//scale: local -> 1
	x = local->vr[0];
	y = local->vr[1];
	z = local->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = local->vf[0];
	y = local->vf[1];
	z = local->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = local->vt[0];
	y = local->vt[1];
	z = local->vt[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nu*2;
	if(max < nr)max = nr;
	if(max < nf)max = nf;

	tmp[0][0] = 1.0 / max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0 / max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0 / max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//rotate: local -> 1

	//move: local -> (0,0,0)
	tmp[0][0] = 1.0;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = - local->vc[0];
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0;
	tmp[1][2] = 0.0;
	tmp[1][3] = - local->vc[1];
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0;
	tmp[2][3] = - local->vb[2];
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);
}
void local2world_transpose(mat4 mat, struct fstyle* local, struct fstyle* world)
{
	local2world(mat, local, world);
	mat4_transpose(mat);
}




void world2local(mat4 mat, struct fstyle* world, struct fstyle* local)
{
	float x,y,z,max;
	float nr, nf, nu;
	mat4 tmp;

	//move: (0,0,0) -> local
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = local->vc[0];
	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = local->vc[1];
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = local->vb[2];
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	//rotate: 1 -> local

	//scale: 1 -> local
	x = local->vr[0];
	y = local->vr[1];
	z = local->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = local->vf[0];
	y = local->vf[1];
	z = local->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = local->vt[0];
	y = local->vt[1];
	z = local->vt[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nu*2;
	if(max < nr)max = nr;
	if(max < nf)max = nf;

	tmp[0][0] = max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//scale: world -> 1
	x = world->vr[0];
	y = world->vr[1];
	z = world->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = world->vf[0];
	y = world->vf[1];
	z = world->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = world->vt[0];
	y = world->vt[1];
	z = world->vt[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nu;
	if(max < nr)max = nr;
	if(max < nf)max = nf;

	tmp[0][0] = 1.0 / max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0 / max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0 / max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//rotate: world -> 1

	//move: world -> (0,0,0)
	tmp[0][0] = 1.0;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = - world->vc[0];
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0;
	tmp[1][2] = 0.0;
	tmp[1][3] = - world->vc[1];
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0;
	tmp[2][3] = - world->vc[2];
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);
}
void world2local_transpose(mat4 mat, struct fstyle* world, struct fstyle* local)
{
	world2local(mat, world, local);
	mat4_transpose(mat);
}