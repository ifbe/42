double ln(double);
double squareroot(double);
double power(double, double);
double arctan2(double, double);
double cosine(double);
double sine(double);




float complexmodulus(float* d)
{
	return squareroot(d[0]*d[0] + d[1]*d[1]);
}
float complexangle(float* d)
{
	return arctan2(d[1], d[0]);
}
void complexpower_real(float* d, float e)
{
	float mod = complexmodulus(d);
	float ang = complexangle(d);
	float ppp = power(mod, e);
	d[0] = ppp * cosine(ang * e);
	d[1] = ppp * sine(ang * e);
}
void complexpower_complex(float* d, float* s)
{
/*
=(R*exp(i(θ+2kπ))) ^ (a+ib)
=(exp(i(θ+2kπ)+lnR)) ^ (a+ib)
=exp((i(θ+2kπ)+lnR) * (a+ib))
=exp(alnR-(θ+2kπ)*b)*exp(i((θ+2kπ)*a+blnR))
=exp(alnR-(θ+2kπ)*b)*(cos((θ+2kπ)*a+blnR)+i*sin((θ+2kπ)*a+blnR))
*/
	float lnr = 0.5 * ln(d[0]*d[0] + d[1]*d[1]);
	float ang = arctan2(d[1], d[0]);
	float j = power(2.71828, s[0]*lnr - s[1]*ang);
	float k = s[0]*ang + s[1]*lnr;
	d[0] = j * cosine(k);
	d[1] = j * sine(k);
}




void complexaddition(float* d, float* s)
{
	d[0] += s[0];
	d[1] += s[1];
}
void complexsubtraction(float* d, float* s)
{
	d[0] -= s[0];
	d[1] -= s[1];
}
void complexmulply(float* d, float* s)
{
	float t[2];
	t[0] = d[0];
	t[1] = d[1];

	d[0] = t[0]*s[0] - t[1]*s[1];
	d[1] = t[0]*s[1] + t[1]*s[0];
}
void complexdivision(float* d, float* s)
{
	float a;
	float t[2];

	a = s[0]*s[0] + s[1]*s[1];
	t[0] = s[0];
	t[1] = -s[1];

	complexmulply(d, t);
	d[0] /= a;
	d[1] /= a;
}