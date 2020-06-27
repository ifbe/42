#include<math.h>


double getsin(double in)
{
	return sin(in);
}
double arcsin(double x)
{
	return asin(x);
}


double getcos(double in)
{
	return cos(in);
}
double arccos(double x)
{
	return acos(x);
}


double gettan(double in)
{
	return tan(in);
}
double arctan(double in)
{
	return atan(in);
}


double gettanyx(double y, double x)
{
	return tan(y/x);
}
double arctanyx(double y, double x)
{
	return atan2(y, x);
}


double squareroot(double in)
{
	return sqrt(in);
}
double exponent(double in)
{
	return exp(in);
}
double power(double a, double b)
{
        return pow(a, b);
}


double lg(double in)
{
	return log10(in);
}
double ln(double in)
{
	return log(in);
}
double log2(double in)
{
	return log(in)/log(2);
}
