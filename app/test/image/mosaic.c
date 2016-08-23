//martin buttner
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void mosaic(QWORD size,QWORD partradius,BYTE* src,BYTE* dst);
//
void windowcreate();
void windowdelete();
void windowstart(void* addr,void* pixfmt, int width,int height);
void windowstop();
void windowread();
void windowwrite();
//
void eventwrite();
void eventread(QWORD* first,QWORD* second);




//
static DWORD palette[1024*1024];
static DWORD final[1024*1024];




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
void processmessage(QWORD type,QWORD key)
{
	
	int i;
	printf("%llx,%llx\n",type,key);

	if(type==0x72616863)             //'char'
	{
		//0
		if(key==0x30)
		{
			for(i=0;i<1024*1024;i++)
			{
				final[i]=palette[i];
			}
		}
		//1
		else if(key==0x31)
		{
			mosaic(0x04000400,0,(BYTE*)palette,(BYTE*)final);
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
	QWORD type=0;
	QWORD key=0;
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
