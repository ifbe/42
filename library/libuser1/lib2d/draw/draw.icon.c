#include<actor.h>
u32 getrandom();




void drawicon_1(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1, u8* buf, int len)
{
	u32 r,g,b,a;
	int x,y,m,n;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* fb = (u32*)(win->buf);

	for(y=y0;y<y1;y++)
	{
		if(y < 0)continue;
		if(y >= height)continue;
		for(x=x0;x<x1;x++)
		{
			if(x < 0)continue;
			if(x >= width)continue;
/*
			if(x-x0+y-y0 < 4)continue;
			else if(x-x0+y1-y < 4)continue;
			else if(x1-x+y-y0 < 4)continue;
			else if(x1-x+y1-y < 4)continue;
*/
			m = fb[y*stride+x];
			a = (rgb>>24)&0xff;

			r = (m&0xff)*(0x100-a) + 0xff*a;
			r = (r>>8)&0xff;

			g = ((m>> 8)&0xff)*(0x100-a) + 0xff*a;
			g = (g>>8)&0xff;

			b = ((m>>16)&0xff)*(0x100-a) + 0xff*a;
			b = (b>>8)&0xff;

			m = r | (g<<8) | (b<<16) | 0xff000000;
			fb[y*stride+x] = m;
		}
	}

	if(buf == 0)return;
	if(len == 0)
	{
		while(buf[len] > 0x20)len++;
	}
	else
	{
		for(x=0;x<len;x++)
		{
			if(buf[x] < 0x20){len = x;break;}
		}
	}
	if(len == 0)return;

	x = (x0+x1)/2 - len*4;
	y = (y0+y1)/2 - 8;
	drawstring(win, rgb, x, y, buf, len);
}
void drawicon_normal(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1, u8* buf, int len)
{
}
void drawicon_focused(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1, u8* buf, int len)
{
}
void drawicon_pressed(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1, u8* buf, int len)
{
}
void drawicon_disabled(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1, u8* buf, int len)
{
}
