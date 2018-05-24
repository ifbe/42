#include "actor.h"
#define halfsqrt3 0.8660254037844386




void drawline(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1)
{
	int dx,dy,sx,sy,e1,e2;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	if(x0 < x1){dx = x1-x0;sx = 1;}
	else {dx = x0-x1;sx = -1;}
	if(y0 < y1){dy = y1-y0;sy = 1;}
	else {dy = y0-y1;sy = -1;}
	if(dx > dy){e1 = dx/2;}
	else {e1 = -dy/2;}

	rgb |= 0xff000000;
	while(1)
	{
		if((x0 == x1)&&(y0 == y1))break;

		if((x0 >= 0)&&(x0 < width)&&(y0 >= 0)&&(y0 < height))
		{buf[(y0*stride) + x0] = rgb;}

		e2 = e1;
		if(e2 >-dx){e1 -= dy;x0 += sx;}
		if(e2 < dy){e1 += dx;y0 += sy;}
	}
}
void drawline_arrow(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1)
{
	float x,y,a,b;
	drawline(win, rgb, x0, y0, x1, y1);

	if((x0==x1)&&(y0==y1))return;
	x = (float)(x0-x1);
	y = (float)(y0-y1);

	a = squareroot(x*x+y*y);
	x = 16*x/a;
	y = 16*y/a;

	a = x1 + halfsqrt3*x + 0.5*y;
	b = y1 - 0.5*x + halfsqrt3*y;
	drawline(win, rgb, x1, y1, (int)a, (int)b);

	a = x1 + halfsqrt3*x - 0.5*y;
	b = y1 + 0.5*x + halfsqrt3*y;
	drawline(win, rgb, x1, y1, (int)a, (int)b);
}
void drawbezier(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2, int xc, int yc)
{
	int x,y,t;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	for(t=0;t<1000;t++)
	{
		x = (1000-t)*(1000-t)*x1 + 2*t*(1000-t)*xc + t*t*x2;
		x /= 1000*1000;
		if(x<0|x>=width)continue;

		y = (1000-t)*(1000-t)*y1 + 2*t*(1000-t)*yc + t*t*y2;
		y /= 1000*1000;
		if(y<0|y>=height)continue;

		buf[y*stride + x] = rgb;
	}
}




void drawline_triangle(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2, int x3, int y3)
{
	drawline(win, rgb, x1, y1, x2, y2);
	drawline(win, rgb, x1, y1, x3, y3);
	drawline(win, rgb, x2, y2, x3, y3);
}
void drawline_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2)
{
	int x,y,n;
	int startx,endx,starty,endy;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	if(x1<x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(y1<y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	//say("(%x,%x),(%x,%x)\n",startx,starty,endx,endy);

	rgb |= 0xff000000;
	for(n=0;n<2;n++)
	{
		for(x=startx;x<endx;x++)
		{
			if(starty < 0)break;
			if(starty >= width)break;
			if(x > width-1)break;
			if(x < 0)x=0;
			buf[((starty+n)*stride) + x] = rgb;
		}
		for(x=startx;x<endx;x++)
		{
			if(endy < n)break;
			if(endy > height-1)break;
			if(x > width-1)break;
			if(x < 0)x=0;
			buf[((endy-n)*stride) + x] = rgb;
		}
		for(y=starty;y<endy;y++)
		{
			if(startx < 0)break;
			if(startx >= width)break;
			if(y > height-1)break;
			if(y < 0)y = 0;
			buf[(y*stride) + startx+n] = rgb;
		}
		for(y=starty;y<endy;y++)
		{
			if(endx < n)break;
			if(endx > width-1)break;
			if(y > height-1)break;
			if(y < 0)y = 0;
			buf[(y*stride) + endx-n] = rgb;
		}
	}
}
void drawline_circle(struct arena* win, u32 rgb,
	int cx, int cy, int radius)
{
	int x, y, ret;
	int x1, y1, x2, y2;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	y1=cy-radius;
	if(y1<0)y1=0;
	if(y1>=height)y1=height-1;

	y2=cy+radius;
	if(y2<0)y2=0;
	if(y2>=height)y2=height-1;

	rgb |= 0xff000000;
	for(y=y1;y<=y2;y++)
	{
		ret = (int)squareroot(radius*radius - (y-cy)*(y-cy));

		x1 = cx - ret;
		if(x1<0)x1=0;
		if(x1>=width)x1=width-1;

		x2 = cx + ret;
		if(x2<0)x2=0;
		if(x2>=width)x2=width-1;

		buf[ (y*stride) + x1 ] = rgb;
		buf[ (y*stride) + x2 ] = rgb;
	}
}
void drawline_oval(struct arena* win, u32 rgb,
	int cx, int cy, int rx, int ry, int fx, int fy)
{
}
void drawline_sector(struct arena* win, u32 rgb,
	int cx, int cy, int radius, int start, int end)
{
}