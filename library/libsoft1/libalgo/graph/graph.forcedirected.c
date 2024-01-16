#include "libsoft.h"
double squareroot(double);




struct pair
{
	u16 parent;
	u16 child;
};
void forcedirected_2d(
	vec3 obuf[], int olen,
	vec3 vbuf[], int vlen,
	struct pair* lbuf, int llen)
{
	int j,k;
	float x,y,t;
	//logtoall("vlen=%d\n",vlen);

	//coulomb force
	for(j=0;j<vlen;j++)
	{
		obuf[j][0] = obuf[j][1] = 0.0;
		for(k=0;k<vlen;k++)
		{
			if(j == k)continue;

			x = vbuf[j][0] - vbuf[k][0];
			y = vbuf[j][1] - vbuf[k][1];

			//F = (vec/r)*(k*q1*q2)/(r^2)
			//F = vec*(k*q1*q2*)/(r^3)
			t = x*x + y*y;
			t = 0.01 / t / squareroot(t);
			x *= t;
			y *= t;

			obuf[j][0] += x;
			obuf[j][1] += y;
			//logtoall("%d,%d,%f,%f\n",j,k,x,y);
		}
	}

	//spring force
	for(j=0;j<llen;j++)
	{
		x = vbuf[lbuf[j].parent][0] - vbuf[lbuf[j].child][0];
		y = vbuf[lbuf[j].parent][1] - vbuf[lbuf[j].child][1];

		//F = vec*k*r
		t = squareroot(x*x + y*y);
		x /= t;
		y /= t;

		obuf[lbuf[j].child][0] += x;
		obuf[lbuf[j].child][1] += y;

		obuf[lbuf[j].parent][0] -= x;
		obuf[lbuf[j].parent][1] -= y;
	}

	//move point
	for(j=0;j<vlen;j++)
	{
/*
		logtoall("%f,%f -> %f,%f\n",
			vbuf[j][0], vbuf[j][1],
			obuf[j][0], obuf[j][1]
		);
*/
		vbuf[j][0] += obuf[j][0] / 50.0;
		vbuf[j][1] += obuf[j][1] / 50.0;
	}
	//logtoall("\n");
}
void forcedirected_3d(
	vec3 obuf[], int olen,
	vec3 vbuf[], int vlen,
	struct pair* lbuf, int llen)
{
	int j,k;
	float x,y,z,t;

	//coulomb force
	for(j=0;j<vlen;j++)
	{
		obuf[j][0] = obuf[j][1] = obuf[j][2] = 0.0;
		for(k=0;k<vlen;k++)
		{
			if(j == k)continue;
			x = vbuf[j][0] - vbuf[k][0];
			y = vbuf[j][1] - vbuf[k][1];
			z = vbuf[j][2] - vbuf[k][2];

			//F = (vec/r)*(k*q1*q2)/(r^2)
			//F = vec*(k*q1*q2*)/(r^3)
			t = x*x + y*y + z*z;
			t = 10000.0 / t / squareroot(t);
			x *= t;
			y *= t;
			z *= t;

			obuf[j][0] += x;
			obuf[j][1] += y;
			obuf[j][2] += z;
		}
	}

	//spring force
	for(j=0;j<llen;j++)
	{
		x = vbuf[lbuf[j].parent][0] - vbuf[lbuf[j].child][0];
		y = vbuf[lbuf[j].parent][1] - vbuf[lbuf[j].child][1];
		z = vbuf[lbuf[j].parent][2] - vbuf[lbuf[j].child][2];

		//F = vec*k*r
		t = squareroot(x*x + y*y + z*z);
		x /= t;
		y /= t;
		z /= t;

		obuf[lbuf[j].child][0] += x;
		obuf[lbuf[j].child][1] += y;
		obuf[lbuf[j].child][2] += z;

		obuf[lbuf[j].parent][0] -= x;
		obuf[lbuf[j].parent][1] -= y;
		obuf[lbuf[j].parent][2] -= z;
	}

	//move point
	for(j=0;j<vlen;j++)
	{
/*
		logtoall("%f,%f,%f -> %f,%f,%f\n",
			vbuf[j][0], vbuf[j][1], vbuf[j][2],
			obuf[j][0], obuf[j][1], obuf[j][2]
		);
*/
		vbuf[j][0] += obuf[j][0] / 50.0;
		vbuf[j][1] += obuf[j][1] / 50.0;
		vbuf[j][2] += obuf[j][2] / 50.0;
	}
	//logtoall("\n");
}
