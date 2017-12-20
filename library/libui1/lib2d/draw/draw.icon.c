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
			fb[y*width + x] &= 0xfefefefe;
			fb[y*width + x] >>= 1;
			fb[y*width + x] |= 0x80000000;
			fb[y*width + x] += 0x808080;
		}
	}

	for(y=0;y<8;y++)
	{
		color = 0x101010*y;

		p = fb + (y+y0)*width;
		for(x=x0+y;x<x1-y;x++)
		{
			p[x] &= 0xfefefefe;
			p[x] >>= 1;
			p[x] |= 0x80000000;
			p[x] += color;
		}

		p = fb + (y1-1-y)*width;
		for(x=x0+y;x<x1-y;x++)
		{
			p[x] &= 0xfefefefe;
			p[x] >>= 1;
			p[x] |= 0x80000000;
			p[x] += color;
		}
	}

	for(x=0;x<8;x++)
	{
		color = 0x101010*x;

		for(y=y0+x;y<y1-x;y++)
		{
			p = fb + y*width;

			p[x0+x] &= 0xfefefefe;
			p[x0+x] >>= 1;
			p[x0+x] |= 0x80000000;
			p[x0+x] += color;

			p[x1-1-x] &= 0xfefefefe;
			p[x1-1-x] >>= 1;
			p[x1-1-x] |= 0x80000000;
			p[x1-1-x] += color;
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
