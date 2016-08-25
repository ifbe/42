#define u64 unsigned long long
#define u8 unsigned char
void channel_b(u64 size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4+2]=addr[i*4+1]=0;
	}
}
void channel_g(u64 size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4]=addr[i*4+2]=0;
	}
}
void channel_r(u64 size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4]=addr[i*4+1]=0;
	}
}




void channel_rg(u64 size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4]=0;
	}
}
void channel_rb(u64 size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4+1]=0;
	}
}
void channel_gb(u64 size,char* addr)
{
	int x=size&0xffff;
	int y=(size>>16)&0xffff;
	int i;
	for(i=0;i<x*y;i++)
	{
		addr[i*4+2]=0;
	}
}
