#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void blackstring(int x,int y,char* str);
void line(QWORD,QWORD,QWORD);
void rectangle(QWORD,QWORD,QWORD);
QWORD windowresolution();




static DWORD* screenbuf=0;
void initbackground(char* addr)
{
	screenbuf=(DWORD*)addr;
}




void cleanscreen()
{

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*768;x++)
	{
		screenbuf[x]=0;
	}
}




void background1()
{
	QWORD x,y;
	QWORD temp=windowresolution();
	int ysize=(temp>>16)&0xffff;
	int xsize=temp&0xffff;
	if(xsize>1024)xsize=1024;
	if(ysize>1024)ysize=1024;

	//用指定颜色清屏
	for(x=0;x<1024*ysize;x++)
	{
		screenbuf[x]=0xf0f0f0f0;
	}

	//上下
	for(y=0;y<16;y++)
	{
		DWORD color=0x40404040+(0x0b0b0b0b*y);

		//上，编译器会不会用rep stosd指令优化呢?
		DWORD* p=screenbuf+y*1024;
		for(x=y;x<1024-y;x++)p[x]=color;

		//下
		p=screenbuf+(ysize-1-y)*1024;
		for(x=y;x<1024-y;x++)p[x]=color;
	}
	//左右
	for(x=0;x<16;x++)
	{
		DWORD color=0x40404040+(0x0b0b0b0b*x);

		for(y=x;y<ysize-x;y++)
		{
			screenbuf[(y*1024)+x]=color;
			screenbuf[(y*1024)+xsize-1-x]=color;
		}
	}
}




void background3()
{
	int x,y;

	for(x=0;x<1024*768;x++)
	{
		screenbuf[x]=0;
	}
}




void background2()
{

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*768;x++)
	{
		screenbuf[x]=0;
	}
}




void background4(QWORD showaddr,QWORD temp)
{
	//用指定颜色清屏
	DWORD x,y,color;

	for(x=0;x<1024*752;x++)
	{
		screenbuf[x]=0;
	}

	for(y=768-16;y<768;y++)
	{
		for(x=0;x<512;x++)
		{
			color=(x/2)*0x01010101;
			screenbuf[(y*1024) + x]=color;
			screenbuf[(y*1024) + 1023-x]=color;
		}
	}

	for(y=0;y<384;y++)
	{
		color = y*0xff/384;//0x44444488;

		for(x=1024-16;x<1024;x++)
		{
			screenbuf[(y*1024) + x] = color;
		}
		for(x=1024-16;x<1024;x++)
		{
			screenbuf[((767-y)*1024) + x] = color;
		}
	}

	//位置
	if(temp>=0x80*40)
	{
		QWORD end=64+(640-64*2)*showaddr/temp;			//temp变量=max
		QWORD start=end-(640-64*2)*0x80*40/temp;
		for(y=start;y<end;y++)
		{
			for(x=1024-16+4;x<1024-4;x++)
			{
				screenbuf[(y*1024) + x] = 0x01234567;
			}
		}
	}
}
