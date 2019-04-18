#include "libuser.h"
u32 getrandom();




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




void drawicon_1(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1)
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




void drawhyaline_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2)
{
	u8 r,g,b,a;
	int x,y,z;
	int startx,endx,starty,endy;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u8* buf = (u8*)(win->buf);

	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(startx < 0)startx = 0;
	if(endx >= width)endx = width-1;

	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	if(starty < 0)starty = 0;
	if(endy >= height)endy = height-1;

	a = (rgb>>24)&0xff;
	r = (rgb>>16)&0xff;
	g = (rgb>>8)&0xff;
	b = rgb&0xff;
	for(y=starty;y<=endy;y++)
	{
		for(x=startx;x<=endx;x++)
		{
			z = buf[4*(y*stride + x) + 0];
			buf[4*(y*stride + x) + 0] = (z*(0x100-a) + b*a)>>8;

			z = buf[4*(y*stride + x) + 1];
			buf[4*(y*stride + x) + 1] = (z*(0x100-a) + g*a)>>8;

			z = buf[4*(y*stride + x) + 2];
			buf[4*(y*stride + x) + 2] = (z*(0x100-a) + r*a)>>8;

			buf[4*(y*stride + x) + 3] = 0xff;
		}
	}
}




void drawarrorkey2d(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1, u8* buf, int t)
{
	u32 c;
	int cx = (x0+x1)/2;
	int cy = (y0+y1)/2;
	int r = (y1-y0)/2;
	drawsolid_circle(win, 0x404040, cx, cy, r);

	//x-: left
	if(buf[0]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx-r*2/3, cy, r/3);
	drawascii_fit(win, 0xffffff, cx-r, cy-r/3, cx-r/3, cy+r/3, buf[0]&0x7f);

	//x+: right
	if(buf[1]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx+r*2/3, cy, r/3);
	drawascii_fit(win, 0xffffff, cx+r/3, cy-r/3, cx+r, cy+r/3, buf[1]&0x7f);

	//y-: near
	if(buf[2]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx, cy+r*2/3, r/3);
	drawascii_fit(win, 0xffffff, cx-r/3, cy+r/3, cx+r/3, cy+r, buf[2]&0x7f);

	//y+: far
	if(buf[3]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx, cy-r*2/3, r/3);
	drawascii_fit(win, 0xffffff, cx-r/3, cy-r, cx+r/3, cy-r/3, buf[3]&0x7f);

	//z-: trigger
	if(buf[4]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx-r*2/3, cy-r*4/3, r/3);
	drawascii_fit(win, 0xffffff, cx-r, cy-r*5/3, cx-r/3, cy-r, buf[4]&0x7f);

	//z+: bumper
	if(buf[5]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx+r*2/3, cy-r*4/3, r/3);
	drawascii_fit(win, 0xffffff, cx+r/3, cy-r*5/3, cx+r, cy-r, buf[5]&0x7f);

	//press
	if(buf[6]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx, cy, r/3);
	drawascii_fit(win, 0xffffff, cx-r/3, cy-r/3, cx+r/3, cy+r/3, buf[6]&0x7f);

	//select or start
	if(buf[7]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx+r*t*4/3, cy-r*2/3, r/3);
	drawascii_fit(win, 0xffffff, cx+r*t*4/3-r/3, cy-r, cx+r*t*4/3+r/3, cy-r/3, buf[7]&0x7f);
}
void drawborder2d(struct arena* win, struct style* sty, void* name)
{
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];
	drawline_rect(win, 0x400040, cx-ww-1, cy-hh, cx+ww, cy+hh);
	drawsolid_rect(win, 0xff00ff, cx-ww-1, cy-hh-16, cx+ww, cy-hh);
	drawstring_fit(win, 0, cx-ww, cy-hh-16, cx+ww, cy-hh, name, 8);
}