#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




void backgroundcolor(u32* screenbuf, u64 fmt, int width, int height,
	unsigned int color)
{
	int x;
	color |= 0xff000000;
	for(x=0; x<width*height; x++)
	{
		screenbuf[x]=color;
	}
}
void background1(u32* screenbuf, u64 fmt, int width, int height)
{
	int x,y;

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
