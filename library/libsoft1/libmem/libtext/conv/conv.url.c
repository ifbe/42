#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int decstr2data(void*, void*);
void printmemory(char*, int);
void say(char*, ...);




int buf2net(
	u8* orig, int max,
	u8* type, u8* addr, int* port)
{
	int j,k;
	u8* pp;
	if(orig==0)return 0;
	pp = orig;

	//type
	for(j=0;j<max;j++)
	{
		if(pp[j]==0)return 0;
		if(pp[j]==':')
		{
			if( (pp[j+1] == '/') && (pp[j+2] == '/') )
			{
				for(k=0;k<j;k++)type[k] = pp[k];
				type[k] = 0;

				pp += j+3;
				break;
			}
		}
	}

	//addr
	for(j=0;j<max;j++)
	{
		if(pp[j]==0)return 1;
		else if(pp[j]==':')
		{
			addr[j] = 0;
			pp += j+1;
			break;
		}
		else addr[j] = pp[j];
	}

	//port
	j = decstr2data(pp, port);
	if( (j<=0) | (j>5) | (*port>65535) )return 0;
	pp += j;

	return pp-orig;
}
