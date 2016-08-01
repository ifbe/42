#define QWORD unsigned long long
#define BYTE unsigned char
void channel_b(QWORD size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4+2]=addr[i*4+1]=0;
	}
}
void channel_g(QWORD size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4]=addr[i*4+2]=0;
	}
}
void channel_r(QWORD size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4]=addr[i*4+1]=0;
	}
}




void channel_rg(QWORD size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4]=0;
	}
}
void channel_rb(QWORD size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4+1]=0;
	}
}
void channel_gb(QWORD size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4+2]=0;
	}
}
