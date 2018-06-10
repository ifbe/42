#define u8 unsigned char
void say(u8* fmt , ...);




int cmp(u8* first, u8* second)
{
	int i;
	int ret=1;	//default different
	//say("%s,%s\n",first,second);

	i=0;
	while(1)
	{
		//say("%c,%c\n",first[i],second[i]);
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
	//say("%s,%s\n",first,second);

	for(i=0;i<count;i++)
	{
		//say("%c,%c\n",first[i],second[i]);
		ret=first[i]-second[i];

		if( ret != 0 )break;
		else if(first[i]==0)break;
	}
	return ret;
}
