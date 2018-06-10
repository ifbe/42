#include "libuser.h"
int openreadclose(void*, u64, void*, u64);
int openwriteclose(void*, u64, void*, u64);
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




static void hex_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 unicode, color;
	int x,y,j;
	int cx, cy, ww, hh;
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
static void hex_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void hex_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void hex_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void hex_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void hex_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("hex(%x,%x,%x)\n",win,act,sty);
}
static void hex_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)hex_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)hex_read_tui(win, sty, act, pin);
	else if(fmt == _html_)hex_read_html(win, sty, act, pin);
	else if(fmt == _json_)hex_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)hex_read_vbo(win, sty, act, pin);
	else hex_read_pixel(win, sty, act, pin);
}
static void hex_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
static void hex_list()
{
}
static void hex_into()
{
}
static void hex_stop(struct actor* act, struct pinid* pin)
{
}
static void hex_start(struct actor* act, struct pinid* pin)
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
	if(_copy_ == act->type)act->buf = memorycreate(0x1000);
}




void hex_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('h', 'e', 'x', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)hex_create;
	p->ondelete = (void*)hex_delete;
	p->onstart  = (void*)hex_start;
	p->onstop   = (void*)hex_stop;
	p->onlist   = (void*)hex_list;
	p->onchoose = (void*)hex_into;
	p->onread   = (void*)hex_read;
	p->onwrite  = (void*)hex_write;
}
