#include "libsoft.h"
double squareroot(double);




void vec2_normalize(vec2 v)
{
	float norm = squareroot(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] /= norm;
	v[1] /= norm;
	v[2] /= norm;
}
float vec2_cross(vec2 v, vec2 x)
{
	return v[0]*x[1] - v[1]*x[0];
}
float vec2_dot(vec2 v, vec2 x)
{
	return v[0]*x[0] + v[1]*x[1];
}
float vec2_cosine(vec2 v, vec2 x)
{
	float a[2] = {v[0], v[1]};
	float b[2] = {x[0], x[1]};
	vec2_normalize(a);
	vec2_normalize(b);
	return vec2_dot(a, b);
}




float vec3_getlen(vec3 v)
{
	return squareroot(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
void vec3_setlen(vec3 v, float len)
{
	len /= vec3_getlen(v);
	v[0] *= len;
	v[1] *= len;
	v[2] *= len;
}
void vec3_normalizefrom(vec3 to, vec3 v)
{
	float norm = 1.0 / squareroot(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	to[0] = v[0] * norm;
	to[1] = v[1] * norm;
	to[2] = v[2] * norm;
}
void vec3_normalize(vec3 v)
{
	float norm = squareroot(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] /= norm;
	v[1] /= norm;
	v[2] /= norm;
}
float vec3_dot(vec3 v, vec3 x)
{
	return v[0]*x[0] + v[1]*x[1] + v[2]*x[2];
}
float vec3_cosine(vec3 v, vec3 x)
{
	float a[3] = {v[0], v[1], v[2]};
	float b[3] = {x[0], x[1], x[2]};
	vec3_normalize(a);
	vec3_normalize(b);
	return vec3_dot(a, b);
}
void vec3_cross(vec3 v, vec3 a, vec3 b)
{
	//a × b= [aybz-azby, azbx-axbz, axby-aybx]
	v[0] = a[1]*b[2] - a[2]*b[1];
	v[1] = a[2]*b[0] - a[0]*b[2];
	v[2] = a[0]*b[1] - a[1]*b[0];
}