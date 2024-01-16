#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int ncmp(u8*, u8*, int);




int findzero(char* p)
{
	int j=0;
	while(1)
	{
		if(p[j] == 0)break;

		j++;
	}
	return j;
}
int findtail(char* p)
{
	int j=0;
	while(1)
	{
		if(p[j] == 0)break;
		if(p[j] == 0xd)break;
		if(p[j] == 0xa)break;

		j++;
	}
	return j;
}
int findhead(char* p)
{
	int j=0;
	int flag=0;
	while(1)
	{
		if(p[j] == 0)break;
		if(p[j] == 0xa)
		{
			j++;
			break;
		}
		if(p[j] == 0xd)
		{
			if(p[j+1] == 0xa)
			{
				j+=2;
			}
			else j++;

			break;
		}

		j++;
	}
	return j;
}
u8* findstr(u8* src, int len, u8* target, int tarlen)
{
	int j,k;
	u8* p = 0;
	if(len < tarlen)return 0;

	for(j=0;j<len-tarlen;j++)
	{
		if(src[j] != target[0])continue;
		if(src[j] == 0)break;

		k = ncmp(src+j, target, tarlen);
/*
logtoall("%d,%c%c%c,%c%c%c\n",k,
	src[j],src[j+1],src[j+2],
	target[0],target[1],target[2]
);
*/
		if(k == 0)
		{
			p = src+j;
			break;
		}
	}

	return p;
}
