#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void say(void*, ...);




/*
(buf,size) -> (argc,argv)
*/
void buf2arg(u8* buf, int len, int* argc, u8** argv)
{
	int j;
	int count=0;
	int splited=0;
	argv[0]=0;

	//
	for(j=0;j<len;j++)
	{
		//finished
		if( buf[j] == 0 )break;

		//blank
		if( buf[j] <= 0x20 )
		{
			buf[j]=0;
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
			argv[count]=buf+j;
		}
	}//for

	//result
	count+=1;
	argv[count]=0;
	*argc=count;

/*
	//debug
	say("count=%x\n",count);
	for(j=0;j<count;j++)
	{
		say("%x=%s\n",j,argv[j]);
	}
*/
}




/*
"card=wlan0" -> "card" , "wlan0"
"user=name" -> "user" , "name"
*/
int buf2optval(u8* buf, int len, u8** type, u8** name)
{
	int j;
	for(j=0;j<len;j++)
	{
		if(buf[j] == 0)break;
		if(buf[j] == '=')break;
	}

	if( buf[j] != '=' )
	{
		*type = 0;
		*name = 0;
	}
	else
	{
		buf[j]=0;

		*type = buf;
		*name = buf+j+1;
	}
	return j;
}




/*
"%d%16s%3.5f\n" 71 "wertyui" 3.14159
*/
int buf2say(u8* buf, int len)
{
	u64 list[64];
	say(buf, list[0], list[1], list[2], list[3], list[4], list[5], list[6], list[7]);
}