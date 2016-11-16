int findzero(char* p)
{
	int j=0;
	while(1)
	{
		if(p[j] == 0)break;

		j++;
	}
	return j;
}
int findtail(char* p)
{
	int j=0;
	while(1)
	{
		if(p[j] == 0)break;
		if(p[j] == 0xd)break;
		if(p[j] == 0xa)break;

		j++;
	}
	return j;
}
int findhead(char* p)
{
	int j;
	int flag=0;
	while(1)
	{
		if(p[j] == 0)break;
		if(p[j] == 0xa)
		{
			j++;
			break;
		}
		if(p[j] == 0xd)
		{
			if(p[j+1] == 0xa)
			{
				j+=2;
			}
			else j++;

			break;
		}

		j++;
	}
	return j;
}
