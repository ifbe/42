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




static void klotski_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	carvesolid_rect(
		win, 0xffffff,
		cx, cy, cz,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
}
static void klotski_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

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
static void klotski_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void klotski_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void klotski_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void klotski_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)klotski_read_cli(win, sty, act, com);
	else if(fmt == _tui_)klotski_read_tui(win, sty, act, com);
	else if(fmt == _html_)klotski_read_html(win, sty, act, com);
	else if(fmt == _vbo_)klotski_read_vbo(win, sty, act, com);
	else klotski_read_pixel(win, sty, act, com);
}
static void klotski_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
}




static void klotski_list()
{
}
static void klotski_change()
{
}
static void klotski_start()
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
static void klotski_stop()
{
}
static void klotski_delete()
{
}
static void klotski_create()
{
}




void klotski_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('k', 'l', 'o', 't', 's', 'k', 'i', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)klotski_create;
	p->ondelete = (void*)klotski_delete;
	p->onstart  = (void*)klotski_start;
	p->onstop   = (void*)klotski_stop;
	p->onlist   = (void*)klotski_list;
	p->onchoose = (void*)klotski_change;
	p->onread   = (void*)klotski_read;
	p->onwrite  = (void*)klotski_write;
}