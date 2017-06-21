#include "actor.h"




void linesegment(struct arena* win,
	int x1, int y1, int x2, int y2, u32 color)
{
}
void halfline(struct arena* win,
	int x1, int y1, int x2, int y2, u32 color)
{
}
void line(struct arena* win,
	int x1, int y1, int x2, int y2, u32 color)
{
	int temp;
	int x,y;
	int width,height;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;
	color |= 0xff000000;

	if(x1<0)x1=0;
	if(x1>width)x1=width-1;
	if(x2<0)x2=0;
	if(x2>width)x2=width-1;
	if(y1<0)y1=0;
	if(y1>=height)y1=height-1;
	if(y2<0)y2=0;
	if(y2>=height)y2=height-1;
//say("(%d,%d)->(%d,%d)\n",x1,y1,x2,y2);




	if(x1 == x2)
	{
		if(y1<=y2){y=y1;temp=y2;}
		else{y=y2;temp=y1;}

		for(;y<=temp;y++)
		{
			buf[ (y*width) + x1 ] = color;
		}
	}

	else
	{
		double dx;
		double k=(double)(y1-y2) / (double)(x1-x2);
		if(x1<x2){x=x1;temp=x2;}
		else{x=x2;temp=x1;}

		for(;x<temp;x++)
		{
			dx=(double)(x-x1);
			y=y1+ (int)(k*dx);
//say("%d,%d\n",x,y);
			if(y<height)
			{
				buf[ (y*width) + x ] = color;
			}
			else say("wrong\n");
		}
	}
}




void rectframe(struct arena* win,
	int x1, int y1, int x2, int y2, u32 color)
{
	int x,y,n;
	int width,height;
	int startx,endx,starty,endy;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;
	color |= 0xff000000;

	if(x1<x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(y1<y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	//say("(%x,%x),(%x,%x)\n",startx,starty,endx,endy);

	for(n=0;n<2;n++)
	{
		for(x=startx;x<endx;x++)
		{
			if(starty < 0)break;
			if(starty >= width)break;
			if(x > width-1)break;
			if(x < 0)x=0;
			buf[((starty+n)*width) + x] = color;
		}
		for(x=startx;x<endx;x++)
		{
			if(endy < n)break;
			if(endy > height-1)break;
			if(x > width-1)break;
			if(x < 0)x=0;
			buf[((endy-n)*width) + x] = color;
		}
		for(y=starty;y<endy;y++)
		{
			if(startx < 0)break;
			if(startx >= width)break;
			if(y > height-1)break;
			if(y < 0)y = 0;
			buf[(y*width) + startx+n] = color;
		}
		for(y=starty;y<endy;y++)
		{
			if(endx < n)break;
			if(endx > width-1)break;
			if(y > height-1)break;
			if(y < 0)y = 0;
			buf[(y*width) + endx-n] = color;
		}
	}
}
void rectbody(struct arena* win,
	int x1, int y1, int x2, int y2, u32 color)
{
	int x,y;
	int width,height;
	int startx,endx,starty,endy;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;
	color |= 0xff000000;

	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(startx < 0)startx = 0;
	if(endx >= width)endx = width-1;

	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	if(starty < 0)starty = 0;
	if(endy >= height)endy = height-1;

	for(y=starty;y<=endy;y++)
	{
		for(x=startx;x<=endx;x++)
		{
			buf[ (y*width) + x ] = color;
		}
	}
}
void rect(struct arena* win,
	int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolor)
{
	rectbody(win, x1, y1, x2, y2, bodycolor);
	rectframe(win, x1, y1, x2, y2, framecolor);
}




void trianglebody(struct arena* win,
	int x1, int y1, int x2, int y2, int x3, int y3, u32 color)
{
}
void triangleframe(struct arena* win,
	int x1, int y1, int x2, int y2, int x3, int y3, u32 color)
{
	line(win,
	x1, y1, x2, y2, color);

	line(win,
	x1, y1, x3, y3, color);

	line(win,
	x2, y2, x3, y3, color);
}
void triangle(struct arena* win,
	int x1, int y1, int x2, int y2, int x3, int y3, u32 bodycolor, u32 framecolor)
{
	trianglebody(win,
	x1, y1, x2, y2, x3, y3, bodycolor);

	triangleframe(win,
	x1, y1, x2, y2, x3, y3, framecolor);
}




void circleframe(struct arena* win,
	int cx, int cy, int radius, u32 color)
{
	int ret;
	int x,y;
	int x1,x2;
	int y1,y2;
	int width,height;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;
	color |= 0xff000000;

	y1=cy-radius;
	if(y1<0)y1=0;
	if(y1>=height)y1=height-1;

	y2=cy+radius;
	if(y2<0)y2=0;
	if(y2>=height)y2=height-1;

	for(y=y1;y<=y2;y++)
	{
		ret = (int)squareroot(radius*radius - (y-cy)*(y-cy));

		x1 = cx - ret;
		if(x1<0)x1=0;
		if(x1>=width)x1=width-1;

		x2 = cx + ret;
		if(x2<0)x2=0;
		if(x2>=width)x2=width-1;

		buf[ (y*width) + x1 ] = color;
		buf[ (y*width) + x2 ] = color;
	}
}
void circlebody(struct arena* win,
	int cx, int cy, int radius, u32 color)
{
	int ret;
	int x,y;
	int x1,x2;
	int y1,y2;
	int width,height;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;
	color |= 0xff000000;

	y1=cy-radius;
	if(y1<0)y1=0;
	if(y1>=height)y1=height-1;

	y2=cy+radius;
	if(y2<0)y2=0;
	if(y2>=height)y2=height-1;

	for(y=y1;y<=y2;y++)
	{
		ret = (int)squareroot(radius*radius - (y-cy)*(y-cy));

		x1 = cx - ret;
		if(x1<0)x1=0;
		if(x1>=width)x1=width-1;

		x2 = cx + ret;
		if(x2<0)x2=0;
		if(x2>=width)x2=width-1;

		for(x=x1;x<=x2;x++)
		{
			buf[ (y*width) + x ] = color;
		}
	}
}




void ovalbody(struct arena* win,
	int x1, int y1, int x2, int y2, u32 color)
{
}
void ovalframe(struct arena* win,
	int x1, int y1, int x2, int y2, u32 color)
{
}
void oval(struct arena* win,
	int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolor)
{
	ovalbody(win,
	x1, y1, x2, y2, bodycolor);

	ovalbody(win,
	x1, y1, x2, y2, framecolor);
}




void sectorbody(struct arena* win,
	int cx, int cy, int radius, int start, int end, u32 color)
{
}
void sectorframe(struct arena* win,
	int cx, int cy, int radius, int start, int end, u32 color)
{
}
void sector(struct arena* win,
	int cx, int cy, int radius, int start, int end, u32 bodycolor, u32 framecolor)
{
}




void bezier(struct arena* win,
	int ax, int ay, int bx, int by, int cx, int cy, u32 color)
{
	int x,y,t;
	int width;
	int height;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;

	for(t=0;t<1000;t++)
	{
		x = (1000-t)*(1000-t)*ax + 2*t*(1000-t)*cx + t*t*bx;
		x /= 1000*1000;
		if(x<0|x>=width)continue;

		y = (1000-t)*(1000-t)*ay + 2*t*(1000-t)*cy + t*t*by;
		y /= 1000*1000;
		if(y<0|y>=height)continue;

		buf[y*width + x] = 0xff00;
	}
}
