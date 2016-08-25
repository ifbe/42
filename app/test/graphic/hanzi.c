#include<stdio.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int createunicode(u64,void*);
void printunicode(int,int,u32);
void printunicodebig(int,int,u32);
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




static u32 palette[1024*1024];
static u32 which=0x4e00;		//hanzi starts @ 0x4e00



void printworld()
{
	int x,y;
	for(y=0;y<1024;y++)
	{
		for(x=0;x<1024;x++)
		{
			palette[y*1024+x]=0x44444444;
		}
	}
/*
	printunicode(0,0,0x548c);
	printunicode(500,0,0x548c);
	printunicode(1000,0,0x548c);
	printunicode(0,250,0x548c);
	printunicode(0,500,0x548c);
*/
	for(y=0;y<1024;y+=32)
	{
		for(x=0;x<1024;x+=32)
		{
			printunicode(x,y,which + y + (x/32) );
		}
	}

	//
	printunicodebig(0,0,which);

}
void processmessage(u64 type,u64 key)
{
	if(type==0x6E6F7266207A7978)             //'xyz fron'
	{
		if(which>=0x40)which-=0x20;
	}
	else if(type==0x6B636162207A7978)        //'xyz back'
	{
		if(which<0xf000)which+=0x20;
	}
}
void main()
{
	//int ret=createunicode(0x04000400,palette);

	//
	windowcreate();
	windowstart(palette, "rgba8888", 1024, 1024);

	//forever
	u64 type=0;
	u64 key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		printworld();
		windowwrite();

		//2.等事件，是退出消息就退出
		eventread(&type,&key);
		if( type==0 )break;
		if( (type==0x64626b)&&(key==0x1b))break;


		//3.处理事件，如果要求自杀就让它死
		processmessage(type,key);
	}

	//after
death:
	windowstop();
	windowdelete();
}
