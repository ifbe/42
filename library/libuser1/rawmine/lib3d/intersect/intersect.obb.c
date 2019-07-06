#include "libuser.h"
int ray_rect(vec3 ray[], vec3 rect[], vec3 out);




//1: yes
int obb_point(struct fstyle* obb, vec3 xyz, vec3 out[])
{
	return 0;
}




//0: 0 intersection
//n: n intersection
//-1: infinite intersections
int obb_ray(struct fstyle* obb, vec3 ray[], vec3 out[])
{
	int ret;
	vec3 rect[4];
	float* u = obb->vt;
	float* f = obb->vf;
	float* r = obb->vr;
	float* c = obb->vc;


	//left,right
	rect[1][0] = f[0];
	rect[1][1] = f[1];
	rect[1][2] = f[2];
	rect[2][0] = u[0];
	rect[2][1] = u[1];
	rect[2][2] = u[2];

	rect[3][0] = -r[0];
	rect[3][1] = -r[1];
	rect[3][2] = -r[2];
	rect[0][0] = c[0] + rect[3][0];
	rect[0][1] = c[1] + rect[3][1];
	rect[0][2] = c[2] + rect[3][2];
	ret = ray_rect(ray, rect, out[0]);
	if(ret != 0)return ret;

	rect[3][0] = r[0];
	rect[3][1] = r[1];
	rect[3][2] = r[2];
	rect[0][0] = c[0] + rect[3][0];
	rect[0][1] = c[1] + rect[3][1];
	rect[0][2] = c[2] + rect[3][2];
	ret = ray_rect(ray, rect, out[0]);
	if(ret != 0)return ret;


	//near,far
	rect[1][0] = r[0];
	rect[1][1] = r[1];
	rect[1][2] = r[2];
	rect[2][0] = u[0];
	rect[2][1] = u[1];
	rect[2][2] = u[2];

	rect[3][0] = -f[0];
	rect[3][1] = -f[1];
	rect[3][2] = -f[2];
	rect[0][0] = c[0] + rect[3][0];
	rect[0][1] = c[1] + rect[3][1];
	rect[0][2] = c[2] + rect[3][2];
	ret = ray_rect(ray, rect, out[0]);
	if(ret != 0)return ret;

	rect[3][0] = f[0];
	rect[3][1] = f[1];
	rect[3][2] = f[2];
	rect[0][0] = c[0] + rect[3][0];
	rect[0][1] = c[1] + rect[3][1];
	rect[0][2] = c[2] + rect[3][2];
	ret = ray_rect(ray, rect, out[0]);
	if(ret != 0)return ret;


	//bot, top
	rect[1][0] = r[0];
	rect[1][1] = r[1];
	rect[1][2] = r[2];
	rect[2][0] = f[0];
	rect[2][1] = f[1];
	rect[2][2] = f[2];

	rect[3][0] = -u[0];
	rect[3][1] = -u[1];
	rect[3][2] = -u[2];
	rect[0][0] = c[0] + rect[3][0];
	rect[0][1] = c[1] + rect[3][1];
	rect[0][2] = c[2] + rect[3][2];
	ret = ray_rect(ray, rect, out[0]);
	if(ret != 0)return ret;

	rect[3][0] = u[0];
	rect[3][1] = u[1];
	rect[3][2] = u[2];
	rect[0][0] = c[0] + rect[3][0];
	rect[0][1] = c[1] + rect[3][1];
	rect[0][2] = c[2] + rect[3][2];
	ret = ray_rect(ray, rect, out[0]);
	if(ret != 0)return ret;

	return 0;
}




//0: 0 intersection
//n: n intersection
//-1: infinite intersections
int obb_plane(struct fstyle* obb, vec3 plane[], vec3 out[])
{
	return 0;
}




//0: 0 intersection
//n: n intersections
int obb_sphere(struct fstyle* obb, vec3 sphere[], vec3 out[])
{
	return 0;
}




//0: 0 intersection
//n: n intersection
//-1: infinite intersections
int obb_trigon(struct fstyle* obb, vec3 trigon[], vec3 out[])
{
	return 0;
}




//0: 0 intersection
//n: n intersections
//-1: infinite intersections
int obb_obb(struct fstyle* obb, struct style* sty, vec3 out[])
{
	return 0;
}