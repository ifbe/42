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




static void printnode(_obj* win, struct bplushead* this, int x, int y,
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
static void bplus_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	struct bplusleaf* node;
	struct bplushead* right;
	int cx = sty->fs.vc[0];
	int cy = sty->fs.vc[1];
	int ww = sty->fs.vr[0];
	int hh = sty->fs.vf[1];
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	node = act->listptr.buf0;
	if(node == 0)return;
	right = bplus_getparent(node, node);
	if(right == 0)return;

//printmemory(node, 0x800);
	printnode(win, right, cx, 1, cx, cy, ww, hh);
}
static void bplus_draw_gl41(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void bplus_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void bplus_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void bplus_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void bplus_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("tree(%x,%x,%x)\n",win,act,sty);
}




static void bplus_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
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




static void bplus_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void bplus_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void bplus_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void bplus_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void bplus_search(_obj* act)
{
}
static void bplus_modify(_obj* act)
{
}
static void bplus_delete(_obj* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->listptr.buf0);
}
static void bplus_create(_obj* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = node;
	if(_copy_ == act->type)act->listptr.buf0 = memorycreate(0x80*16, 0);
}




void bplus_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('b', 'p', 'l', 'u', 's', 0, 0, 0);

	p->oncreate = (void*)bplus_create;
	p->ondelete = (void*)bplus_delete;
	p->onreader = (void*)bplus_search;
	p->onwriter = (void*)bplus_modify;

	p->onattach = (void*)bplus_attach;
	p->ondetach = (void*)bplus_detach;
	p->ontaking = (void*)bplus_taking;
	p->ongiving = (void*)bplus_giving;
}
