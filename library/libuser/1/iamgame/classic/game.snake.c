#include "libuser.h"
#define WIDTH 32
#define HEIGHT 32
int snake_generate(void* buf, int w, int h);
int snake_left(void* buf, int w, int h);
int snake_right(void* buf, int w, int h);
int snake_up(void* buf, int w, int h);
int snake_down(void* buf, int w, int h);




struct snake
{
	u8 x;
	u8 y;
	u16 next;
};
static struct snake buf[WIDTH*HEIGHT];




void snake_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	int t1, t2, t3, t4;
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
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	//body
	j = 1;
	while(1)
	{
		//say("[%x]=%x,%x,%x\n", j, buf[j].x, buf[j].y, buf[j].next);
		t1 = cx-ww + buf[j].x * 2 * ww / WIDTH;
		t2 = cy-hh + buf[j].y * 2 * hh / HEIGHT;
		t3 = cx-ww + (buf[j].x+1) * 2 * ww / WIDTH;
		t4 = cy-hh + (buf[j].y+1) * 2 * hh / HEIGHT;
		drawsolid_rect(win, 0xffffff, t1+1, t2+1, t3-1, t4-1);

		if(buf[j].next <= 1)break;
		j = buf[j].next;
	}

	//food
	t1 = cx-ww + buf[0].x * 2 * ww / WIDTH;
	t2 = cy-hh + buf[0].y * 2 * hh / HEIGHT;
	t3 = cx-ww + (buf[0].x+1) * 2 * ww / WIDTH;
	t4 = cy-hh + (buf[0].y+1) * 2 * hh / HEIGHT;
	drawsolid_rect(win, 0x00ff00, t1+1, t2+1, t3-1, t4-1);
}
void snake_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void snake_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void snake_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
/*
	//<head>
	htmlprintf(win, 1,
		".snakebg{WIDTH:50%%;HEIGHT:50%%;float:left;background-color:#000;text-align:center;}\n"
		".snakefg{WIDTH:14%%;HEIGHT:14%%;float:left;background-color:#ccc;margin:0.1%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"snakebg\">\n");
	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"snakefg\">%d</div>\n",
				data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
*/
}
void snake_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j,t;
	int stride = win->stride;
	char* p = (char*)(win->buf);

	t = buf[0].x + buf[0].y*stride;
	p[t<<2] = '@';
}
void snake_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("snake(%x,%x,%x)\n",win,act,sty);
}
void snake_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)snake_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)snake_read_tui(win, sty, act, pin);
	else if(fmt == _html_)snake_read_html(win, sty, act, pin);
	else if(fmt == _json_)snake_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)snake_read_vbo(win, sty, act, pin);
	else snake_read_pixel(win, sty, act, pin);
}




void snake_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(_char_ == type)
	{
		if(key == 'w')snake_up(buf, WIDTH, HEIGHT);
		else if(key == 's')snake_down(buf, WIDTH, HEIGHT);
		else if(key == 'a')snake_left(buf, WIDTH, HEIGHT);
		else if(key == 'd')snake_right(buf, WIDTH, HEIGHT);
	}
	if(_kbd_ == type)
	{
		if(key == 0x48)snake_up(buf, WIDTH, HEIGHT);
		else if(key == 0x4b)snake_left(buf, WIDTH, HEIGHT);
		else if(key == 0x4d)snake_right(buf, WIDTH, HEIGHT);
		else if(key == 0x50)snake_down(buf, WIDTH, HEIGHT);
	}
}
static void snake_list()
{
}
static void snake_choose()
{
}
static void snake_stop(struct actor* act, struct pinid* pin)
{
}
static void snake_start(struct actor* act, struct pinid* pin)
{
	snake_generate(buf, WIDTH, HEIGHT);
}
static void snake_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void snake_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buf;
	if(_copy_ == act->type)act->buf = memorycreate(WIDTH*HEIGHT*4);
}




void snake_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('s', 'n', 'a', 'k', 'e', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)snake_create;
	p->ondelete = (void*)snake_delete;
	p->onstart  = (void*)snake_start;
	p->onstop   = (void*)snake_stop;
	p->onlist   = (void*)snake_list;
	p->onchoose = (void*)snake_choose;
	p->onread   = (void*)snake_read;
	p->onwrite  = (void*)snake_write;
}
