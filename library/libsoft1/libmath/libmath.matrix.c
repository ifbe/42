#include "libsoft.h"




void mat2_transpose(float* u)
{
	float t;
	t = u[1];
	u[1] = u[2];
	u[2] = t;
}
void mat2_multiply(float* u, float* v)
{
	int j;
	float w[4];
	for(j=0;j<4;j++)w[j] = u[j];

	u[0] = w[0]*v[0] + w[1]*v[2];
	u[1] = w[0]*v[1] + w[1]*v[3];

	u[2] = w[2]*v[0] + w[3]*v[2];
	u[3] = w[2]*v[1] + w[3]*v[3];
}
int mat2_inverse(float* d, float* s)
{
	float t = s[0]*s[3] - s[1]*s[2];
	if((t > -1e-50)&&(t < 1e-50))return -1;

	d[0] = s[3] / t;
	d[1] = -s[1] / t;
	d[2] = -s[2] / t;
	d[3] = s[0] / t;
	return 0;
}
float mat2_det(mat2 m)
{
	return m[0][0]*m[1][1] - m[0][1]*m[1][0];
}




void mat3_transposefrom(float* o, float* i)
{
	o[0] = i[ 0];
	o[1] = i[ 3];
	o[2] = i[ 6];

	o[3] = i[ 1];
	o[4] = i[ 4];
	o[5] = i[ 7];

	o[6] = i[ 2];
	o[7] = i[ 5];
	o[8] = i[ 8];
}
void mat3_transpose(float* u)
{
	float t;

	t = u[1];
	u[1] = u[3];
	u[3] = t;

	t = u[2];
	u[2] = u[6];
	u[6] = t;

	t = u[5];
	u[5] = u[7];
	u[7] = t;
}
void mat3_multiplyfrom(float* o, float* u, float* v)
{
	o[0] = u[0]*v[0] + u[1]*v[3] + u[2]*v[6];
	o[1] = u[0]*v[1] + u[1]*v[4] + u[2]*v[7];
	o[2] = u[0]*v[2] + u[1]*v[5] + u[2]*v[8];

	o[3] = u[3]*v[0] + u[4]*v[3] + u[5]*v[6];
	o[4] = u[3]*v[1] + u[4]*v[4] + u[5]*v[7];
	o[5] = u[3]*v[2] + u[4]*v[5] + u[5]*v[8];

	o[6] = u[6]*v[0] + u[7]*v[3] + u[8]*v[6];
	o[7] = u[6]*v[1] + u[7]*v[4] + u[8]*v[7];
	o[8] = u[6]*v[2] + u[7]*v[5] + u[8]*v[8];
}
void mat3_multiply(float* l, float* r)
{
	int j;
	float t[9];
	for(j=0;j<9;j++)t[j] = l[j];
	mat3_multiplyfrom(l, t, r);
}
int mat3_inverse(float* d, float* s)
{
	float t = s[0]*(s[4]*s[8]-s[5]*s[7])
			- s[3]*(s[1]*s[8]-s[2]*s[7])
			+ s[6]*(s[1]*s[5]-s[2]*s[4]);
	if((t > -1e-50)&&(t < 1e-50))return -1;

	t = 1.0 / t;
	d[0] = (s[4]*s[8] - s[5]*s[7]) * t;
	d[1] = (s[2]*s[7] - s[1]*s[8]) * t;
	d[2] = (s[1]*s[5] - s[2]*s[4]) * t;
	d[3] = (s[5]*s[6] - s[3]*s[8]) * t;
	d[4] = (s[0]*s[8] - s[2]*s[6]) * t;
	d[5] = (s[3]*s[2] - s[0]*s[5]) * t;
	d[6] = (s[3]*s[7] - s[4]*s[6]) * t;
	d[7] = (s[1]*s[6] - s[0]*s[7]) * t;
	d[8] = (s[0]*s[4] - s[3]*s[1]) * t;
	return 0;
}
float mat3_det(mat3 m)
{
	return 0 +
	m[0][0]*m[1][1]*m[2][2] +
	m[0][2]*m[1][0]*m[2][1] +
	m[0][1]*m[1][2]*m[2][0] -
	m[0][0]*m[1][2]*m[2][1] -
	m[0][1]*m[1][0]*m[2][2] -
	m[0][2]*m[1][1]*m[2][0];
}




