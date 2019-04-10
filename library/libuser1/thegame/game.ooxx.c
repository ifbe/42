#include "libuser.h"




static int turn;
static u8 data[3][3];




static void ooxx_read_pixel(
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
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	//heng
	drawline(win, 0xffffff, cx-ww, cy-hh/3, cx+ww, cy-hh/3);
	drawline(win, 0xffffff, cx-ww, cy+hh/3, cx+ww, cy+hh/3);

	//shu
	drawline(win, 0xffffff, cx-ww/3, cy-hh, cx-ww/3, cy+hh);
	drawline(win, 0xffffff, cx+ww/3, cy-hh, cx+ww/3, cy+hh);

	//ox
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			if('o' == data[y][x])
			{
				drawline_circle(win, 0xff,
					cx+(x-1)*ww*2/3, cy+(y-1)*hh*2/3, ww/6
				);
			}
			else if('x' == data[y][x])
			{
				drawline(win, 0xff0000,
					cx+(4*x-5)*ww/6, cy+(4*y-5)*hh/6,
					cx+(4*x-3)*ww/6, cy+(4*y-3)*hh/6
				);
				drawline(win, 0xff0000,
					cx+(4*x-5)*ww/6, cy+(4*y-3)*hh/6,
					cx+(4*x-3)*ww/6, cy+(4*y-5)*hh/6
				);
			}
		}//forx
	}//fory
}
static void ooxx_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc,tr,tf;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid2d_rect(win, 0x444444, vc, vr, vf);
}
static void ooxx_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_rect(win, 0x444444, vc, vr, vf);
}
static void ooxx_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void ooxx_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	char p[2];
	p[0] = p[1] = 0;

	//<head>
	htmlprintf(win, 1,
		".oxbg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".oxfg{width:33%%;height:33%%;float:left;background-color:#ccc;margin:0.1%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"oxbg\">\n");
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			p[0] = data[y][x];
			if(('o' != p[0])&&('x' != p[0]))p[0] = 0;

			htmlprintf(win, 2, "<div class=\"oxfg\">%s</div>\n", p);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void ooxx_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void ooxx_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8 ch;
	int x,y;
	say("ooxx(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			ch = data[y][x];
			if((ch!='o') && (ch!='x'))say("_	");
			else say("%c	",ch);
		}
		say("\n");
	}
}
static void ooxx_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)ooxx_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)ooxx_read_tui(win, sty, act, pin);
	else if(fmt == _html_)ooxx_read_html(win, sty, act, pin);
	else if(fmt == _json_)ooxx_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)ooxx_read_vbo2d(win, sty, act, pin);
		else ooxx_read_vbo3d(win, sty, act, pin);
	}
	else ooxx_read_pixel(win, sty, act, pin);
}
void ooxx_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	char val;
	int x,y;
/*
	int width = haha->width;
	int height = haha->height;
	int min = (width<height) ? width:height;
*/
	int min = 512;
	if(ev->what == 0x2d70)
	{
		x=(ev->why) & 0xffff;
		if(x>min)return;

		y=( (ev->why) >> 16 ) & 0xffff;
		if(y>min)return;

		x = x*3/min;
		y = y*3/min;
say("%d,%d\n",x,y);
		if(0 != data[y][x])return;

		if((turn&0x1) == 0x1)val='o';
		else val='x';

		data[y][x] = val;
		turn++;
	}
}
static void ooxx_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void ooxx_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void ooxx_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void ooxx_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	int x,y;

	turn=0;
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)data[y][x] = 0;
	}
}
static void ooxx_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void ooxx_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void ooxx_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('o', 'o', 'x', 'x');

	p->oncreate = (void*)ooxx_create;
	p->ondelete = (void*)ooxx_delete;
	p->onstart  = (void*)ooxx_start;
	p->onstop   = (void*)ooxx_stop;
	p->oncread  = (void*)ooxx_cread;
	p->oncwrite = (void*)ooxx_cwrite;
	p->onsread  = (void*)ooxx_sread;
	p->onswrite = (void*)ooxx_swrite;
}
