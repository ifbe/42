#define u8 unsigned char




/*
static void sort_merge_2in1(
	u8* buf1, int len1,
	u8* buf2, int len2,
	u8* buf)
{
	int j=0,k=0,dst=0;
	while(1)
	{
		if(j >= len1)
		{
			for(;k<len2;k++)
			{
				buf[dst] = buf2[k];
				dst++;
				goto theend;
			}
		}
		if(k >= len2)
		{
			for(;j<len1;j++)
			{
				buf[dst] = buf1[j];
				dst++;
				goto theend;
			}
		}

		if(buf1[j] < buf2[k])
		{
			buf[dst] = buf1[j];
			dst++;
			j++;
		}
		else
		{
			buf[dst] = buf2[k];
			dst++;
			k++;
		}
	}
theend:
	return;
}
*/




static void sort_merge_insert(
	u8* buf1, int len1,
	u8* buf2, int len2)
{
	int j, k, tmp;
	int last, defier;

	last = len1-1;
	for(defier=0;defier<len2;defier++)
	{
		for(j=0;j<=last;j++)
		{
			tmp = buf2[defier];
			if(tmp >= buf1[j])continue;

			for(k=last;k>=j;k--)buf1[k+1] = buf1[k];
			buf1[j] = tmp;
			break;
		}
		last++;
	}
}
void sort_merge(u8* buf, int len)
{
	int tmp;
	if(len <= 1)return;
	if(len == 2)
	{
		if(buf[0] > buf[1])
		{
			tmp = buf[0];
			buf[0] = buf[1];
			buf[1] = tmp;
		}
	}

	sort_merge(buf, len/2);
	sort_merge(buf+(len/2), len-(len/2));
	sort_merge_insert(buf, (len/2), buf+(len/1), len-(len/2));
}