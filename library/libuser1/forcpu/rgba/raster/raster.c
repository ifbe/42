#include "libuser.h"




/*
void raster_position(
vec4 gl_Position,	//o
vec3 v,			//i
uniform mat)		//uniform
{
	gl_Position = mat*v;
}




void raster_fragment(
vec4 FragColor,		//o
vec3 v,vec3 n,		//i
uniform tex)		//uniform
{
	FragColor = vec3(1.0, 1.0, 1.0, 1.0);
}




void raster_trigon(struct supply* wnd, mat4 mat, float* t0, float* t1, float* t2)
{
	//step0: shader
	vert = raster_position;
	frag = raster_fragment;

	//step1: xyzw conv
	vec4 v0,v1,v2;
	vert(v0, t0, mat);
	vert(v1, t1, mat);
	vert(v2, t2, mat);

	//step2: each pixel
	vec3 v;
	while(1){
		v = this pixel

		float* pdepth = depth + v.y*stride + v.x;
		float* pcolor = color + v.y*stride + v.x;
		if(*pdepth < v.z){
			*pdepth = v.z;
			frag(pcolor, v, 0);
		}

		//next pixel
	}
}
*/
