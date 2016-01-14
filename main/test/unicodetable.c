#include<stdio.h>
unsigned char* whereisunicodetable();
void printmemory(char*,int);




void printunicode(char* p)
{
	int j,k;
	unsigned short temp;
	for(j=0;j<0x10;j++)
	{
		temp  = p[j*2] << 8;
		temp += p[(j*2)+1];

		for(k=0;k<0x10;k++)
		{
			if( (temp&0x8000) == 0x8000 )printf("* ");
			else printf("  ");

			temp=temp<<1;
		}

		printf("\n");
	}
}
int main()
{
	int i;
	unsigned char* p;

	initwindow();
	p=whereisunicodetable();
	if(p==0)
	{
		printf("unicode table not found\n");
		return;
	}

	printunicode(p+(0x6709*0x20));		//有
	printunicode(p+(0x6ca1*0x20));		//没

	killwindow();
	return 0;
}
