#include<stdio.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void windowcreate();
void windowdelete();
void windowstart(void* addr,void* pixfmt, int width,int height);
void windowstop();
void windowread();
void windowwrite();
//
void eventwrite();
void eventread(u64* first,u64* second);
//
unsigned char* whereisunicodetable();
void printmemory(char*,int);




static u32 final[1024*1024];
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
void main()
{
	int i;
	unsigned char* p;

	//
	windowcreate();
	windowstart(final, "rgba8888", 1024, 1024);

	//
	p=whereisunicodetable();
	if(p==0)
	{
		printf("unicode table not found\n");
		goto bye;
	}

	printunicode(p+(0x6709*0x20));		//有
	printunicode(p+(0x6ca1*0x20));		//没

	//forever
	u64 type=0;
	u64 key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		windowwrite();

		//2.等事件，是退出消息就退出
		eventread(&type,&key);
		if( type==0 )break;
		if( (type==0x64626b)&&(key==0x1b))break;
	}

bye:
	//after
	windowstop();
	windowdelete();
}
