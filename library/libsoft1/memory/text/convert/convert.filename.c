#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void say(char*,...);
static char buf[0x1000];




/*
"1.txt" -> "txt"
"2.html" -> "html"
*/
int getsuffix(u8* p,u8** suffix)
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




char* getfilename(char* p)
{
        int j=0;
        int k=0;
        while(1)
        {
                if(p[j] == 0)break;
                if(p[j] == '/')k=j+1;

                j++;
        }
        return p+k;
}




char* getfolder(char* p)
{
	int j = 0;
	int k = -1;
	while(1)
	{
		if(j>0x1000)break;
		if(p[j] == 0)break;
		if(p[j] == '/')k=j;

		buf[j]=p[j];
		j++;
	}

	if(k<0)return p;
	if(k==0)return "/";

	buf[k]=0;
	return buf;
}
