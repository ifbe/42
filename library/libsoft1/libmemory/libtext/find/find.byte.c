int findzero(char* p)
{
	int j=0;
	while(1)
	{
		if(p[j] == 0)break;
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