void mat4_multiplyfrom(float* o, float* u, float* v)
{
	o[ 0] = u[ 0]*v[ 0] + u[ 1]*v[ 4] + u[ 2]*v[ 8] + u[ 3]*v[12];
	o[ 1] = u[ 0]*v[ 1] + u[ 1]*v[ 5] + u[ 2]*v[ 9] + u[ 3]*v[13];
	o[ 2] = u[ 0]*v[ 2] + u[ 1]*v[ 6] + u[ 2]*v[10] + u[ 3]*v[14];
	o[ 3] = u[ 0]*v[ 3] + u[ 1]*v[ 7] + u[ 2]*v[11] + u[ 3]*v[15];

	o[ 4] = u[ 4]*v[ 0] + u[ 5]*v[ 4] + u[ 6]*v[ 8] + u[ 7]*v[12];
	o[ 5] = u[ 4]*v[ 1] + u[ 5]*v[ 5] + u[ 6]*v[ 9] + u[ 7]*v[13];
	o[ 6] = u[ 4]*v[ 2] + u[ 5]*v[ 6] + u[ 6]*v[10] + u[ 7]*v[14];
	o[ 7] = u[ 4]*v[ 3] + u[ 5]*v[ 7] + u[ 6]*v[11] + u[ 7]*v[15];

	o[ 8] = u[ 8]*v[ 0] + u[ 9]*v[ 4] + u[10]*v[ 8] + u[11]*v[12];
	o[ 9] = u[ 8]*v[ 1] + u[ 9]*v[ 5] + u[10]*v[ 9] + u[11]*v[13];
	o[10] = u[ 8]*v[ 2] + u[ 9]*v[ 6] + u[10]*v[10] + u[11]*v[14];
	o[11] = u[ 8]*v[ 3] + u[ 9]*v[ 7] + u[10]*v[11] + u[11]*v[15];

	o[12] = u[12]*v[ 0] + u[13]*v[ 4] + u[14]*v[ 8] + u[15]*v[12];
	o[13] = u[12]*v[ 1] + u[13]*v[ 5] + u[14]*v[ 9] + u[15]*v[13];
	o[14] = u[12]*v[ 2] + u[13]*v[ 6] + u[14]*v[10] + u[15]*v[14];
	o[15] = u[12]*v[ 3] + u[13]*v[ 7] + u[14]*v[11] + u[15]*v[15];
}
void mat4_multiply(float* l, float* r)
{
	int j;
	float t[16];
	for(j=0;j<16;j++)t[j] = l[j];
	mat4_multiplyfrom(l, t, r);
}
void mat4_transposefrom(float* m, float* u)
{
	m[ 0] = u[ 0];
	m[ 1] = u[ 4];
	m[ 2] = u[ 8];
	m[ 3] = u[12];

	m[ 4] = u[ 1];
	m[ 5] = u[ 5];
	m[ 6] = u[ 9];
	m[ 7] = u[13];

	m[ 8] = u[ 2];
	m[ 9] = u[ 6];
	m[10] = u[10];
	m[11] = u[14];

	m[12] = u[ 3];
	m[13] = u[ 7];
	m[14] = u[11];
	m[15] = u[15];
}
void mat4_transpose(float* u)
{
	float t;

	t = u[1];
	u[1] = u[4];
	u[4] = t;

	t = u[2];
	u[2] = u[8];
	u[8] = t;

	t = u[3];
	u[3] = u[12];
	u[12] = t;

	t = u[6];
	u[6] = u[9];
	u[9] = t;

	t = u[7];
	u[7] = u[13];
	u[13] = t;

	t = u[11];
	u[11] = u[14];
	u[14] = t;
}
int mat4_inverse(float* d, float* s)
{
	return 0;
}
float mat4_det(mat4 m)
{
	mat3 n;
	int j;
	float t = 0.0;

	//m[3][0]
	for(j=0;j<3;j++)
	{
		n[j][0] = m[j][1];
		n[j][1] = m[j][2];
		n[j][2] = m[j][3];
	}
	t -= m[3][0]*mat3_det(n);

	//m[3][1]
	for(j=0;j<3;j++)
	{
		n[j][0] = m[j][0];
		n[j][1] = m[j][2];
		n[j][2] = m[j][3];
	}
	t += m[3][1]*mat3_det(n);

	//m[3][2]
	for(j=0;j<3;j++)
	{
		n[j][0] = m[j][0];
		n[j][1] = m[j][1];
		n[j][2] = m[j][3];
	}
	t -= m[3][2]*mat3_det(n);

	//m[3][3]
	for(j=0;j<3;j++)
	{
		n[j][0] = m[j][0];
		n[j][1] = m[j][1];
		n[j][2] = m[j][2];
	}
	t += m[3][3]*mat3_det(n);

	return t;
}