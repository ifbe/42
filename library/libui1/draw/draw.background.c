#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




static struct temp{
        QWORD type;
        QWORD id;
        QWORD start;
        QWORD end;

        QWORD pixelbuffer;
        QWORD pixelformat;
        QWORD width;
        QWORD height;
}*haha;




void backgroundcolor(unsigned int color)
{
	int x;
	int width,height;
	DWORD* screenbuf=(DWORD*)(haha->pixelbuffer);

	color |= 0xff000000;
	for(x=0; x<(haha->width)*(haha->height); x++)
	{
		screenbuf[x]=color;
	}
}
void background1()
{
	int x,y;
	int width,height;
	DWORD* screenbuf;

	width = haha->width;
	height = haha->height;
	screenbuf = (DWORD*)(haha->pixelbuffer);

	//用指定颜色清屏
	for(x=0;x<width*height;x++)
	{
		screenbuf[x]=0xfff0f0f0;
	}

	//上下
	for(y=0;y<16;y++)
	{
		DWORD color=0xff404040+(0x0b0b0b*y);

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
		DWORD color=0xff404040+(0x0b0b0b*x);

		for(y=x;y<height-x;y++)
		{
			screenbuf[(y*width)+x]=color;
			screenbuf[(y*width)+width-1-x]=color;
		}
	}
}




void background_start(DWORD size,char* addr)
{
}
void background_stop()
{
}
void background_init(void* home,void* me)
{
	haha = me;
	haha->type = 0;
	haha->id = 0x6762;
}
void background_kill()
{
}
