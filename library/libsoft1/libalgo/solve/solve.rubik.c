#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




//l2, r2, f, b, u ,d
void rubikscube_group1(u8* buf, int level)
{
}

//l2, r2, f2, b2, u, d
void rubikscube_group2(u8* buf, int level)
{
}

//l2, r2, f2, b2, u2, d2
void rubikscube_group3(u8* buf, int level)
{
}

//finish it
void rubikscube_finalize(u8* buf, int level)
{
}

void rubikscube_solve(u8* buf, int level)
{
}




//left, right, near, far, bottom, upper
void rubikscube_generate(u8* buf, int level)
{
	int x,y,z;
	if(buf == 0)return;

	for(z=0;z<6;z++)
	{
		for(y=0;y<level;y++)
		{
			for(x=0;x<level;x++)
			{
				buf[(level*level*z)+(level*y)+x] = z;
			}
		}
	}
}