#include<actor.h>




void backgroundcolor(struct arena* win, u32 color)
{
	int x;
	u32* buf = (u32*)(win->buf);
	color |= 0xff000000;

	for(x=0; x<(win->w)*(win->h); x++)
	{
		buf[x] = color;
	}
}
void background1(struct arena* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;
	u32* buf = (u32*)(win->buf);

	//用指定颜色清屏
	for(x=0;x<width*height;x++)
	{
		buf[x]=0xfff0f0f0;
	}

	//上下
	for(y=0;y<16;y++)
	{
		u32 color=0xff404040+(0x0b0b0b*y);

		//上，编译器会不会用rep stosd指令优化呢?
		u32* p = buf+y*width;
		for(x=y;x<width-y;x++)p[x]=color;

		//下
		p = buf+(height-1-y)*width;
		for(x=y;x<width-y;x++)p[x]=color;
	}
	//左右
	for(x=0;x<16;x++)
	{
		u32 color=0xff404040+(0x0b0b0b*x);

		for(y=x;y<height-x;y++)
		{
			buf[(y*width)+x]=color;
			buf[(y*width)+width-1-x]=color;
		}
	}
}
