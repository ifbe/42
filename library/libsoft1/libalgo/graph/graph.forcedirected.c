#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct vertex
{
	float x;
	float y;
	float z;
	float w;
}
struct link
{
	u16 parent;
	u16 child;
}
void forcedirected(
	struct vertex* obuf, int olen,
	struct vertex* vbuf, int vlen,
	struct link* lbuf, int llen)
{
	int j,k;
	float x,y,z;

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
			t = 1.0 / t / squareroot(t);
			x *= t;
			y *= t;
			z *= t;

			obuf[k].x += x;
			obuf[k].y += y;
			obuf[k].z += z;
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
		obuf[lbuf[j].parnet].z -= z;
	}

	//move point
	for(j=0;j<vlen;j++)
	{
		vbuf[j].x += obuf[j].x;
		vbuf[j].y += obuf[j].y;
		vbuf[j].z += obuf[j].z;
	}
}
