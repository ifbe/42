#define u8 unsigned char




void sort_insert(u8* buf, int len)
{
	int j, tmp;
	int last, defier;

	last = 0;
	for(defier=1;defier<len;defier++)
	{
		for(j=0;j<=last;j++)
		{
			tmp = buf[defier];
			if(tmp >= buf[j])continue;

			for(j=last;j>=0;j--)buf[j+1] = buf[j];
			buf[0] = tmp;
			break;
		}
		last++;
	}
}