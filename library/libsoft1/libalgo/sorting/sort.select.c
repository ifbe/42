#define u8 unsigned char




void sort_select(u8* buf, int len)
{
	int j, k;
	int min, tmp;
	for(j=0;j<len-1;j++)
	{
		min = j;
		for(k=j;k<len-1;k++)
		{
			if(buf[k] < buf[min])min=k;
		}

		if(j != min)
		{
			tmp = buf[j];
			buf[j] = buf[min];
			buf[min] = tmp;
		}
	}
}