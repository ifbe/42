void say(char* fmt , ...);
int compare(unsigned char* first,unsigned char* second)
{
	int i;
	int ret=1;	//default different
	//say("%s,%s\n",first,second);

	for(i=0;i<16;i++)
	{
		//say("%c,%c\n",first[i],second[i]);
		ret=first[i]-second[i];

		if( ret != 0 )break;
		else if(first[i]==0)break;
	}
	return ret;
}
