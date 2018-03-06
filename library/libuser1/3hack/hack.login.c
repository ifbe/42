#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void term_write(void*);
void carvestarry_random(void*);
void act_at(void*, void*);
void draw8bit_rect(struct arena* win, u8 rgb, int x0, int y0, int x1, int y1);
void carveascii_area(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float x0, float y0, float x1, float y1
);




static struct arena* arena = 0;
static struct actor* actor = 0;
static int chosen = 0;




void login_read_pixel(struct arena* win)
{
	u32 c;
	int x,y,j;
	int w = (win->w)/8;
	int h = (win->h)/16;

	for(j=0;j<32;j++)
	{
		if(j == chosen)c = 0x80ff00ff;
		else c = 0x800000ff;

		x = j%4;
		y = j/4;
		drawicon_1(
			win, c,
			(x+2)*w, (y+4)*h, (x+3)*w, (y+5)*h,
			(u8*)&actor[j].name, 8
		);
	}
}
void login_read_8bit(struct arena* win)
{
	int x,y;
	int j,c;
	for(j=0;j<32;j++)
	{
		if(j == chosen)c = 0x80;
		else c = 0x42;

		x = j%4;
		y = j/4;
		draw8bit_rect(
			win, c,
			 81+x*40, 61+y*10,
			118+x*40, 68+y*10
		);
	}
}
void login_read_vbo(struct arena* win)
{
	u32 c;
	int j,k;
	float x,y;

	carveline(
		win, 0xffffff,
		0.0, 0.0, 0.0,
		0.0, 0.0, 100.0
	);
	carveascii_area(
		win, 0xffffff,
		0.0, 0.0, 0.0,
		100.0, 0.0, 0.0,
		0.0, 100.0, 0.0,
		0.0, 0.0, 1.0, 1.0
	);

	for(j=0;j<32;j++)
	{
		if(j == chosen)
		{
			k = 4.0;
			c = 0x00ff00;
		}
		else
		{
			k = 1.0;
			c = 0xffffff;
		}

		x = cosine(j/PI)/32.0;
		y = sine(j/PI)/32.0;
		carvestring(
			win, c,
			x*256, y*256, j+1,
			x*64, y*64, 0.0,
			-y*64, x*64, 0.0,
			(u8*)&actor[j].name, 8
		);
	}
}
void login_read_html(struct arena* win)
{
	u32 c;
	int j;
	int len = win->len;
	u8* buf = win->buf;

	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)break;

		if(j == chosen)c = 0xff0000;
		else c = 0xffffff;

		len += mysnprintf(
			buf+len, 0x1000,
			"<div style=\"float:left;width:25%%;color:#%06x;\">%.8s</div>",
			c, (u8*)&actor[j].name
		);
	}
	win->len = len;
}
void login_read_tui(struct arena* win)
{
	int j,k,x,y;
	int ww = ((win->w)/2)&0xfffc;
	int hh = (win->h)/2;

	gentui_rect(win, 4, ww/2, hh/2, ww*3/2, hh*3/2);

	for(j=0;j<32;j++)
	{
		x = j%4;
		x = ww + (x-2)*ww/4;
		y = j/4;
		y = hh + (y-4);
		if(j==chosen)k=1;
		else k=2;

		gentui_rect(win, k, x, y, x+7, y);
		gentui_str(win, 0, x, y, (u8*)&actor[j].name, 8);
	}
}
void login_read_cli(struct arena* win)
{
}
void login_read(struct arena* win)
{
	if(win->fmt == _cli_)login_read_cli(win);
	else if(win->fmt == _tui_)login_read_tui(win);
	else if(win->fmt == _vbo_)login_read_vbo(win);
	else if(win->fmt == _html_)login_read_html(win);
	else if(win->fmt == _8bit_)login_read_8bit(win);
	else login_read_pixel(win);
}
void login_write(struct arena* win, struct event* ev)
{
	int x,y;
	y = (ev->what)&0xff;
	x = ((ev->what)>>8)&0xff;

	if(y == 'p')
	{
		if(x == '@')
		{
			x = (ev->why)&0xffff;
			x = (x*8) / (win->w);
			y = ((ev->why)>>16)&0xffff;
			y = (y*16) / (win->h);
			chosen = (y-4)*4 + (x-2);
		}
		else if(x == '-')
		{
			if((chosen >= 0) && (chosen < 32))
			{
				act_at(win, &actor[chosen]);
			}
		}
	}
	else if(ev->what == _kbd_)
	{
		if(ev->why == 0x4b)
		{
			chosen = (chosen+31)%32;
		}
		else if(ev->why == 0x4d)
		{
			chosen = (chosen+1)%32;
		}
	}
	else if(ev->what == _char_)
	{
		if((ev->why == 0xd)|(ev->why == 0xa))
		{
			act_at(win, &actor[chosen]);
		}
		else if(ev->why == 0x435b1b)
		{
			chosen = (chosen+1)%32;
		}
		else if(ev->why == 0x445b1b)
		{
			chosen = (chosen+31)%32;
		}
		else
		{
			term_write(ev);
		}
	}
}
void login_create(void* addr)
{
	arena = addr + 0;
	actor = addr + 0x100000;
}
void login_delete()
{
}
