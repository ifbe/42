#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long



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
void rubikscube_solve(u8* buf, int level)
{
}