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




int str2type32(u8* buf, u8* type)
{
	int j;
	for(j=0;j<4;j++){
		if(buf[j] <= 0x20)break;
		type[j] = buf[j];
	}

	int k;
	for(k=j;k<4;k++)type[k] = 0;

	return j;
}
int str2type64(u8* buf, u8* type)
{
	int j;
	for(j=0;j<8;j++){
		if(buf[j] <= 0x20)break;
		type[j] = buf[j];
	}

	int k;
	for(k=j;k<8;k++)type[k] = 0;

	return j;
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