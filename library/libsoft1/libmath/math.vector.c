#include<math.h>




void vectornormalize(float* v)
{
	float norm = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] /= norm;
	v[1] /= norm;
	v[2] /= norm;
}
void vectorcross(float* v, float* x)
{
	//a × b= [a2b3-a3b2,a3b1-a1b3, a1b2-a2b1]
	float t[3] = {v[0], v[1], v[2]};
	v[0] = t[1]*x[2] - t[2]*x[1];
	v[1] = t[2]*x[0] - t[0]*x[2];
	v[2] = t[0]*x[1] - t[1]*x[0];
}
float vectordot(float* v, float* x)
{
	return v[0]*x[0] + v[1]*x[1] + v[2]*x[2];
}
float vectorcosine(float* v, float* x)
{
	float a[3] = {v[0], v[1], v[2]};
	float b[3] = {x[0], x[1], x[2]};
	vectornormalize(a);
	vectornormalize(b);
	return vectordot(a, b);
}