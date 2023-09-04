double ln(double);
double squareroot(double);
double power(double, double);
double getcos(double);
double getsin(double);
double arctanyx(double, double);




float complex_modulus(float* d)
{
	return squareroot(d[0]*d[0] + d[1]*d[1]);
}
float complex_angle(float* d)
{
	return arctanyx(d[1], d[0]);
}
void complex_power_real(float* d, float e)
{
	float mod = complex_modulus(d);
	float ang = complex_angle(d);
	float ppp = power(mod, e);
	d[0] = ppp * getcos(ang * e);
	d[1] = ppp * getsin(ang * e);
}
void complex_power_complex(float* d, float* s)
{
/*
=(R*exp(i(θ+2kπ))) ^ (a+ib)
=(exp(i(θ+2kπ)+lnR)) ^ (a+ib)
=exp((i(θ+2kπ)+lnR) * (a+ib))
=exp(alnR-(θ+2kπ)*b)*exp(i((θ+2kπ)*a+blnR))
=exp(alnR-(θ+2kπ)*b)*(cos((θ+2kπ)*a+blnR)+i*sin((θ+2kπ)*a+blnR))
*/
	float lnr = 0.5 * ln(d[0]*d[0] + d[1]*d[1]);
	float ang = arctanyx(d[1], d[0]);
	float j = power(2.71828, s[0]*lnr - s[1]*ang);
	float k = s[0]*ang + s[1]*lnr;
	d[0] = j * getcos(k);
	d[1] = j * getsin(k);
}




void complex_add(float* d, float* s)
{
	d[0] += s[0];
	d[1] += s[1];
}
void complex_sub(float* d, float* s)
{
	d[0] -= s[0];
	d[1] -= s[1];
}
void complex_mul(float* d, float* s)
{
	float t[2];
	t[0] = d[0];
	t[1] = d[1];

	d[0] = t[0]*s[0] - t[1]*s[1];
	d[1] = t[0]*s[1] + t[1]*s[0];
}
void complex_div(float* d, float* s)
{
	float a;
	float t[2];

	a = s[0]*s[0] + s[1]*s[1];
	t[0] = s[0];
	t[1] = -s[1];

	complex_mul(d, t);
	d[0] /= a;
	d[1] /= a;
}
