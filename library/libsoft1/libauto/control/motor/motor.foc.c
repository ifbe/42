#include "libsoft.h"




//Ia,Ib,Ic -> Ix,Iy
void computeclark(float* in, float* out)
{
	out[0] = in[0] - in[1]/2 - in[2]/2;
	out[1] = (in[1] - in[2])*sqrt3/2;
}


//Ix,Iy -> Ia,Ib,Ic
void inverseclark(float* in, float* out)
{
	out[0] = in[0];
	out[1] =-in[0]/2 + in[1]*sqrt3/2;
	out[2] =-in[0]/2 - in[1]*sqrt3/2;
}


//Ix,Iy -> Id,Iq
void computepark(float* in, float* out)
{
	float a = 0.0;
	float c = getcos(a);
	float s = getsin(a);
	out[0] = in[0]*c - in[1]*s;
	out[1] = in[0]*s + in[1]*c;
}


//Id,Iq -> Ix,Iy
void inversepark(float* in, float* out)
{
}


//Ia,Ib,Ic -> Id,Iq
void computefoc(float* in, float* out)
{
	float tmp[4];
	computeclark(in, tmp);
	computepark(tmp, out);
}


//Id,Iq -> Ia,Ib,Ic
void inversefoc(float* in, float* out)
{
	float tmp[4];
	inversepark(in, tmp);
	inverseclark(tmp, out);
}
