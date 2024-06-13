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



/*
--------method 0--------
#two equation
x0 * a + y0 * b + c = 0
x1 * a + y1 * b + c = 0
#relation between a b
x0 * a + y0 * b = x1 * a + y1 * b
a * (x0-x1) = b * (y1-y0)
a = b * (y1-y0) / (x0-x1)
#replace a with b
x0 * b * (y1-y0) / (x0-x1) + y0 * b + c = 0
#relation between b c
c = b * (x0 * (y1-y0) / (x1-x0) - y0)
#replace a c with b
x * b * (y1-y0) / (x0-x1) + y * b + b * (x0 * (y1-y0) / (x1-x0) - y0) = 0
#remove b
x * (y1-y0) / (x0-x1) + y + (x0 * (y1-y0) / (x1-x0) - y0) = 0
#mul (x0-x1)
x * (y1-y0) + y * (x0-x1) + x0*(y0-y1) + y0*(x1-x0) = 0
x * (y1-y0) + y * (x0-x1) + x0*y0-x0*y1+x1*y0-x0*y0 = 0
x * (y1-y0) + y * (x0-x1) + x1*y0-x0*y1 = 0

--------method 1--------
#vector:
linevec = (x-p0x, y-p0y)
normvec = (p1y-p0y, p0x-p1x)

#vertical relation
normvec * linevec = 0
(p1y-p0y)*(x-p0x) + (p0x-p1x)(y-p0y) = 0
(p1y-p0y)*x + (p0x-p1x)*y + (p0y-p1y)*p0x+(p1x-p0x)*p0y = 0
(p1y-p0y)*x + (p0x-p1x)*y + p0x*p0y-p0x*p1y+p0y*p1x-p0x*p0y = 0
(p1y-p0y)*x + (p0x-p1x)*y + p0y*p1x-p0x*p1y = 0
*/
void dim2_point2_getabc(float* p0, float* p1, float* abc)
{
	abc[0] = p1[1]-p0[1];
	abc[1] = p0[0]-p1[0];
	abc[2] = p0[1]*p1[0] - p0[0]*p1[1];
}

/*
#vector:
linevec = (x-p0x, y-p0y, z-p0z)
normvec = cross(p1-p0, p2-p0)
=
|   i       j       k   |
|p1x-p0x p1y-p0y p1z-p0z|
|p2x-p0x p2y-p0y p2z-p0z|
=
|p1y-p0y p1z-p0z| - |p1x-p0x p1z-p0z| + |p1x-p0x p1y-p0y|
|p2y-p0y p2z-p0z|   |p2x-p0x p2z-p0z|   |p2x-p0x p2y-p0y|
=
[ (p1y-p0y)*(p2z-p0z) - (p1z-p0z)*(p2y-p0y) ] -
[ (p1x-p0x)*(p2z-p0z) - (p1z-p0z)*(p2x-p0x) ] +
[ (p1x-p0x)*(p2y-p0y) - (p1y-p0y)*(p2x-p0x) ]

#vertical relation
normvec * linevec = 0
na*(x-p0x) + nb*(y-p0y) + nc*(z-p0z) = 0
na*x + nb*y + nc*z - (na*p0x+nb*p0y+nc*p0z) = 0
*/
void dim3_point3_getabcd(float* p0, float* p1, float* p2, float* abcd)
{
	abcd[0] = (p1[1]-p0[1])*(p2[2]-p0[2]) - (p1[2]-p0[2])*(p2[1]-p0[1]);
	abcd[1] = (p1[2]-p0[2])*(p2[0]-p0[0]) - (p1[0]-p0[0])*(p2[2]-p0[2]);
	abcd[2] = (p1[0]-p0[0])*(p2[1]-p0[1]) - (p1[1]-p0[1])*(p2[0]-p0[0]);
	abcd[3] = -( abcd[0]*p0[0] + abcd[1]*p0[1] + abcd[2]*p0[2] );
}

/*
x * a0 + y * b0 + c0 = 0
x * a1 + y * b1 + c1 = 0
#relation between x y
x * (a0-a1) + (c0-c1) = y * (b1-b0)
y = (x * (a0-a1) + (c0-c1)) / (b1-b0)
#replace y with x
x * a0 + b0 * (x * (a0-a1) + (c0-c1)) / (b1-b0) + c0 = 0
#mul (b1-b0)
x * a0 * (b1-b0) + b0 * (x * (a0-a1) + (c0-c1)) + c0 * (b1-b0) = 0
x * a0 * (b1-b0) + b0 * x * (a0-a1) + b0*(c0-c1) + c0 * (b1-b0) = 0
x * (a0 * (b1-b0) + b0 * (a0-a1)) + b0*c0-b0*c1+c0*b1-b0*c0 = 0
x * (a0*b1 - a0*b0 + b0*a0 - b0*a1) - b0*c1 + c0*b1 = 0
x * (a0*b1 - b0*a1) = b0*c1 - c0*b1
x = (b0*c1 - c0*b1) / (a0*b1 - b0*a1)
#deal with y
a0 * (b0*c1 - c0*b1) / (a0*b1 - b0*a1) + y * b0 + c0 = 0
(a0*b0*c1 - a0*c0*b1) + y*b0*(a0*b1 - b0*a1) +c0*(a0*b1 - b0*a1) = 0
a0*b0*c1 - a0*c0*b1 + y*b0*(a0*b1 - b0*a1) + c0*a0*b1 - c0*b0*a1 = 0
a0*b0*c1 + y*b0*(a0*b1 - b0*a1) - c0*b0*a1 = 0
a0*c1 + y*(a0*b1 - b0*a1) - c0*a1 = 0
y*(a0*b1 - b0*a1) = c0*a1 - a0*c1
y = (c0*a1 - a0*c1) / (a0*b1 - b0*a1)
*/
void dim2_twoline_intersect(float* M0, float* M1, float* N0, float* N1, float* out)
{
	vec3 Mabc;
	dim2_point2_getabc(M0, M1, Mabc);

	vec3 Nabc;
	dim2_point2_getabc(N0, N1, Nabc);

	float determinant = Mabc[0]*Nabc[1] - Mabc[1]*Nabc[0];
	if( (determinant >-1e-9) && (determinant < 1e-9) )return;

	out[0] = (Mabc[1]*Nabc[2] - Mabc[2]*Nabc[1]) / determinant;
	out[1] = (Mabc[2]*Nabc[0] - Mabc[0]*Nabc[2]) / determinant;
}


