#include "libuser.h"
#define halfsqrt3 0.8660254037844386




void drawsolid_triangle_crop(struct entity* win, u32 rgb,
	int x0, int y0, int x1, int y1, int x2, int y2)
{
	int l,r,x,y;
	int j,k,m,n;
	int width,height,stride;
	u32* buf;
	u32* tmp;
	if(y1 != y2)return;
	if(y0 == y1)return;

	width = win->width;
	height = win->height;
	stride = win->fbwidth>>2;
	buf = (u32*)(win->rgbabuf);

	if(y0 < y1){j = y0;k = y1;}
	if(y0 > y1){j = y1;k = y0;}
	if(j < 0)j = 0;
	if(k >= height)k = height-1;
	for(y=j;y<=k;y++)
	{
		l = x0 + (y-y0)*(x1-x0)/(y1-y0);
		r = x0 + (y-y0)*(x2-x0)/(y2-y0);

		if(l < r){m=l;n=r;}
		else {m=r;n=l;}

		if(n < 0)continue;
		if(m >= width)continue;

		if(m < 0)m = 0;
		if(n >= width)n = width-1;

		tmp = &buf[y*stride];
		for(x=m;x<=n;x++)tmp[x] = rgb;
	}
}
void drawsolid_triangle(struct entity* win, u32 rgb,
	int x0, int y0, int x1, int y1, int x2, int y2)
{
	int t;
	if(y0 > y1)
	{
		t = x0;x0 = x1;x1 = t;
		t = y0;y0 = y1;y1 = t;
	}
	if(y1 > y2)
	{
		t = x1;x1 = x2;x2 = t;
		t = y1;y1 = y2;y2 = t;
	}
	if(y0 > y1)
	{
		t = x0;x0 = x1;x1 = t;
		t = y0;y0 = y1;y1 = t;
	}
	if(y2 == y0)return;

	t = x0 + (y1-y0)*(x2-x0)/(y2-y0);
	drawsolid_triangle_crop(win, rgb, x0, y0, x1, y1, t, y1);
	drawsolid_triangle_crop(win, rgb, x2, y2, x1, y1, t, y1);
}




void drawsolid_rect(struct entity* win, u32 rgb,
	int x1, int y1, int x2, int y2)
{
	int x,y;
	int startx,endx,starty,endy;
	int width = win->width;
	int height = win->height;
	int stride = win->fbwidth>>2;
	u32* buf = (u32*)(win->rgbabuf);

	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(startx < 0)startx = 0;
	if(endx >= width)endx = width-1;

	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	if(starty < 0)starty = 0;
	if(endy >= height)endy = height-1;

	rgb |= 0xff000000;
	for(y=starty;y<=endy;y++)
	{
		for(x=startx;x<=endx;x++)
		{
			buf[ (y*stride) + x ] = rgb;
		}
	}
}
void drawsolid_circle(struct entity* win, u32 rgb,
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

		for(x=x1;x<=x2;x++)
		{
			buf[ (y*stride) + x ] = rgb;
		}
	}
}
void drawsolid_oval(struct entity* win, u32 rgb,
	int cx, int cy, int rx, int ry, int fx, int fy)
{
}
void drawsolid_sector(struct entity* win, u32 rgb,
	int cx, int cy, int radius, int start, int end)
{
}
