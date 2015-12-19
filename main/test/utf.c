#include<stdio.h>
int unicode2utf(unsigned int,unsigned char*);
int utf2unicode(unsigned char*,unsigned int*);


void main()
{
	int i=0,j=0;
	int count=0;
	unsigned char temp[10];
	unsigned char p[100]={"从前有座山，山里有座庙，庙里有个小和尚。没"};
	unsigned int unicode;

	while(1)
	{
		//utf2unicode
		i=utf2unicode(p+count,&unicode);
		if(i<=1)break;

		//utf8
		for(j=0;j<i;j++)
		{
			printf("%x",p[count+j]);
		}

		count+=i;
		if(count>=90)break;

		//unicode
		printf("	%x	",unicode);

		//unicode2utf
		i=unicode2utf(unicode,temp);
		temp[i]=0;

		//chinese
		printf("%s\n",temp);
	}

}
