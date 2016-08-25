//martin buttner
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void gray_b(u64 size,u8* addr);
void gray_g(u64 size,u8* addr);
void gray_r(u64 size,u8* addr);
void gray_max(u64 size,u8* addr);
void gray_average(u64 size,u8* addr);
void gray_opencv(u64 size,u8* addr);
void gray_bio(u64 size,u8* addr);
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
static u32 palette[1024*1024];
static u32 final[1024*1024];




//
#define _sq(x) ((x)*(x))
#define r(n)(rand()%n)
unsigned char RED(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):RED((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char GREEN(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):GREEN((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char BLUE(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):BLUE((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}




//
void processmessage(u64 type,u64 key)
{
	
	int i;
	printf("%llx,%llx\n",type,key);

	if(type==0x72616863)             //'char'
	{
		//gray
		for(i=0;i<1024*1024;i++)
		{
			final[i]=palette[i];
		}

		//0
		if(key==0x30)
		{
		}
		//1
		else if(key==0x31)
		{
			gray_b(0x04000400,(u8*)final);
		}
		//2
		else if(key==0x32)
		{
			gray_g(0x04000400,(u8*)final);
		}
		//3
		else if(key==0x33)
		{
			gray_r(0x04000400,(u8*)final);
		}
		//4
		else if(key==0x34)
		{
			gray_max(0x04000400,(u8*)final);
		}
		//5
		else if(key==0x35)
		{
			gray_average(0x04000400,(u8*)final);
		}
		//6
		else if(key==0x36)
		{
			gray_opencv(0x04000400,(u8*)final);
		}
		//7
		else if(key==0x37)
		{
			gray_bio(0x04000400,(u8*)final);
		}
	}
}
void main()
{
	//before
	int x,y;
	int r,g,b;

	//
	windowcreate();
	windowstart(final, "rgba8888", 1024, 1024);

	//picture
	for(y=0;y<1024;y++)
	{
		for(x=0;x<1024;x++)
		{
			r=RED(x,y)&0xff;
			g=GREEN(x,y)&0xff;
			b=BLUE(x,y)&0xff;
			palette[y*1024+x]=(b)+(g<<8)+(r<<16);
		}
	}
	processmessage(0x72616863,0x30);

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

		//3.处理事件
		processmessage(type,key);
	}

	//after
	windowstop();
	windowdelete();
}
