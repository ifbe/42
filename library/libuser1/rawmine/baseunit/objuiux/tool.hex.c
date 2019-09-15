#include "libuser.h"
int utf2unicode(u8* src,u32* dst);
int windowread(int type, void* buf);




static u8 databuf[0x1000];
static int printmethod = 0;
void hex_prep(void* name)
{
	int ret = openreadclose(name, 0, databuf, 0x1000);
	if(ret <= 0)return;

	databuf[ret] = 0;
}




static void hex_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u32 unicode, color;
	int x,y,j;
	int cx, cy, ww, hh;
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
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	ww &= 0xfff0;
	hh &= 0xfff0;
	if(printmethod == 0)		//hex
	{
		for(y=-hh;y<hh;y+=16)
		{
			for(x=-256;x<256;x+=16)
			{
				if(x >= ww)continue;
				if(x < -ww)continue;
				drawsolid_rect(
					win, ((0xf0-y)<<18)|((0xf0-x)<<2),
					cx+x, cy+y, cx+x+15, cy+y+15);

				j = ((y+hh)<<1) + ((x+256)>>4);
				drawbyte(win, 0xffffff, cx+x, cy+y, databuf[j]);
			}
		}
	}
	else
	{
		color = 0xff;
		for(y=-hh;y<hh;y+=16)
		{
			for(x=-256;x<256;x+=16)
			{
				if(x >= ww)continue;
				if(x < -ww)continue;

				j = ((y+hh)<<1) + ((x+256)>>4);
				if(databuf[j] < 0x80)
				{
					drawsolid_rect(win, color,
						cx+x, cy+y, cx+x+15, cy+y+15
					);
					drawascii(win, 0xffffff, cx+x, cy+y, databuf[j]);
				}
				else
				{
					drawsolid_rect(win, color,
						cx+x, cy+y, cx+x+47, cy+y+47
					);

					j = utf2unicode(&databuf[j], &unicode);
					drawunicode(win, 0xffffff, cx+x, cy+y, unicode);
					x += 0x10*(j-1);
				}

				color = (~color) & 0xff00ff;
			}
		}
	}
}
static void hex_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void hex_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void hex_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"hex\" style=\"width:50%%;height:100px;float:left;background-color:#a8a7d9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void hex_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void hex_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("hex(%x,%x,%x)\n",win,act,sty);
}
static void hex_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)hex_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)hex_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)hex_draw_html(act, pin, win, sty);
	else if(fmt == _json_)hex_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)hex_draw_vbo(act, pin, win, sty);
	else hex_draw_pixel(act, pin, win, sty);
}
static void hex_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;
	//say("%x,%x\n",type,key);

	if(_drag_ == type)
	{
		ret = windowread(type, databuf);
		say("%s", databuf);

		for(j=0;j<ret;j++)
		{
			if(databuf[j] < 0x20)
			{
				databuf[j] = 0;
				break;
			}
		}
		hex_prep(databuf);
	}
	else if(_char_ == type)
	{
		if('	' == key)printmethod ^= 1;
	}
}




static void hex_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//hex_draw(act, pin, win, sty);
}
static void hex_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//hex_event(act, pin, win, sty, ev, 0);
}
static void hex_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void hex_start(struct halfrel* self, struct halfrel* peer)
{
}




static void hex_search(struct actor* act)
{
}
static void hex_modify(struct actor* act)
{
}
static void hex_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void hex_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = databuf;
	if(_copy_ == act->type)act->buf = memorycreate(0x1000, 0);
}




void hex_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('h', 'e', 'x', 0);

	p->oncreate = (void*)hex_create;
	p->ondelete = (void*)hex_delete;
	p->onsearch = (void*)hex_search;
	p->onmodify = (void*)hex_modify;

	p->onstart = (void*)hex_start;
	p->onstop  = (void*)hex_stop;
	p->onread  = (void*)hex_read;
	p->onwrite = (void*)hex_write;
}
