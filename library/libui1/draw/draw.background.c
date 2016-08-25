#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




static struct temp{
        u64 type;
        u64 id;
        u64 start;
        u64 end;

        u64 pixelbuffer;
        u64 pixelformat;
        u64 width;
        u64 height;
}*haha;




void backgroundcolor(unsigned int color)
{
	int x;
	int width,height;
	u32* screenbuf=(u32*)(haha->pixelbuffer);

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
	u32* screenbuf;

	width = haha->width;
	height = haha->height;
	screenbuf = (u32*)(haha->pixelbuffer);

	//用指定颜色清屏
	for(x=0;x<width*height;x++)
	{
		screenbuf[x]=0xfff0f0f0;
	}

	//上下
	for(y=0;y<16;y++)
	{
		u32 color=0xff404040+(0x0b0b0b*y);

		//上，编译器会不会用rep stosd指令优化呢?
		u32* p=screenbuf+y*width;
		for(x=y;x<width-y;x++)p[x]=color;

		//下
		p=screenbuf+(height-1-y)*width;
		for(x=y;x<width-y;x++)p[x]=color;
	}
	//左右
	for(x=0;x<16;x++)
	{
		u32 color=0xff404040+(0x0b0b0b*x);

		for(y=x;y<height-x;y++)
		{
			screenbuf[(y*width)+x]=color;
			screenbuf[(y*width)+width-1-x]=color;
		}
	}
}




void background_start()
{
}
void background_stop()
{
}
void background_create(void* home,void* me)
{
	haha = me;
	haha->type = 0;
	haha->id = 0x6762;
}
void background_delete()
{
}
