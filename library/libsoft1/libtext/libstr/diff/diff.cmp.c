#define u8 unsigned char
void logtoall(u8* fmt , ...);




int cmp(u8* first, u8* second)
{
	int i;
	int ret=1;	//default different
	//logtoall("%s,%s\n",first,second);

	i=0;
	while(1)
	{
		//logtoall("%c,%c\n",first[i],second[i]);
		ret=first[i]-second[i];

		if( ret != 0 )break;
		else if(first[i]==0)break;

		i++;
	}
	return ret;
}
int ncmp(u8* first, u8* second, int count)
{
	int i;
	int ret=1;	//default different
	//logtoall("%s,%s\n",first,second);

	for(i=0;i<count;i++)
	{
		//logtoall("%c,%c\n",first[i],second[i]);
		ret=first[i]-second[i];

		if( ret != 0 )break;
		else if(first[i]==0)break;
	}
	return ret;
}
