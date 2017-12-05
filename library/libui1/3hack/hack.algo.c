#include "actor.h"




static struct actor* pl;
//
static u64 algtype[] = {
	0x35646d,		//md5
	0x31616873,		//sha1
	0
};
static int this = 0;
//
static u8* buffer;
static int buflen;
//
static u8* key;
static int keylen;
//
static int reslen;




static void algorithm_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	//top
	drawsolid_rect(win, 0x40,
		(win->w)/16, (win->h)/16,
		(win->w)*15/16, (win->h)*7/16
	);
	drawstring(win, 0xffffff,
		(win->w)/2, (win->h)/4,
		(void*)"makefile", 0
	);

	//middle
	drawsolid_rect(win, 0,
		(win->w)/16, (win->h)*7/16,
		(win->w)*15/16, (win->h)*9/16
	);
	drawline_rect(win, 0xffffff,
		(win->w/4)+32, (win->h/2)-16,
		(win->w*3/4)-32, (win->h/2)+16
	);

	//middle.left
	drawline_rect(win, 0xffffff,
		(win->w/4)-32, (win->h/2)-16,
		(win->w/4)+32, (win->h/2)+16
	);
	drawstring(win, 0xffffff,
		(win->w/4)-16, (win->h/2)-4,
		(void*)&algtype[this], 0
	);

	//middle.right
	drawline_rect(win, 0xffffff,
		(win->w*3/4)-32, (win->h/2)-16,
		(win->w*3/4)+32, (win->h/2)+16
	);
	drawstring(win, 0xffffff,
		(win->w*3/4)-16, (win->h/2)-4,
		(void*)"doit", 0
	);

	//bottom
	drawsolid_rect(win, 0x400000,
		(win->w)/16, (win->h)*9/16,
		(win->w)*15/16, (win->h)*15/16
	);
	drawstring(win, 0xffffff,
		(win->w/2)-(reslen*4), (win->h)*3/4,
		(void*)pl->priv, 0
	);
}
static void algorithm_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void algorithm_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void algorithm_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void algorithm_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("algorithm(%x,%x,%x)\n",win,act,sty);
}
static void algorithm_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)algorithm_read_cli(win, act, sty);
	else if(fmt == __tui__)algorithm_read_tui(win, act, sty);
	else if(fmt == __html__)algorithm_read_html(win, act, sty);
	else if(fmt == __vbo__)algorithm_read_vbo(win, act, sty);
	else algorithm_read_pixel(win, act, sty);
}
static void algorithm_write(struct event* ev)
{
	u64 key = ev->why;
	u64 type = ev->what;
	int x;
	int y;
	int ret;
	int temp[64];

	if(type==0x2d70)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		if(y<0x7000)return;
		if(y>0x9000)return;
		if(x<0x3000)return;
		if(x<0x5000)
		{
			this = (this+1)%2;
			return;
		}
		if(x<0xb000)return;

		int ret = readfile("makefile", buffer, 0, 0x100000);
		if(algtype[this] == 0x35646d)
		{
			md5sum(temp, buffer, ret);
			datastr2hexstr(pl->priv, temp, 16);
			pl->priv[32] = 0;
			reslen = 32;
		}
		else if(algtype[this] == 0x31616873)
		{
			sha1sum(temp, buffer, ret);
			datastr2hexstr(pl->priv, temp, 20);
			pl->priv[40] = 0;
			reslen = 40;
		}
	}
}
static void algorithm_list()
{
}
static void algorithm_choose()
{
}
static void algorithm_start()
{
	pl->priv[0] = 0;
}
static void algorithm_stop()
{
}
void algorithm_create(void* base, void* addr)
{
	pl = addr;
	buffer = base+0x300000;

	pl->type = hex32('h', 'a', 'c', 'k');
	pl->name = hex32('a', 'l', 'g', 'o');
	pl->irel = 0;
	pl->orel = 0;

	pl->start = (void*)algorithm_start;
	pl->stop = (void*)algorithm_stop;
	pl->list = (void*)algorithm_list;
	pl->choose = (void*)algorithm_choose;
	pl->read = (void*)algorithm_read;
	pl->write = (void*)algorithm_write;
}
void algorithm_delete()
{
}
