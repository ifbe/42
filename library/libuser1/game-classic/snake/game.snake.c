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




void snake_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int j;
	int t1, t2, t3, t4;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	//body
	j = 1;
	while(1)
	{
		//logtoall("[%x]=%x,%x,%x\n", j, buf[j].x, buf[j].y, buf[j].next);
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
void snake_draw_gl41(
	_obj* act, struct style* part,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
}
void snake_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
void snake_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
void snake_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int j,t;
	int width = win->whdf.width;
	char* p = win->tuitext.buf;

	t = buf[0].x + buf[0].y*width;
	p[t<<2] = '@';
}
void snake_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("snake(%x,%x,%x)\n",win,act,sty);
}




void snake_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
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




static void snake_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	snake_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void snake_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		snake_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void snake_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void snake_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void snake_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void snake_search(_obj* act)
{
}
static void snake_modify(_obj* act)
{
}
static void snake_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memoryfree(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void snake_create(_obj* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = buf;
	if(_copy_ == act->type)act->listptr.buf0 = memoryalloc(WIDTH*HEIGHT*4, 0);

	snake_generate(buf, WIDTH, HEIGHT);
}




void snake_register(_obj* p)
{
	p->kind = _game_;
	p->type = hex64('s', 'n', 'a', 'k', 'e', 0, 0, 0);
	p->vfmt = _orig_;

	p->oncreate = (void*)snake_create;
	p->ondelete = (void*)snake_delete;
	p->onreader = (void*)snake_search;
	p->onwriter = (void*)snake_modify;

	p->onattach = (void*)snake_attach;
	p->ondetach = (void*)snake_detach;
	p->ontaking = (void*)snake_taking;
	p->ongiving = (void*)snake_giving;
}
