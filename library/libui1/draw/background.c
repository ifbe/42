#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void blackstring(int x,int y,char* str);
void line(QWORD,QWORD,QWORD);
void rectangle(QWORD,QWORD,QWORD);




static DWORD* screenbuf=0;
static int width=0;
static int height=0;




void backgroundcolor(unsigned int color)
{
	int x;

	for(x=0; x<width*height; x++)
	{
		screenbuf[x]=color;
	}
}
void background1()
{
	QWORD x,y;

	//用指定颜色清屏
	for(x=0;x<width*height;x++)
	{
		screenbuf[x]=0xf0f0f0f0;
	}

	//上下
	for(y=0;y<16;y++)
	{
		DWORD color=0x40404040+(0x0b0b0b0b*y);

		//上，编译器会不会用rep stosd指令优化呢?
		DWORD* p=screenbuf+y*width;
		for(x=y;x<width-y;x++)p[x]=color;

		//下
		p=screenbuf+(height-1-y)*width;
		for(x=y;x<width-y;x++)p[x]=color;
	}
	//左右
	for(x=0;x<16;x++)
	{
		DWORD color=0x40404040+(0x0b0b0b0b*x);

		for(y=x;y<height-x;y++)
		{
			screenbuf[(y*width)+x]=color;
			screenbuf[(y*width)+width-1-x]=color;
		}
	}
}




void initbackground(QWORD size,void* addr)
{
	//how
	width=size&0xffff;
	height=(size>>16)&0xffff;

	//where
	screenbuf=addr;
}
void killbackground()
{
}
