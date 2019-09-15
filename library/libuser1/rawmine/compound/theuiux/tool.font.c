#include "libuser.h"
void drawascii_bitmap(u8* buf, int ch);
void carveunicode_surround(
	struct actor*,u32,
	float,float,float,
	float,float,float,
	float,float,float,
	int, int);
void carveascii_test(struct actor* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf);




static int chosen = 0x4040;
static u8 buffer[16];




static void font_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y,m,n;
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
static void font_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carveline_rect(win, 0xffffff, vc, vr, vf);
	carveascii_test(win, 0xffffff, vc, vr, vf);
/*
	int x,y,dx,dy;
	int left,right,near,far;
	vec3 tc, tr, tf, tu, f;
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
*/
}
static void font_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void font_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
static void font_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void font_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
static void font_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)font_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)font_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)font_draw_html(act, pin, win, sty);
	else if(fmt == _json_)font_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)font_draw_vbo(act, pin, win, sty);
	else font_draw_pixel(act, pin, win, sty);
}
static void font_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
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




static void font_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	//say("@drone_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)font_draw_vbo(act,pin,ctx,sty);
	}
	//font_draw(act, pin, win, sty);
}
static void font_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//font_event(act, pin, win, sty, ev, 0);
}
static void font_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void font_start(struct halfrel* self, struct halfrel* peer)
{
}




static void font_search(struct actor* act)
{
}
static void font_modify(struct actor* act)
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
	if(_copy_ == act->type)act->buf = memorycreate(16, 0);
}




void font_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('f', 'o', 'n', 't');

	p->oncreate = (void*)font_create;
	p->ondelete = (void*)font_delete;
	p->onsearch = (void*)font_search;
	p->onmodify = (void*)font_modify;

	p->onstart = (void*)font_start;
	p->onstop  = (void*)font_stop;
	p->onread  = (void*)font_read;
	p->onwrite = (void*)font_write;
}
