#include "libuser.h"
#define halfsqrt3 0.8660254037844386




void drawline(struct entity* win, u32 rgb,
	int x0, int y0, int x1, int y1)
{
	int dx,dy,sx,sy,e1,e2;
	int width = win->width;
	int height = win->height;
	int stride = win->fbwidth>>2;
	u32* buf = (u32*)(win->rgbabuf);

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
void drawline_arrow(struct entity* win, u32 rgb,
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
void drawbezier(struct entity* win, u32 rgb,
	int x1, int y1, int x2, int y2, int xc, int yc)
{
	int x,y,t;
	int width = win->width;
	int height = win->height;
	int stride = win->fbwidth>>2;
	u32* buf = (u32*)(win->rgbabuf);

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




void drawline_triangle(struct entity* win, u32 rgb,
	int x1, int y1, int x2, int y2, int x3, int y3)
{
	drawline(win, rgb, x1, y1, x2, y2);
	drawline(win, rgb, x1, y1, x3, y3);
	drawline(win, rgb, x2, y2, x3, y3);
}
void drawline_rect(struct entity* win, u32 rgb,
	int x1, int y1, int x2, int y2)
{
	int x,y,n;
	int startx,endx,starty,endy;
	int width = win->width;
	int height = win->height;
	int stride = win->fbwidth>>2;
	u32* buf = (u32*)(win->rgbabuf);

	if(x1<x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(y1<y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	//say("(%x,%x),(%x,%x)\n",startx,starty,endx,endy);

	rgb |= 0xff000000;
	for(n=0;n<2;n++)
	{
		if((starty+n >= 0) && (starty+n < height))
		{
			for(x=startx;x<endx;x++)
			{
				if(x >= width)break;
				if(x < 0)x=0;
				buf[((starty+n)*stride) + x] = rgb;
			}
		}
		if((endy-n >= 0) && (endy-n < height))
		{
			for(x=startx;x<endx;x++)
			{
				if(x > width-1)break;
				if(x < 0)x=0;
				buf[((endy-n)*stride) + x] = rgb;
			}
		}
		if((startx+n >= 0) && (startx+n < width))
		{
			for(y=starty;y<endy;y++)
			{
				if(y >= height)break;
				if(y < 0)y = 0;
				buf[(y*stride) + startx+n] = rgb;
			}
		}
		if((endx-n >= 0) && (endx-n < width))
		{
			for(y=starty;y<endy;y++)
			{
				if(y >= height)break;
				if(y < 0)y = 0;
				buf[(y*stride) + endx-n] = rgb;
			}
		}
	}
}
void drawline_circle(struct entity* win, u32 rgb,
	int cx, int cy, int radius)
{
	int x, y, ret;
	int x1, y1, x2, y2;
	int width = win->width;
	int height = win->height;
	int stride = win->fbwidth>>2;
	u32* buf = (u32*)(win->rgbabuf);

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
void drawline_oval(struct entity* win, u32 rgb,
	int cx, int cy, int rx, int ry, int fx, int fy)
{
}
void drawline_sector(struct entity* win, u32 rgb,
	int cx, int cy, int radius, int start, int end)
{
}




void drawaxis(struct entity* win)
{
	int w = win->width;
	int h = win->height;
	drawline(win, 0xffffff, w/2, 0, w/2, h);
	drawline(win, 0xffffff, 0, h/2, w, h/2);
}
void select_2d(struct entity* win, u32 rgb, struct fstyle* sty, u32 flag)
{
	int lb[2];
	int lu[2];
	int rb[2];
	int ru[2];
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int rx = sty->vr[0];
	int ry = sty->vr[1];
	int fx = sty->vf[0];
	int fy = sty->vf[1];

	lb[0] = cx-rx-fx;
	lb[1] = cy-ry-fy;
	lu[0] = cx-rx+fx;
	lu[1] = cy-ry+fy;
	rb[0] = cx+rx-fx;
	rb[1] = cy+ry-fy;
	ru[0] = cx+rx+fx;
	ru[1] = cy+ry+fy;
	drawline(win, 0xff00ff, lb[0], lb[1], lu[0], lu[1]);
	drawline(win, 0xff00ff, rb[0], rb[1], ru[0], ru[1]);
	drawline(win, 0xff00ff, lb[0], lb[1], rb[0], rb[1]);
	drawline(win, 0xff00ff, lu[0], lu[1], ru[0], ru[1]);
}
