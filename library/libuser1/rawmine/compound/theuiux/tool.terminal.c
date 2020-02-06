#include "libuser.h"
#define CLIENT data0
#define SERVER data1
#define DATBUF buf2
#define IDXBUF buf3




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
void drawterm(struct entity* win, void* term, int x0, int y0, int x1, int y1);
void terminal_serverinput(struct uartterm* term, u8* buf, int len);
void terminal_clientinput(struct entity* act, struct style* pin, struct entity* win, struct style* sty, struct event* ev);
//
void* getstdin();
int getcurin();
void* getstdout();
int getcurout();
void input(u8* buf, int len);




static u8 charbuf[0x100];
static int charlen = 0;




static void terminal_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
		drawterm(win, act->IDXBUF, cx-ww, cy-hh, cx+ww, cy+hh);
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
static void terminal_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terminal_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terminal_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x, y, w, h;
	u32* p;
	u32* q;
	u8* buf;
	struct uartterm* term;

	term = act->IDXBUF;
	w = win->width;
	h = win->height;
	if(w > term->width)w = term->width;
	if(h > term->height)h = term->height;

	p = (void*)(win->textbuf);
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u8* p;
	int enq, deq;
	//say("terminal(%x,%x,%x)\n",win,act,sty);
}




void terminal_write_c(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	struct entity* ent = self->pchip;
	if(ent->SERVER){
		relationwrite(ent, 's', 0, 0, buf,len);
		return;
	}

	struct event* ev = buf;
	if(_char_ == ev->what){
		input(buf, 1);
	}
}
void terminal_write_s(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	struct entity* ent = self->pchip;
	if(ent->CLIENT)relationwrite(ent, 'c', 0, 0, buf,len);
	else printmemory(buf, 8);
}




static void terminal_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	vec3 tc;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	for(j=0;j<3;j++)tc[j] = vc[j] - vt[j]/1000;
	gl41opaque_rect(ctx, 0x80808080, tc, vr, vf);

	void* obuf = getstdout();
	int ocur = getcurout();
	carvetext_reverse(ctx, 0xffffff, vc, vr, vf, obuf, ocur);
	//carvestring(ctx, 0xffffff, vc, vr, vf, obuf, ocur);
}
static void terminal_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//scene -> terminal
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack && ('v' == len)){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		terminal_draw_gl41(act,slot, scn,geom, wnd,area);
	}
}




static void terminal_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	switch(self->flag){
	case 'c':break;
	case 's':break;
	default: terminal_read_bycam(self,peer, stack,rsp, buf,len);
	}
}
static void terminal_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	switch(self->flag){
	case 'c':terminal_write_c(self,peer, buf,len);break;
	case 's':terminal_write_s(self,peer, buf,len);break;
	}
}
static void terminal_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void terminal_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
	case 'c':ent->CLIENT = 1;break;
	case 's':ent->SERVER = 1;break;
	}
}




static void terminal_search(struct entity* act)
{
}
static void terminal_modify(struct entity* act)
{
}
static void terminal_delete(struct entity* act)
{
	if(0 == act)return;
}
static void terminal_create(struct entity* act, void* arg, int argc, u8** argv)
{
	struct uartterm* term;
	if(0 == act)return;

	act->CLIENT = 0;
	act->SERVER = 0;
	if(0 == arg)return;

	term = act->IDXBUF = memorycreate(sizeof(struct uartterm), 0);
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
	term->buf = memorycreate(0x100000, 0);
}




void terminal_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('t', 'e', 'r', 'm');

	p->oncreate = (void*)terminal_create;
	p->ondelete = (void*)terminal_delete;
	p->onsearch = (void*)terminal_search;
	p->onmodify = (void*)terminal_modify;

	p->onlinkup = (void*)terminal_linkup;
	p->ondiscon = (void*)terminal_discon;
	p->onread  = (void*)terminal_read;
	p->onwrite = (void*)terminal_write;
}
