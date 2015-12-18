#include<stdio.h>
int unicode2utf(unsigned int,unsigned char*);
int utf2unicode(unsigned char*,unsigned int*);


void main()
{
	int i=0,count=0;
	char p[100];
	unsigned int unicode;
	scanf("%s",p);

	while((i!=1)&&(count<90))
	{
		i=utf2unicode(p+count,&unicode);
		count+=i;
		printf("%x\n",unicode);
	}
}
