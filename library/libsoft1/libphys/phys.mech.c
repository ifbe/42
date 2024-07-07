
#define G 6.67259e-11		//N*(m^2)/(kg^2) = N*(km^2)/(t^2)
#include "libsoft.h"




float gravity_f32(float m1,float m2,float r)
{
	return G*m1*m2/r/r;
}
double gravity_f64(double m1,double m2,double r)
{
	return G*m1*m2/r/r;
}




void mechanical_read()
{
}
void mechanical_write()
{
}
void mechanical_list()
{
}
void mechanical_choose()
{
}
void mechanical_attach()
{
}
void mechanical_detach()
{
}
void mechanical_create(u8* softaddr, u64* p)
{
}
void mechanical_delete()
{
}
