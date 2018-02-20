#include "actor.h"
int openreadclose(void*, void*, u64, u64);
int openwriteclose(void*, void*, u64, u64);
int windowread(int type, char* buf);




static u8 fsbuf[0x1000];
static u8* buffer;
void fs_prep(void* name)
{
	int ret;
	u64 type;

	ret = openreadclose(name, fsbuf, 0, 0x1000);
	if(ret <= 0)return;
	fsbuf[ret] = 0;

	type = file_read(fsbuf, ret);
	say("type=%.8s\n", &type);
}




static void fs_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fs_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fs_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fs_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fs_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	drawtext(win, 0xffffff, cx-ww, cy-hh, cx+ww, cy+hh, fsbuf, 0x1000);
}
static void fs_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)fs_read_cli(win, act, sty);
	else if(fmt == _tui_)fs_read_tui(win, act, sty);
	else if(fmt == _vbo_)fs_read_vbo(win, act, sty);
	else if(fmt == _html_)fs_read_html(win, act, sty);
	else fs_read_pixel(win, act, sty);
}
static void fs_write(struct event* ev)
{
	int j,ret;
	u64 type = ev->what;
	if(_drop_ == type)
	{
		ret = windowread(type, buffer);
		say("%s", buffer);

		for(j=0;j<ret;j++)
		{
			if(buffer[j] < 0x20)
			{
				buffer[j] = 0;
				break;
			}
		}
		fs_prep(buffer);
	}
}
static void fs_list()
{
}
static void fs_into()
{
}
static void fs_start()
{
	fs_prep("makefile");
}
static void fs_stop()
{
}
void fs_create(void* base,void* addr)
{
	struct actor* p = addr;
	buffer = base+0x300000;

	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex32('f', 's', 0, 0);

	p->start = (void*)fs_start;
	p->stop = (void*)fs_stop;
	p->list = (void*)fs_list;
	p->choose = (void*)fs_into;
	p->read = (void*)fs_read;
	p->write = (void*)fs_write;
}
void fs_delete()
{
}
