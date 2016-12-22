#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void say(char*,...);




/*
(buf,size) -> (argc,argv)
*/
void buf2arg(u8* buf,int max,int* argc,u8** argv)
{
	int i;
	int count=0;
	int splited=0;
	argv[0]=0;

	//
	for(i=0;i<max;i++)
	{
		//finished
		if( buf[i] == 0 )break;

		//blank
		if( buf[i] <= 0x20 )
		{
			buf[i]=0;
			splited=1;
			continue;
		}

		//new?
		if(splited != 0)
		{
			count++;
			if(count>=7)break;

			argv[count]=0;
			splited=0;
		}

		//new!
		if( argv[count]==0 )
		{
			argv[count]=buf+i;
		}
	}//for

	//result
	count+=1;
	argv[count]=0;
	*argc=count;

/*
	//debug
	say("count=%x\n",count);
	for(i=0;i<count;i++)
	{
		say("%x=%s\n",i,argv[i]);
	}
*/
}




/*
"card=wlan0" -> "card" , "wlan0"
"user=name" -> "user" , "name"
*/
int buf2optval(u8* pp,int max,u8** type,u8** name)
{
	int ii;
	for(ii=0;ii<max;ii++)
	{
		if(pp[ii]==0)break;
		if(pp[ii]=='=')break;
	}

	if( pp[ii] != '=' )
	{
		*type=0;
		*name=0;
	}
	else
	{
		pp[ii]=0;

		*type=pp;
		*name=pp+ii+1;
	}
	return ii;
}
