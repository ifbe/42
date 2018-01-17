#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
double squareroot(double);
void say(void*, ...);




struct vertex
{
	float x;
	float y;
	float z;
	float w;
};
struct pair
{
	u16 parent;
	u16 child;
};
void forcedirected_2d(
	struct vertex* obuf, int olen,
	struct vertex* vbuf, int vlen,
	struct pair* lbuf, int llen)
{
	int j,k;
	float x,y,t;
	//say("vlen=%d\n",vlen);

	//coulomb force
	for(j=0;j<vlen;j++)
	{
		obuf[j].x = obuf[j].y = 0.0;
		for(k=0;k<vlen;k++)
		{
			if(j == k)continue;

			x = vbuf[j].x - vbuf[k].x;
			y = vbuf[j].y - vbuf[k].y;

			//F = (vec/r)*(k*q1*q2)/(r^2)
			//F = vec*(k*q1*q2*)/(r^3)
			t = x*x + y*y;
			t = 0.01 / t / squareroot(t);
			x *= t;
			y *= t;

			obuf[j].x += x;
			obuf[j].y += y;
			//say("%d,%d,%f,%f\n",j,k,x,y);
		}
	}

	//spring force
	for(j=0;j<llen;j++)
	{
		x = vbuf[lbuf[j].parent].x - vbuf[lbuf[j].child].x;
		y = vbuf[lbuf[j].parent].y - vbuf[lbuf[j].child].y;

		//F = vec*k*r
		t = squareroot(x*x + y*y);
		x /= t;
		y /= t;

		obuf[lbuf[j].child].x += x;
		obuf[lbuf[j].child].y += y;

		obuf[lbuf[j].parent].x -= x;
		obuf[lbuf[j].parent].y -= y;
	}

	//move point
	for(j=0;j<vlen;j++)
	{
/*
		say("%f,%f -> %f,%f\n",
			vbuf[j].x, vbuf[j].y,
			obuf[j].x, obuf[j].y
		);
*/
		vbuf[j].x += obuf[j].x / 100.0;
		vbuf[j].y += obuf[j].y / 100.0;
	}
	//say("\n");
}
void forcedirected_3d(
	struct vertex* obuf, int olen,
	struct vertex* vbuf, int vlen,
	struct pair* lbuf, int llen)
{
	int j,k;
	float x,y,z,t;

	//coulomb force
	for(j=0;j<vlen;j++)
	{
		obuf[j].x = obuf[j].y = obuf[j].z = 0.0;
		for(k=0;k<vlen;k++)
		{
			if(j == k)continue;
			x = vbuf[j].x - vbuf[k].x;
			y = vbuf[j].y - vbuf[k].y;
			z = vbuf[j].z - vbuf[k].z;

			//F = (vec/r)*(k*q1*q2)/(r^2)
			//F = vec*(k*q1*q2*)/(r^3)
			t = x*x + y*y + z*z;
			t = 0.01 / t / squareroot(t);
			x *= t;
			y *= t;
			z *= t;

			obuf[j].x += x;
			obuf[j].y += y;
			obuf[j].z += z;
		}
	}

	//spring force
	for(j=0;j<llen;j++)
	{
		x = vbuf[lbuf[j].parent].x - vbuf[lbuf[j].child].x;
		y = vbuf[lbuf[j].parent].y - vbuf[lbuf[j].child].y;
		z = vbuf[lbuf[j].parent].z - vbuf[lbuf[j].child].z;

		//F = vec*k*r
		t = squareroot(x*x + y*y + z*z);
		x /= t;
		y /= t;
		z /= t;

		obuf[lbuf[j].child].x += x;
		obuf[lbuf[j].child].y += y;
		obuf[lbuf[j].child].z += z;

		obuf[lbuf[j].parent].x -= x;
		obuf[lbuf[j].parent].y -= y;
		obuf[lbuf[j].parent].z -= z;
	}

	//move point
	for(j=0;j<vlen;j++)
	{
/*
		say("%f,%f,%f -> %f,%f,%f\n",
			vbuf[j].x, vbuf[j].y, vbuf[j].z,
			obuf[j].x, obuf[j].y, obuf[j].z
		);
*/
		vbuf[j].x += obuf[j].x / 100.0;
		vbuf[j].y += obuf[j].y / 100.0;
		vbuf[j].z += obuf[j].z / 100.0;
	}
	//say("\n");
}
