#include "libuser.h"
void drawascii_bitmap(u8* buf, int ch);
void carveunicode_surround(
	struct arena*,u32,
	float,float,float,
	float,float,float,
	float,float,float,
	int, int);




static int chosen = 0x4040;
static u8 buffer[16];




static void font_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,m,n;
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
	drawline_rect(win, 0xff, cx-ww, cy-hh, cx+ww, cy+hh);

	ww &= 0xfff0;
	hh &= 0xfff0;
	for(y=-hh;y<hh;y+=16)
	{
		for(x=-ww;x<ww;x+=16)
		{
			m = (x>>4) + (chosen&0xff);
			if(m < 0)continue;
			if(m > 0xff)continue;

			n = (y>>4) + (chosen>>8);
			if(n < 0)continue;
			if(n > 0xff)continue;

//say("%d,%d\n",x,y);
			if((0 == x)|(0 == y))
			{
				drawsolid_rect(
					win, (255-m)+(0x008000)+(n<<16),
					cx+x, cy+y,
					cx+x+15, cy+y+15
				);
			}

			m = m + (n<<8);
			if((m > 0x20)&&(m < 0x80))
			{
				drawascii(win, 0xffffff, cx+x, cy+y, m);
			}
			else
			{
				drawunicode(win, 0xffffff, cx+x, cy+y, m);
			}
		}
	}

	drawsolid_rect(win, 0x0000ff, cx-32, cy-16, cx-1, cy-1);
	drawhexadecimal(win, 0xff0000, cx-32, cy-16, chosen);
}
static void font_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,dx,dy;
	int left,right,near,far;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline_rect(win, 0xffffff, vc, vr, vf);

	for(y=-32;y<32;y++)
	{
		for(x=-32;x<32;x++)
		{
			dx = x + (chosen&0xff);
			dy = y + ((chosen>>8)&0xff);
			if(dx < 0)continue;
			if(dy < 0)continue;
			if(dx > 0xff)continue;
			if(dy > 0xff)continue;

			f[0] = (2*x+1)/64;
			f[1] = (2*y+1)/64;
			f[2] = 0.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / 64;
			tr[1] = vr[1] / 64;
			tr[2] = vr[2] / 64;
			tf[0] = vf[0] / 64;
			tf[1] = vf[1] / 64;
			tf[2] = vf[2] / 64;
			carveunicode(win, 0xffffff, tc, tr, tf, (dy<<8)+dx);
		}
	}

	tr[0] = vr[0]/4;
	tr[1] = vr[1]/4;
	tr[2] = vr[2]/4;
	tf[0] = vf[0]/4;
	tf[1] = vf[1]/4;
	tf[2] = vf[2]/4;
	carvehexadecimal(win, 0x0000ff, vc, tr, tf, chosen);
}
static void font_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void font_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"font\" style=\"width:50%%;height:100px;float:left;background-color:#38147a;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void font_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void font_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	u8 ch;
	u8 buf[0x20];
	drawascii_bitmap(buf, chosen);

	say("\n%02x0 1 2 3 4 5 6 7 8 9 a b c d e f\n", chosen);
	for(y=0;y<16;y++)
	{
		if(y<=9)say("%c ", 0x30+y);
		else say("%c ", 0x57+y);

		ch = buf[y];
		for(x=0;x<16;x++)
		{
			if(x >= 8)say("**");
			else
			{
				if((ch&0x80) != 0)say("%@");
				else say("--");
				ch = ch<<1;
			}
		}
		say("\n");
	}
}
static void font_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)font_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)font_read_tui(win, sty, act, pin);
	else if(fmt == _html_)font_read_html(win, sty, act, pin);
	else if(fmt == _json_)font_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)font_read_vbo(win, sty, act, pin);
	else font_read_pixel(win, sty, act, pin);
}
static void font_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int k;
	if(_kbd_ == ev->what)
	{
		k = ev->why;
		if(k == 0x48)
		{
			if(chosen >= 256)chosen -= 256;
		}
		else if(k == 0x4b)
		{
			if((chosen&0xff) > 0)chosen--;
		}
		else if(k == 0x4d)
		{
			if((chosen&0xff) < 0xff)chosen++;
		}
		else if(k == 0x50)
		{
			if(chosen <= 65535-256)chosen += 256;
		}
	}
}
static void font_list()
{
}
static void font_change()
{
}
static void font_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void font_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void font_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void font_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void font_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('f', 'o', 'n', 't');

	p->oncreate = (void*)font_create;
	p->ondelete = (void*)font_delete;
	p->onstart  = (void*)font_start;
	p->onstop   = (void*)font_stop;
	p->onlist   = (void*)font_list;
	p->onchoose = (void*)font_change;
	p->onread   = (void*)font_read;
	p->onwrite  = (void*)font_write;
}
