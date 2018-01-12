#include "actor.h"
#define caocao 0
#define machao 1
#define zhaoyun 2
#define huangzhong 3
#define zhangfei 4
#define guanyu 5
#define bing0 6
#define bing1 7
#define bing2 8
#define bing3 9
static u8 data[5][4];
static u32 color[10] =
{
	0xffffff, 0x0000ff, 0x00ff00, 0x00ffff,
	0xff0000, 0xff00ff,
	0x882266, 0x367638, 0x069713, 0x123490
};




static void huarong_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void huarong_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int cx = (sty->cx) * (win->w) / 0x10000;
	int cy = (sty->cy) * (win->h) / 0x10000;
	int ww = (sty->wantw) * (win->w) / 0x20000;
	int hh = (sty->wanth) * (win->h) / 0x20000;

	for(y=0;y<5;y++)
	{
		for(x=0;x<4;x++)
		{
			if(data[y][x] > 9)continue;
			drawsolid_rect(
				win, color[data[y][x]],
				cx+(x-2)*ww/2,
				cy+(2*y-5)*hh/5,
				cx+(x-1)*ww/2,
				cy+(2*y-3)*hh/5
			);
		}
	}
}
static void huarong_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void huarong_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void huarong_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void huarong_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)huarong_read_cli(win, act, sty);
	else if(fmt == __tui__)huarong_read_tui(win, act, sty);
	else if(fmt == __html__)huarong_read_html(win, act, sty);
	else if(fmt == __vbo__)huarong_read_vbo(win, act, sty);
	else huarong_read_pixel(win, act, sty);
}
static void huarong_write(struct event* ev)
{
}




static void huarong_list()
{
}
static void huarong_change()
{
}
static void huarong_start()
{
	data[0][1] = data[0][2] = data[1][1] = data[1][2] = caocao;
	data[0][0] = data[1][0] = machao;
	data[0][3] = data[1][3] = zhaoyun;
	data[2][0] = data[3][0] = huangzhong;
	data[2][3] = data[3][3] = zhangfei;
	data[2][1] = data[2][2] = guanyu;
	data[3][1] = bing0;
	data[3][2] = bing1;
	data[4][0] = bing2;
	data[4][3] = bing3; 
	data[4][1] = data[4][2] = 0xff;
}
static void huarong_stop()
{
}
void huarong_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('h', 'u', 'a', 'r', 'o', 'n', 'g', 0);

	p->start = (void*)huarong_start;
	p->stop = (void*)huarong_stop;
	p->list = (void*)huarong_list;
	p->choose = (void*)huarong_change;
	p->read = (void*)huarong_read;
	p->write = (void*)huarong_write;
}
void huarong_delete()
{
}
