#define u8 unsigned char




int count_u8(u8* buf, int len, u8 ch)
{
	int j,k=0;
	for(j=0;j<len;j++)
	{
		if(buf[j] == ch)k++;
	}
	return k;
}


int count_item(u8* buf, int len)
{
	int j,k=0,flag=0;
	for(j=0;j<len;j++)
	{
		if(buf[j] == 0)break;

		if((buf[j] == 0x9)|(buf[j] == 0x20))
		{
			if(flag == 0)flag = 1;
			else continue;
		}
		else
		{
			if(flag == 0)continue;
			else
			{
				k++;
				flag = 0;
			}
		}
	}
	return k;
}


int count_strlen(u8* buf)
{
	int j=0;
	while(1)
	{
		if(buf[j] == 0)break;
		j++;
	}
	return j;
}