/*
p = Mo + Md*j
q = To + Td*k
vec_p_to_q = To-Mo + Td*k-Md*j
#
vec_p_to_q * Md = 0
(To-Mo + Td*k-Md*j) * Md = 0
Md*To - Md*Mo + Md*Td*k - Md*Md*j = 0
(Md*To - Md*Mo + Md*Td*k) / (Md*Md) = j
k = (Md*Mo - Md*To + Md*Md*j) / (Md*Td)
#
vec_p_to_q * Td = 0
(To-Mo + Td*k-Md*j) * Td = 0
Td*To - Td*Mo + Td*Td*k - Td*Md*j = 0
(Td*To - Td*Mo + Td*Td*k) / (Td*Md) = j
k = Td*Mo - Td*To + Td*Md*j) / (Td*Td)
#get k
(Md*To - Md*Mo + Md*Td*k) / (Md*Md) = (Td*To - Td*Mo + Td*Td*k) / (Td*Md)
(Md*To - Md*Mo + Md*Td*k) * (Td*Md) = (Td*To - Td*Mo + Td*Td*k) * (Md*Md)
(Td*Md)*(Md*To) - (Td*Md)*(Md*Mo) + (Td*Md)*(Md*Td)*k = (Md*Md)*(Td*To) - (Md*Md)*(Td*Mo) + (Md*Md)*(Td*Td)*k
(Td*Md)*(Md*Td)*k - (Md*Md)*(Td*Td)*k = (Md*Md)*(Td*To) - (Md*Md)*(Td*Mo) + (Td*Md)*(Md*Mo) - (Td*Md)*(Md*To)
k = [(Md*Md)*(Td*To) - (Md*Md)*(Td*Mo) + (Td*Md)*(Md*Mo) - (Td*Md)*(Md*To)] / [(Td*Md)*(Md*Td) - (Md*Md)*(Td*Td)]
#get j
(Md*Mo - Md*To + Md*Md*j) / (Md*Td) = (Td*Mo - Td*To + Td*Md*j) / (Td*Td)
(Md*Mo - Md*To + Md*Md*j) * (Td*Td) = (Td*Mo - Td*To + Td*Md*j) * (Md*Td)
(Td*Td)*(Md*Mo) - (Td*Td)*(Md*To) + (Td*Td)*(Md*Md)*j = (Md*Td)*(Td*Mo) - (Md*Td)*(Td*To) + (Md*Td)*(Td*Md)*j
(Td*Td)*(Md*Md)*j - (Md*Td)*(Td*Md)*j = (Md*Td)*(Td*Mo) - (Md*Td)*(Td*To) + (Td*Td)*(Md*To) - (Td*Td)*(Md*Mo)
j = [(Md*Td)*(Td*Mo) - (Md*Td)*(Td*To) + (Td*Td)*(Md*To) - (Td*Td)*(Md*Mo)] / [(Td*Td)*(Md*Md) - (Md*Td)*(Td*Md)]
*/
void dim3_twoline_nearpoint(float* Mo, float* Md, float* To, float* Td, float* out)
{
	float MdMo = Md[0]*Mo[0] + Md[1]*Mo[1] + Md[2]*Mo[2];
	float MdMd = Md[0]*Md[0] + Md[1]*Md[1] + Md[2]*Md[2];
	float MdTo = Md[0]*To[0] + Md[1]*To[1] + Md[2]*To[2];
	float MdTd = Md[0]*Td[0] + Md[1]*Td[1] + Md[2]*Td[2];

	float TdTo = Td[0]*To[0] + Td[1]*To[1] + Td[2]*To[2];
	float TdTd = Td[0]*Td[0] + Td[1]*Td[1] + Td[2]*Td[2];
	float TdMo = Td[0]*Mo[0] + Td[1]*Mo[1] + Td[2]*Mo[2];
	float TdMd = Td[0]*Md[0] + Td[1]*Md[1] + Td[2]*Md[2];

	float j = (TdMd*TdMo - TdMd*TdTo + TdTd*MdTo - TdTd*MdMo) / (TdTd*MdMd - MdTd*TdMd);
	float k = (MdMd*TdTo - MdMd*TdMo + TdMd*MdMo - TdMd*MdTo) / (TdMd*TdMd - MdMd*TdTd);

	out[0] = Mo[0] + Md[0]*j;
	out[1] = Mo[1] + Md[1]*j;
	out[2] = Mo[2] + Md[2]*j;

	out[3] = To[0] + Td[0]*k;
	out[4] = To[1] + Td[1]*k;
	out[5] = To[2] + Td[2]*k;
}