#define u8 unsigned char




//address
//length
//celltype(char?short?int?)
//direction(small->big, big->small, ...)
void sort_bubble(u8* buf, int len)
{
	int j,k;
	int tmp;
	for(j=0;j<len-1;j++)
	{
		for(k=0;k<len-1;k++)
		{
			if(buf[k] > buf[k+1])
			{
				tmp = buf[k];
				buf[k] = buf[k+1];
				buf[k+1] = tmp;
			}
		}
	}
}
