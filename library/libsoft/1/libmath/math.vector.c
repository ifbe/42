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




void vec3_normalize(vec3 v)
{
	float norm = squareroot(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] /= norm;
	v[1] /= norm;
	v[2] /= norm;
}
void vec3_cross(vec3 v, vec3 x)
{
	//a × b= [a2b3-a3b2,a3b1-a1b3, a1b2-a2b1]
	float t[3] = {v[0], v[1], v[2]};
	v[0] = t[1]*x[2] - t[2]*x[1];
	v[1] = t[2]*x[0] - t[0]*x[2];
	v[2] = t[0]*x[1] - t[1]*x[0];
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