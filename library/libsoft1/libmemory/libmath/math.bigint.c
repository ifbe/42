int bigadd(u8* abuf, int alen, u8* bbuf, int blen, u8* answer, int max)
{
	int j;
	int temp = 0;
	if(alen >= blen)
	{
		for(j=0;j<blen;j++)
		{
			temp += abuf[j] + bbuf[j];
			answer[j] = temp & 0xff;
			temp >>= 8;
		}
		for(;j<alen;j++)
		{
			if(temp == 0)break;
		 
			temp += abuf[j]; 
			answer[j] = temp & 0xff;
			temp >>= 8;
		}
		if(temp != 0)
		{
			answer[j] = 1;
			j++;
		}
	}
	else    //alen<blen
	{
		for(j=0;j<alen;j++)
		{
			temp += abuf[j] + bbuf[j];
			answer[j] = temp & 0xff;
			temp >>= 8;
		}
		for(;j<blen;j++)
		{
			temp += bbuf[j]; 
			answer[j] = temp & 0xff;
			temp >>= 8;
		}
		if(temp != 0)
		{
			answer[j] = 1;
			j++;
		}
	}
	return j;
}
int bigsub(u8* abuf, int alen, u8* bbuf, int blen, u8* answer, int max)
{
	int j;
	int temp = 0;
	if(alen >= blen)
	{
		for(j=0;j<blen;j++)
		{
			temp = abuf[j] - bbuf[j] - temp;
			if(temp >= 0)
			{
				answer[j] = temp;
				temp = 0;
			}
			else
			{
				answer[j] = temp + 256;
				temp = 1;
			}
		}
		for(;j<alen;j++)
		{
			temp = abuf[j] - temp;
			if(temp >= 0)
			{
				answer[j] = temp;
				break;
			}
			else
			{
				answer[j] = temp + 256;
				temp = 1;
			}
		}
		if(temp != 0)
		{
			answer[j] = 0xff;
			j++;
		}
	}
	else    //alen<blen
	{
		for(j=0;j<alen;j++)
		{
			temp = abuf[j] -bbuf[j] - temp;
			if(temp >= 0)
			{
				answer[j] = temp;
				temp = 0;
			}
			else
			{
				answer[j] = temp + 256;
				temp = 1;
			}
		}
		for(;j<=blen;j++)
		{
			temp = -bbuf[j] - temp;
			if(temp >= 0)
			{
				answer[j] = 0;
				break;
			}
			else
			{
				answer[j] = 0xff;
				temp = 1;
			}
		}
	}
	return j;
}
int bigmul(u8* abuf, int alen, u8* bbuf, int blen, u8* answer, int max)
{
	return 0;
}
int bigdiv(u8* abuf, int alen, u8* bbuf, int blen, u8* answer, int max)
{
	return 0;
}
