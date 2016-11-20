#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int findzero(u8* p);
int ncmp(u8*, u8*, int);




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
say("%d,%c%c%c,%c%c%c\n",k,
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
