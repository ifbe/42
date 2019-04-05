#include "libuser.h"
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




static void klotski_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x, y, cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

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
static void klotski_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_rect(win, 0xffffff, vc, vr, vf);
}
static void klotski_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void klotski_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;

	//<head>
	htmlprintf(win, 1,
		".klbg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".klfg{width:25%%;height:20%%;float:left;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"klbg\">\n");
	for(y=0;y<5;y++)
	{
		for(x=0;x<4;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"klfg\" style=\"background-color:#%06x\">%d</div>\n",
				color[data[y][x]], data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void klotski_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void klotski_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void klotski_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)klotski_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)klotski_read_tui(win, sty, act, pin);
	else if(fmt == _html_)klotski_read_html(win, sty, act, pin);
	else if(fmt == _json_)klotski_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)klotski_read_vbo(win, sty, act, pin);
	else klotski_read_pixel(win, sty, act, pin);
}
static void klotski_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}




static void klotski_get()
{
}
static void klotski_post()
{
}
static void klotski_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void klotski_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
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
static void klotski_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void klotski_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(20);
}




void klotski_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('k', 'l', 'o', 't', 's', 'k', 'i', 0);

	p->oncreate = (void*)klotski_create;
	p->ondelete = (void*)klotski_delete;
	p->onstart  = (void*)klotski_start;
	p->onstop   = (void*)klotski_stop;
	p->onget    = (void*)klotski_get;
	p->onpost   = (void*)klotski_post;
	p->onread   = (void*)klotski_read;
	p->onwrite  = (void*)klotski_write;
}
