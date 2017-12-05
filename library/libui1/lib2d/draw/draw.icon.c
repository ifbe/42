#include<actor.h>




void drawicon_1(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1,
	u8* buf, int len)
{
	u32 color;
	int x,y,j;
	int width = win->w;
	//int height = win->h;
	u32* p;
	u32* fb = (u32*)(win->buf);

	for(y=y0+8;y<y1-8;y++)
	{
		for(x=x0+8;x<x1-8;x++)
		{
			fb[y*width + x]=0xfff0f0f0;
		}
	}

	for(y=0;y<8;y++)
	{
		color = 0xff404040+(0x171717*y);

		p = fb + (y+y0)*width;
		for(x=x0+y;x<x1-y;x++)p[x]=color;

		p = fb + (y1-1-y)*width;
		for(x=x0+y;x<x1-y;x++)p[x]=color;
	}

	for(x=0;x<8;x++)
	{
		color = 0xff404040+(0x171717*x);

		for(y=y0+x;y<y1-x;y++)
		{
			p = fb + y*width;
			p[x0+x]=color;
			p[x1-1-x]=color;
		}
	}

	if(buf == 0)return;
	if(len == 0)
	{
		while(buf[len] > 0x20)len++;
	}
	else
	{
		for(j=0;j<len;j++)
		{
			if(buf[j] < 0x20){len = j;break;}
		}
	}
	if(len == 0)return;

	x = (x0+x1)/2 - len*4;
	y = (y0+y1)/2 - 4;
	drawstring(win, rgb, x, y, buf, len);
}




void drawicon_2(struct arena* win, int x0, int y0, int x1, int y1)
{
}