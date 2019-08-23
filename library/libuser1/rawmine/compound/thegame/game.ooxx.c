#include "libuser.h"




static int turn;
static u8 data[3][3];




static void ooxx_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x, y, cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
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
}/*
static void ooxx_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	vec3 tc,tr,tf;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carvesolid2d_rect(win, 0x444444, vc, vr, vf);
}*/
static void ooxx_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carvesolid_rect(win, 0x444444, vc, vr, vf);
}
static void ooxx_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void ooxx_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
static void ooxx_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void ooxx_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
static void ooxx_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)ooxx_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)ooxx_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)ooxx_draw_html(act, pin, win, sty);
	else if(fmt == _json_)ooxx_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)ooxx_draw_vbo2d(act, pin, win, sty);
		//else ooxx_draw_vbo3d(act, pin, win, sty);
	}
	else ooxx_draw_pixel(act, pin, win, sty);
}
void ooxx_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
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




static void ooxx_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	say("@ooxx_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)ooxx_draw_vbo3d(act,pin,ctx,sty);
	}
	//ooxx_draw(act, pin, win, sty);
}
static void ooxx_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//ooxx_event(act, pin, win, sty, ev, 0);
}
static void ooxx_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void ooxx_start(struct halfrel* self, struct halfrel* peer)
{
}




static void ooxx_search(struct actor* act)
{
}
static void ooxx_modify(struct actor* act)
{
}
static void ooxx_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void ooxx_create(struct actor* act)
{
	int x,y;
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(16);

	turn = 0;
	for(y=0;y<3;y++){
		for(x=0;x<3;x++){
			data[y][x] = 0;
		}
	}
}




void ooxx_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('o', 'o', 'x', 'x');

	p->oncreate = (void*)ooxx_create;
	p->ondelete = (void*)ooxx_delete;
	p->onsearch = (void*)ooxx_search;
	p->onmodify = (void*)ooxx_modify;

	p->onstart = (void*)ooxx_start;
	p->onstop  = (void*)ooxx_stop;
	p->onread  = (void*)ooxx_read;
	p->onwrite = (void*)ooxx_write;
}
