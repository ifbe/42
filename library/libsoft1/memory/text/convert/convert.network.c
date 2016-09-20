#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int decstring2data(u8* source,u64* data);
void printmemory(char*,int);
void say(char*,...);




void buf2addrport(u8* pp,int max,u8* addr,int* port)
{
	int ii;
	u64 data=0;

	for(ii=0;ii<max;ii++)
	{
		if(pp[ii]==0)break;
		else if(pp[ii]==':')break;
		else addr[ii]=pp[ii];
	}

	if(pp[ii]!=':')
	{
		addr[0]=0;
		port[0]=0;
	}
	else
	{
		addr[ii]=0;

		decstring2data(pp+ii+1,&data);
		*port=(int)data;
	}
}




/*
"file://code/readme.txt" -> 'file' , "code/readme.txt"
"http://192.168.1.1/index.html" -> 'http' , "192.168.1.1/index.html"
*/
int buf2typename(u8* p,int max,u64* type,u8** name)
{
	int i=0;
	int j=0;
	int tail=0;
	if(p==0)return 0;

	*type=0;
	*name=p;
	for(i=0;i<max;i++)
	{
		//finished
		if( p[i] == 0)break;

		//"://"
		if( p[i] == ':' )
		{
			if( (p[i+1]=='/') && (p[i+2]=='/') )
			{
				for(j=i-1;j>=0;j--)
				{
					*type = (*type) << 8;
					*type += p[j];
				}
				*name=p+i+3;
				break;
			}
		}
	}

	//say("type=%s,name=%s\n",(char*)type,*name);
	return 1;
}
