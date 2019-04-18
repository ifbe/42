#include "libuser.h"
void bplus_prepare(void*, int);
void bplus_alldone(void*);
void bplus_debug(void*);
void* bplus_getleft(void*, void*);
void* bplus_getright(void*, void*);
void* bplus_getparent(void*, void*);
void* bplus_getchild(void*, void*, int);
int bplus_insert(void*, u64);
int bplus_destory(void*, u64);




#define pagesize 0x80
struct bplushead
{
	u64 left:56;
	u8 type;
	u64 right:56;
	u8 len;
	u64 parent:56;
	u8 lock;
	u64 child:56;
	u8 flag;
};
struct indexdata
{
	u64 hash;
	u64 buf;
};
struct bplusindex
{
	struct bplushead head;
	struct indexdata node[(pagesize-0x20) / 0x10];
};
struct leafdata
{
	u64 hash;
	u64 buf;
	u64 irel;
	u64 orel;
};
struct bplusleaf
{
	struct bplushead head;
	struct leafdata node[(pagesize-0x20) / 0x20];
};
static struct bplusleaf node[16];




static void printnode(struct arena* win, struct bplushead* this, int x, int y,
	int cx, int cy, int ww, int hh)
{
	int j,k,len;
	int x0, y0, x1, y1;
	struct bplushead* child;
	struct bplusleaf* leaf;
	struct bplusindex* index;

	x0 = x-36;
	y0 = cy-hh+y*64-16;
	x1 = x+36;
	y1 = cy-hh+y*64+16;
	drawicon_1(win, 0x00ff00, x0, y0, x1, y1);

	len = this->len;
	if(this->type == '!')
	{
		leaf = (struct bplusleaf*)this;
		for(j=0;j<len;j++)
		{
			drawascii(win, 0x00ff00,
				x+(j*2-3)*12, cy-hh+y*64-8,
				leaf->node[j].hash
			);
		}
	}
	else
	{
		index = (struct bplusindex*)this;
		for(j=0;j<len;j++)
		{
			drawascii(win, 0xff00ff,
				x+(j*2-3)*12, cy-hh+y*64-8,
				index->node[j].hash
			);
		}
	}

	k = ww;
	for(j=y;j>0;j--)k = k>>2;
	for(j=0;j<len+1;j++)
	{
		child = bplus_getchild(node, this, (j+4)%5);
		if(child == 0)continue;

		drawline(win, 0xffffff,
			x, cy-hh+y*64+16,
			x+k*(2*j-3), cy-hh+y*64+48
		);
		printnode(win, child,
			x+k*(2*j-3), y+1,
			cx, cy, ww, hh
		);
	}
}
static void bplus_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct bplusleaf* node;
	struct bplushead* right;
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	node = act->buf;
	if(node == 0)return;
	right = bplus_getparent(node, node);
	if(right == 0)return;

//printmemory(node, 0x800);
	printnode(win, right, cx, 1, cx, cy, ww, hh);
}
static void bplus_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void bplus_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void bplus_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"bplus\" style=\"width:50%%;height:100px;float:left;background-color:#289467;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void bplus_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void bplus_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("tree(%x,%x,%x)\n",win,act,sty);
}
static void bplus_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)bplus_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)bplus_read_tui(win, sty, act, pin);
	else if(fmt == _html_)bplus_read_html(win, sty, act, pin);
	else if(fmt == _json_)bplus_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)bplus_read_vbo(win, sty, act, pin);
	else bplus_read_pixel(win, sty, act, pin);
}
static void bplus_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == _char_)
	{
		bplus_insert(node, key);
		printmemory(node, 0x800);
		bplus_debug(node);
	}
}
static void bplus_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void bplus_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void bplus_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void bplus_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void bplus_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void bplus_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = node;
	if(_copy_ == act->type)act->buf = memorycreate(0x80*16);
}




void bplus_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('b', 'p', 'l', 'u', 's', 0, 0, 0);

	p->oncreate = (void*)bplus_create;
	p->ondelete = (void*)bplus_delete;
	p->onstart  = (void*)bplus_start;
	p->onstop   = (void*)bplus_stop;
	p->oncread  = (void*)bplus_cread;
	p->oncwrite = (void*)bplus_cwrite;
	p->onsread  = (void*)bplus_sread;
	p->onswrite = (void*)bplus_swrite;
}
