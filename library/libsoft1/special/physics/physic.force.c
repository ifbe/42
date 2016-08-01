#define G 6.672e-11
#define K 9.0e9
double gravity(double m1,double m2,double r)
{
	return G*m1*m1/r/r;
}
double coulomb(double q1,double q2,double r)
{
	return K*q1*q2/r/r;
}
