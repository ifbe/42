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




int wildcard(char* first,char* second)
{
	int j=0;
	int k=0;
	//printf("%s,%s\n",first,second);

	while(1)
	{
		if(first[j]==second[k])
		{
			if(first[j]==0)
			{
				return 1;
			}
			else
			{
				j++;
				k++;
				continue;
			}
		}

		if( (first[j]=='?') | (second[k]=='?') )
		{
			j++;
			k++;
			continue;
		}

		else if(first[j]=='*')
		{
			while(first[j]=='*')j++;
			if(first[j]==0)
			{
				return 1;
			}

			while(1)
			{
				if(second[k]==0)return 0;

				if(first[j] == second[k])
				{
					if(wildcard(first+j+1 , second+k+1) != 0)
					{
						return 1;
					}
				}
				k++;
			}
		}

		else if(second[k]=='*')
		{
			while(second[k]=='*')k++;
			if(second[k]==0)return 1;

			while(1)
			{
				if(first[j]==0)return 0;

				if(first[j] == second[k])
				{
					if(wildcard(first+j+1,second+k+1) != 0)
					{
						return 1;
					}
				}
				j++;
			}
		}

		else return 0;
	}
	return 1;
}
