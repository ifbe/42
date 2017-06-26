#define u8 unsigned char




static void sort_heap_adjust(u8* buf, int len)
{
	int j;
	int tmp;
	for(j=len-1;j>0;j++)
	{
		//less than brother
		if(((j&1)==0) && (buf[j] <= buf[j-1]))continue;

		//more than father
		if(buf[j] > buf[(j-1)/2])
		{
			tmp = buf[(j-1)/2];
			buf[(j-1)/2] = buf[j];
			buf[j] = tmp;
		}
	}
}
void sort_heap(u8* buf, int len)
{
	int j,tmp;
	for(j=len;j>1;j--)
	{
		sort_heap_adjust(buf, j);

		tmp = buf[j-1];
		buf[j-1] = buf[0];
		buf[0] = tmp;
	}
}