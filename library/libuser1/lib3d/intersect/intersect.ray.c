#include "actor.h"




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
int ray_plane(vec3 ray[], vec3 plane[], vec3 out[])
{
	float* o = ray[0];
	float* d = ray[1];
	float* c = plane[0];
	float* n = plane[1];
	return 0;
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
//n: n intersections
//-1: infinite intersections
int ray_obb(vec3 ray[], struct style* sty, vec3 out[])
{
	float* o = ray[0];
	float* d = ray[1];
	return 0;
}