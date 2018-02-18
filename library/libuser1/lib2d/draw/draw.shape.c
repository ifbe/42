#include "actor.h"




void draw8bit_rect(struct arena* win, u8 rgb, int x0, int y0, int x1, int y1)
{
	int x,y;
	u8* buf = win->buf;
	for(y=y0;y<=y1;y++)
	{
		for(x=x0;x<=x1;x++)
		{
			buf[y*320+x] = rgb;
		}
	}
}




void drawline(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1)
{
	int dx,dy,sx,sy,e1,e2;
	int w = win->w;
	int h = win->h;
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

		if((x0 >= 0)&&(x0 < w)&&(y0 >= 0)&&(y0 < h))
		{buf[(y0*w) + x0] = rgb;}

		e2 = e1;
		if(e2 >-dx){e1 -= dy;x0 += sx;}
		if(e2 < dy){e1 += dx;y0 += sy;}
	}
}
void drawline_bezier(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2, int xc, int yc)
{
	int x,y,t;
	int width = win->w;
	int height = win->h;
	u32* buf = (u32*)(win->buf);

	for(t=0;t<1000;t++)
	{
		x = (1000-t)*(1000-t)*x1 + 2*t*(1000-t)*xc + t*t*x2;
		x /= 1000*1000;
		if(x<0|x>=width)continue;

		y = (1000-t)*(1000-t)*y1 + 2*t*(1000-t)*yc + t*t*y2;
		y /= 1000*1000;
		if(y<0|y>=height)continue;

		buf[y*width + x] = rgb;
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
	int width,height;
	int startx,endx,starty,endy;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;
	rgb |= 0xff000000;

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
			buf[((starty+n)*width) + x] = rgb;
		}
		for(x=startx;x<endx;x++)
		{
			if(endy < n)break;
			if(endy > height-1)break;
			if(x > width-1)break;
			if(x < 0)x=0;
			buf[((endy-n)*width) + x] = rgb;
		}
		for(y=starty;y<endy;y++)
		{
			if(startx < 0)break;
			if(startx >= width)break;
			if(y > height-1)break;
			if(y < 0)y = 0;
			buf[(y*width) + startx+n] = rgb;
		}
		for(y=starty;y<endy;y++)
		{
			if(endx < n)break;
			if(endx > width-1)break;
			if(y > height-1)break;
			if(y < 0)y = 0;
			buf[(y*width) + endx-n] = rgb;
		}
	}
}
void drawline_circle(struct arena* win, u32 rgb,
	int cx, int cy, int radius)
{
	int ret;
	int x,y;
	int x1,x2;
	int y1,y2;
	int width,height;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;
	rgb |= 0xff000000;

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

		buf[ (y*width) + x1 ] = rgb;
		buf[ (y*width) + x2 ] = rgb;
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




void drawsolid_triangle(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2, int x3, int y3)
{
}
void drawsolid_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2)
{
	int x,y;
	int width,height;
	int startx,endx,starty,endy;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;
	rgb |= 0xff000000;

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
			buf[ (y*width) + x ] = rgb;
		}
	}
}
void drawsolid_circle(struct arena* win, u32 rgb,
	int cx, int cy, int radius)
{
	int ret;
	int x,y;
	int x1,x2;
	int y1,y2;
	int width,height;
	u32* buf = (u32*)(win->buf);

	width = win->w;
	height = win->h;
	rgb |= 0xff000000;

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
			buf[ (y*width) + x ] = rgb;
		}
	}
}
void drawsolid_oval(struct arena* win, u32 rgb,
	int cx, int cy, int rx, int ry, int fx, int fy)
{
}
void drawsolid_sector(struct arena* win, u32 rgb,
	int cx, int cy, int radius, int start, int end)
{
}




void drawhyaline_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2)
{
	u8 r,g,b,a;
	int x,y,z;
	int startx,endx,starty,endy;
	int width = win->w;
	int height = win->h;
	u8* buf = (u8*)(win->buf);

	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(startx < 0)startx = 0;
	if(endx >= width)endx = width-1;

	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	if(starty < 0)starty = 0;
	if(endy >= height)endy = height-1;

	b = rgb&0xff;
	g = (rgb>>8)&0xff;
	r = (rgb>>16)&0xff;
	for(y=starty;y<=endy;y++)
	{
		for(x=startx;x<=endx;x++)
		{
			z = buf[4*(y*width + x) + 0];
			buf[4*(y*width + x) + 0] = (z/4) + b;

			z = buf[4*(y*width + x) + 1];
			buf[4*(y*width + x) + 1] = (z/4) + g;

			z = buf[4*(y*width + x) + 2];
			buf[4*(y*width + x) + 2] = (z/4) + r;

			buf[4*(y*width + x) + 3] = 0xff;
		}
	}
}




void drawaxis(struct arena* win)
{
}
void select_2d(struct arena* win, struct style* sty)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int w2 = (win->w) * (sty->wantw) / 0x20000;
	int h2 = (win->h) * (sty->wanth) / 0x20000;

	drawline_rect(win, 0xff00ff,
		cx-w2, cy-h2, cx+w2, cy+h2);
}
