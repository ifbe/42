#define G 6.672e-11
double gravity(double m1,double m2,double r)
{
	return G*m1*m1/r/r;
}