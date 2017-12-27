#include "actor.h"
int bplus_insert(void*, u64);
int bplus_destory(void*, u64);
void* bplus_child(void*, u64);




struct eachdata
{
	//if(leaf)		//if(index)
	u32 hash0;		//same
	u32 hash1;		//same
	u32 buf;		//ignore
	u32 len;		//ignore
	u64 irel;		//bigger childnode
	u64 orel;		//ignore
};
struct bplusnode
{
	//if(leaf)		//if(index)
	u32 type;		//same
	u32 len;		//same
	u64 parentnode;		//same
	u64 prevnode;		//smaller childnode
	u64 nextnode;		//ignore
	struct eachdata node[3];
};
static struct bplusnode* node = 0;




static void printnode(struct arena* win, struct bplusnode* this, int x, int y,
	int cx, int cy, int ww, int hh)
{
	int j,k;
	struct bplusnode* child;

	drawicon_1(win, 0x00ff00,
		x-24, cy-hh+y*64-16,
		x+24, cy-hh+y*64+16,
		"", 0
	);
	for(j=0;j<3;j++)
	{
		drawdecimal(win, 0, x+(j*2-3)*8, cy-hh+y*64-8, this->node[j].hash1);
	}

	if(y>=3)return;
	k = ww;
	for(j=y;j>0;j--)k = k>>2;

	for(j=0;j<4;j++)
	{
		//child = bplus_child(this, this->node[0].irel);
		//if(child == 0)return;
		printnode(win, this, x-k*(2*j-3), y+1, cx, cy, ww, hh);
	}
}
static void bplus_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);
	printnode(win, node, cx, 1, cx, cy, ww, hh);
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
	int j;
	char* p = (char*)node;
	for(j=0;j<0x100;j++)p[j] = 0;
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
