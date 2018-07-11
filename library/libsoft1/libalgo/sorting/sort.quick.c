#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




void sort_quick(u8* buf, int len)
{
	int j,k,tmp;

	j = 0;
	k = len-1;
	while(1)
	{
		while( (j<k) && (buf[0]<=buf[k]) )k--;
		while( (j<k) && (buf[0]>=buf[j]) )j++;
		if(j<k)
		{
			tmp = buf[j];
			buf[j] = buf[k];
			buf[k] = tmp;
		}
		else
		{
			tmp = buf[0];
			buf[0] = buf[k];
			buf[k] = tmp;
			break;
		}
	}
	sort_quick(buf, k);
	sort_quick(buf+k, len-k);
}