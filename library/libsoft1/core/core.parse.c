#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void say(char*,...);




/*
(buf,size) -> (argc,argv)
*/
void buf2arg(BYTE* buf,int max,int* argc,BYTE** argv)
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
int buf2optval(BYTE* p,int max,BYTE** type,BYTE** name)
{
}




/*
"1.txt" -> "txt"
"2.html" -> "html"
*/
int buf2suffix(BYTE* p,BYTE** suffix)
{
	int i=0;
	int tail=0;
	if(p==0)return 0;

	for(i=0;i<256;i++)
	{
		//all possible '.'
		if( p[i] == '.' )
		{
			tail=i;
		}

		//finished
		else if( p[i] == 0 )
		{
			//".gitignore" is not an extension
			if(tail==0)
			{
				*suffix=0;
			}
			else
			{
				*suffix = p + tail;
			}
			break;
		}
	}

	//say("suffix=%s\n",(char*)suffix);
	return 1;
}




/*
"file://code/readme.txt" -> 'file' , "code/readme.txt"
"http://192.168.1.1/index.html" -> 'http' , "192.168.1.1/index.html"
*/
int buf2typename(BYTE* p,int max,QWORD* type,BYTE** name)
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