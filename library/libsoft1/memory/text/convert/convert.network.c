#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int decstr2data(u8* source,u64* data);
void printmemory(char*,int);
void say(char*,...);




int buf2net(
	u8* pp, int max,
	u8* type, u8* addr, int* port, u8* detail
)
{
	int ii,jj;
	u64 data=0;
	if(pp==0)return 0;

	//search for ://
	for(ii=0;ii<max;ii++)
	{
		if(pp[ii]==0)break;
		if(pp[ii]==':')break;
	}

	jj = 0;
	type[0] = 0;
	if(pp[ii] == ':')
	{
		if( (pp[ii+1] == '/') && (pp[ii+2] == '/') )
		{
			for(jj=0;jj<ii;jj++)
			{
				type[jj] = pp[jj];
			}
			type[jj] = 0;
			jj = ii+3;
		}
	}

	//now addr
	pp += jj;
	for(ii=0;ii<max;ii++)
	{
		if(pp[ii]==0)break;
		else if(pp[ii]==':')break;
		else addr[ii]=pp[ii];
	}

	if(pp[ii] != ':')
	{
		addr[0]=0;
		port[0]=0;
	}
	else
	{
		addr[ii]=0;

		decstr2data(pp+ii+1,&data);
		*port=(int)data;
	}

	//say("%s,%s,%d\n", type, addr, port[0]);
	return 1;
}
