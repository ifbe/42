#include<actor.h>




void drawicon_1(struct arena* win, int x0, int y0, int x1, int y1)
{
	u32 color;
	int x,y;
	int width = win->w;
	//int height = win->h;
	u32* p;
	u32* buf = (u32*)(win->buf);

	for(y=y0+16;y<y1-16;y++)
	{
		for(x=x0+16;x<x1-16;x++)
		{
			buf[y*width + x]=0xfff0f0f0;
		}
	}

	for(y=0;y<16;y++)
	{
		color = 0xff404040+(0x0b0b0b*y);

		p = buf + (y+y0)*width;
		for(x=x0+y;x<x1-y;x++)p[x]=color;

		p = buf + (y1-1-y)*width;
		for(x=x0+y;x<x1-y;x++)p[x]=color;
	}

	for(x=0;x<16;x++)
	{
		color = 0xff404040+(0x0b0b0b*x);

		for(y=y0+x;y<y1-x;y++)
		{
			p = buf + y*width;
			p[x0+x]=color;
			p[x1-1-x]=color;
		}
	}
}




void drawicon_2(struct arena* win, int x0, int y0, int x1, int y1)
{
}