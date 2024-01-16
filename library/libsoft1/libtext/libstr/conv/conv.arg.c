#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void logtoall(void*, ...);




int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max)
{
	int j,k,n;
	k = 0;
	n = 0;
	for(j=0;j<len;j++)
	{
		if(buf[j] > 0x20)
		{
			argv[n] = tmp+k;
			n++;

			while(buf[j] > 0x20)
			{
				tmp[k] = buf[j];
				j++;
				k++;
			}

			tmp[k] = 0;
			k++;
		}
	}

	return n;
}




/*
"card=wlan0" -> "card" , "wlan0"
"user=name" -> "user" , "name"
*/
int buf2keyval(
	u8* buf, int len,
	u8* cutstr, u8 cutchr,
	u8** key, int* keylen,
	u8** val, int* vallen)
{
	int j;
	for(j=0;j<len;j++)
	{
		if(buf[j] == 0)break;
		if(buf[j] == '=')break;
	}

	if( buf[j] != '=' )
	{
		*key = 0;
		*val = 0;
	}
	else
	{
		buf[j]=0;

		*key = buf;
		*val = buf+j+1;
	}
	return j;
}




/*
"%d%16s%3.5f\n" 71 "wertyui" 3.14159
*/
int buf2logtoall(u8* buf, int len)
{
	u64 list[64];
	logtoall(buf,
		list[0], list[1], list[2], list[3],
		list[4], list[5], list[6], list[7]
	);
	return 0;
}
