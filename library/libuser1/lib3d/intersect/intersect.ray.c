#include "libuser.h"
#define episilon 0.00000000001




//0: 0 intersection
//1: 1 intersection
int ray_point(vec3 ray[], vec3 p, vec3 out[])
{
	float* o = ray[0];
	float* d = ray[1];
	return 0;
}




//0: 0 intersection
//1: 1 intersection
//-1: infinite intersections
int ray_ray(vec3 ray0[], vec3 ray1[], vec3 out[])
{
	float* o0 = ray0[0];
	float* d0 = ray0[1];
	float* o1 = ray1[0];
	float* d1 = ray1[1];
	return 0;
}




//0: 0 intersection
//1: 1 intersection
//-1: infinite intersections
int ray_plane(vec3 ray[], vec3 plane[], vec3 out)
{
	vec3 d;
	float s,t;
	float* o = ray[0];
	float* v = ray[1];
	float* c = plane[0];
	float* n = plane[1];

	s = (c[0]-o[0])*n[0] + (c[1]-o[1])*n[1] + (c[2]-o[2])*n[2];
	t = v[0]*n[0] + v[1]*n[1] + v[2]*n[2];
	if((t>-episilon)&&(t<episilon))
	{
		if((s>-episilon)&&(s<episilon))return -1;
		return 0;
	}

	t = squareroot(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	d[0] = v[0] / t;
	d[1] = v[1] / t;
	d[2] = v[2] / t;

	t = s / (d[0]*n[0] + d[1]*n[1] + d[2]*n[2]);
	if(t < 0)return 0;

	out[0] = o[0] + t*d[0];
	out[1] = o[1] + t*d[1];
	out[2] = o[2] + t*d[2];
	return 1;
}




//0: 0 intersection
//n: n intersections
int ray_sphere(vec3 ray[], vec3 sphere[], vec3 out[])
{
	float* o = ray[0];
	float* d = ray[1];
	return 0;
}




//0: 0 intersection
//1: 1 intersection
//-1: infinite intersections
int ray_trigon(vec3 ray[], vec3 trigon[], vec3 out[])
{
	float* o = ray[0];
	float* d = ray[1];
	float* ta = trigon[0];
	float* tb = trigon[1];
	float* tc = trigon[2];
	return 0;
}




//0: 0 intersection
//1: 1 intersections
//-1: infinite intersections
int ray_rect(vec3 ray[], vec3 rect[], vec3 out)
{
	int ret;
	vec3 v;
	vec3 plane[2];
	float vx,vy,xx,yy;
	float* c = rect[0];
	float* r = rect[1];
	float* f = rect[2];
	float* u = rect[3];

	plane[0][0] = c[0];
	plane[0][1] = c[1];
	plane[0][2] = c[2];
	plane[1][0] = u[0];
	plane[1][1] = u[1];
	plane[1][2] = u[2];
	ret = ray_plane(ray, plane, out);
	if(ret <= 0)return ret;

	v[0] = out[0] - c[0];
	v[1] = out[1] - c[1];
	v[2] = out[2] - c[2];
	vx= v[0]*r[0] + v[1]*r[1] + v[2]*r[2];
	vy= v[0]*f[0] + v[1]*f[1] + v[2]*f[2];
	xx= r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
	yy= f[0]*f[0] + f[1]*f[1] + f[2]*f[2];
	if((vx>-xx)&&(vx<xx)&&(vy>-yy)&&(vy<yy))return 1;
	return 0;
}




//0: 0 intersection
//n: n intersections
//-1: infinite intersections
int ray_obb(vec3 ray[], struct fstyle* sty, vec3 out[])
{
	return 0;
}