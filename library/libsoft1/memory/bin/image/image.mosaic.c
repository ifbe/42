#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void mosaic(QWORD size,QWORD partradius,DWORD* src,DWORD* dst)
{
	int x,y,m,n;
	int width=size&0xffff;
	int height=(size>>16)&0xffff;
	DWORD temp;

	int partx=size&0xffff;
	int party=(size>>16)&0xffff;
	int radius=(size>>48)&0xffff;
	radius=16;

	for(y=radius;y<height;y+=radius*2)
	{
	for(x=radius;x<width;x+=radius*2)
	{
		//
		temp=src[y*width+x];

		//y-
		for(n=-radius;n<0;n++)
		{
			//x-
			for(m=-radius;m<0;m++)
			{
				dst[width*(y+n)+x+m]=temp;
			}//m

			//x+
			for(m=0;m<radius;m++)
			{
				dst[width*(y+n)+x+m]=temp;
			}//m
		}//n

		//y+
		for(n=0;n<radius;n++)
		{
			//x-
			for(m=-radius;m<0;m++)
			{
				dst[width*(y+n)+x+m]=temp;
			}//m

			//x+
			for(m=0;m<radius;m++)
			{
				dst[width*(y+n)+x+m]=temp;
			}//m
		}//n
	}//x
	}//y
}
