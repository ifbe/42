#define QWORD unsigned long long
#define BYTE unsigned char
void gray_b(QWORD size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4+2]=addr[i*4+1]=addr[i*4];
	}
}
void gray_g(QWORD size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4]=addr[i*4+2]=addr[i*4+1];
	}
}
void gray_r(QWORD size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4]=addr[i*4+1]=addr[i*4+2];
	}
}
//max(r,g,b)
void gray_max(QWORD size,char* addr)
{
	int i,temp;
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	for(i=0;i<x*y;i++)
	{
		temp=addr[i*4];
		if(addr[i*4+1]>temp)temp=addr[i*4+1];
		if(addr[i*4+2]>temp)temp=addr[i*4+2];
		addr[i*4]=addr[i*4+1]=addr[i*4+2]=temp&0xff;
	}
}
//(b+g+r)/3
void gray_average(QWORD size,char* addr)
{
	int i,temp;
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	for(i=0;i<x*y;i++)
	{
		temp=(addr[i*4]+addr[i*4+1]+addr[i*4+2])/3;
		addr[i*4]=addr[i*4+1]=addr[i*4+2]=temp&0xff;
	}
}
//0.072169b+0.715160g+0.212671r
void gray_opencv(QWORD size,char* addr)
{
	int i,temp;
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	for(i=0;i<x*y;i++)
	{
		temp=(4730*addr[i*4]+46869*addr[i*4+1]+13938*addr[i*4+2])>>16;
		addr[i*4]=addr[i*4+1]=addr[i*4+2]=temp&0xff;
	}
}
//0.11b+0.59g+0.3r
void gray_bio(QWORD size,char* addr)
{
	int i,temp;
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	for(i=0;i<x*y;i++)
	{
		temp=(28*addr[i*4]+151*addr[i*4+1]+30*addr[i*4+2])>>8;
		addr[i*4]=addr[i*4+1]=addr[i*4+2]=temp&0xff;
	}
}