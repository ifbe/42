#define K 9.0e9
double coulomb(double q1,double q2,double r)
{
	return K*q1*q2/r/r;
}
