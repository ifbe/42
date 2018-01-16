#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long



//l,r,f,b,u,d
void rubikscube_generate(u8* buf)
{
	int x,y,z;
	if(buf == 0)return;

	for(z=0;z<6;z++)
	{
	for(y=0;y<3;y++)
	{
	for(x=0;x<3;x++)
	{
		buf[z*9+y*3+x] = (z<<4)+(y*3)+x;
	}
	}
	}
}
