#include "libuser.h"
struct uartterm
{
	u8* buf;
	u32 len;
	u32 fg;
	u32 bg;

	int width;
	int height;
	int vimw;
	int vimh;

	int left;
	int right;
	int top;
	int bottom;

	int curx;
	int cury;
};
int listuart(void*, int);
int startuart(void*, int);
int writeuart(int fd, int off, char* buf, int len);
//
int listshell(void*, int);
int startshell();
int writeshell(int fd, int off, char* buf, int len);
//
void drawterm(struct actor* win, void* term, int x0, int y0, int x1, int y1);
void terminal_serverinput(struct uartterm* term, u8* buf, int len);
void terminal_clientinput(struct actor* act, struct style* pin, struct actor* win, struct style* sty, struct event* ev);
//
void* getstdin();
int getcurin();
void* getstdout();
int getcurout();
void input(u8* buf, int len);




static u8 charbuf[0x100];
static int charlen = 0;




static void terminal_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
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

	if(act->orel0)
	{
		drawhyaline_rect(win, 0x111111, cx-ww, cy-hh, cx+ww, cy+hh);
		drawterm(win, act->idx, cx-ww, cy-hh, cx+ww, cy+hh);
	}
	else
	{
		void* obuf = getstdout();
		int ocur = getcurout();
		drawsolid_rect(win, 0x202020, cx-ww, cy-hh, cx+ww, cy+hh);
		drawsolid_rect(win, 0xe0e0e0, cx+ww-16, cy-hh, cx+ww, cy+hh);
		drawtext_reverse(win, 0xffffff, cx-ww, cy-hh, cx+ww-16, cy+hh, obuf, ocur);
	}
}
static void terminal_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int ocur;
	void* obuf;
	float* vc;
	float* vr;
	float* vf;
	vec3 tc, tr, tf;
	struct style tmp;
	if(0 == sty)
	{
		sty = &tmp;
		sty->f.vc[0] = 0.0;
		sty->f.vc[1] = 0.0;
		sty->f.vc[2] = -0.9;
		sty->f.vr[0] = 1.0;
		sty->f.vr[1] = 0.0;
		sty->f.vr[2] = 0.0;
		sty->f.vf[0] = 0.0;
		sty->f.vf[1] = 1.0;
		sty->f.vf[2] = 0.0;
	}
	vc = sty->f.vc;
	vr = sty->f.vr;
	vf = sty->f.vf;

	tc[0] = vc[0];
	tc[1] = vc[1];
	tc[2] = -0.5;
	carveopaque2d_rect(win, 0x404040, tc, vr, vf);

	tc[2] = -0.8;
	obuf = getstdout();
	ocur = getcurout();
	carvetext2d_reverse(win, 0xffffff, tc, vr, vf, obuf, ocur);
}
static void terminal_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void terminal_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"term\" style=\"width:50%%;height:100px;float:left;background-color:#f0ac2b;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void terminal_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x, y, w, h;
	u32* p;
	u32* q;
	u8* buf;
	struct uartterm* term;

	term = act->idx;
	w = win->width;
	h = win->height;
	if(w > term->width)w = term->width;
	if(h > term->height)h = term->height;

	p = (void*)(win->buf);
	q = (void*)(term->buf);
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			p[(win->stride)*y + x] = q[(term->width)*(term->top+y) + x];
		}
	}
}
static void terminal_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u8* p;
	int enq, deq;
	//say("terminal(%x,%x,%x)\n",win,act,sty);
}
static void terminal_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)terminal_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)terminal_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)terminal_draw_html(act, pin, win, sty);
	else if(fmt == _json_)terminal_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)terminal_draw_vbo(act, pin, win, sty);
	else terminal_draw_pixel(act, pin, win, sty);
}
static void terminal_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	void* buf, int len)
{
	int j;
	struct event* ev;
	if(0 == act->orel0)
	{
		ev = buf;
		if(_char_ == ev->what)
		{
			for(j=0;j<4;j++)
			{
				if(*(u8*)(buf+j) < 0x8)break;
			}
			input(buf, j);
		}
	}
	else if(0 == win)
	{
		terminal_clientinput(act, pin, win, sty, buf);
	}
	else
	{
		printmemory(buf,len);
		terminal_serverinput(act->idx, buf, len);
	}
}




static void terminal_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//terminal_draw(act, pin, win, sty);
}
static void terminal_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//terminal_event(act, pin, win, sty, ev, 0);
}
static void terminal_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void terminal_start(struct halfrel* self, struct halfrel* peer)
{
}




static void terminal_search(struct actor* act)
{
}
static void terminal_modify(struct actor* act)
{
}
static void terminal_delete(struct actor* act)
{
	if(0 == act)return;
	if(0 == act->buf)memorydelete(act->buf);
}
static void terminal_create(struct actor* act, void* arg)
{
	struct uartterm* term;
	if(0 == act)return;

	if(0 == arg)return;
	else
	{
		act->idx = memorycreate(sizeof(struct uartterm));
		act->buf = memorycreate(0x100000);

		term = act->idx;
		term->curx = 0;
		term->cury = 0;
		term->left = 0;
		term->right = 0;
		term->top = 0;
		term->bottom = 0;
		term->vimw = 128;
		term->vimh = 24;
		term->width = 128;
		term->height = 24;
		term->bg = 0;
		term->fg = 7;
		term->len = 0x100000;
		term->buf = act->buf;
	}
}




void terminal_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('t', 'e', 'r', 'm');

	p->oncreate = (void*)terminal_create;
	p->ondelete = (void*)terminal_delete;
	p->onsearch = (void*)terminal_search;
	p->onmodify = (void*)terminal_modify;

	p->onstart = (void*)terminal_start;
	p->onstop  = (void*)terminal_stop;
	p->onread  = (void*)terminal_read;
	p->onwrite = (void*)terminal_write;
}
