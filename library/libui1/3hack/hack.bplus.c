#include "actor.h"
void bplus_prepare(void*, int);
int bplus_insert(void*, u64);
int bplus_destory(void*, u64);
void* bplus_getleft(void*, void*);
void* bplus_getright(void*, void*);
void* bplus_getchild(void*, void*, int);




struct bplushead
{
	u64 left:56;
	u8 type;
	u64 right:56;
	u8 len;
	u64 parent:56;
	u8 flag;
	u64 child:56;
	u8 haha;
};
struct indexdata
{
	u64 hash;
	u64 buf;
};
struct bplusindex
{
	struct bplushead head;
	struct indexdata node[6];
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
	struct leafdata node[3];
};
static struct bplusleaf* node = 0;




static void printnode(struct arena* win, struct bplushead* this, int x, int y,
	int cx, int cy, int ww, int hh)
{
	int j,k,len;
	struct bplushead* child;
	struct bplusleaf* leaf;
	struct bplusindex* index;

	drawicon_1(win, 0x00ff00,
		x-36, cy-hh+y*64-16,
		x+36, cy-hh+y*64+16,
		"", 0
	);

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
static void bplus_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	struct bplushead* right;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	if(node == 0)return;
	right = bplus_getright(node, node);
	if(right == 0)return;

//printmemory(node, 0x800);
	printnode(win, right, cx, 1, cx, cy, ww, hh);
}
static void bplus_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void bplus_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void bplus_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void bplus_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("tree(%x,%x,%x)\n",win,act,sty);
}
static void bplus_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)bplus_read_cli(win, act, sty);
	else if(fmt == __tui__)bplus_read_tui(win, act, sty);
	else if(fmt == __html__)bplus_read_html(win, act, sty);
	else if(fmt == __vbo__)bplus_read_vbo(win, act, sty);
	else bplus_read_pixel(win, act, sty);
}
static void bplus_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == __char__)
	{
		bplus_insert(node, key);
		//printmemory(node, 0x800);
	}
}




static void bplus_list()
{
}
static void bplus_into()
{
}
static void bplus_start()
{
	bplus_prepare(node, 0x100000);
}
static void bplus_stop()
{
}
void bplus_create(void* base,void* addr)
{
	struct actor* act = addr;
	node = base+0x300000;

	act->type = hex32('h', 'a', 'c', 'k');
	act->name = hex64('b', 'p', 'l', 'u', 's', 0, 0, 0);

	act->start = (void*)bplus_start;
	act->stop = (void*)bplus_stop;
	act->list = (void*)bplus_list;
	act->choose = (void*)bplus_into;
	act->read = (void*)bplus_read;
	act->write = (void*)bplus_write;
}
void bplus_delete()
{
}
