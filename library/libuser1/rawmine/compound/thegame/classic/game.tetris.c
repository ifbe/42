#include "libuser.h"
#define WIDTH 32
#define HEIGHT 32
int tetris_generate(void* buf, int w, int h);
int tetris_left(void* buf, int w, int h);
int tetris_right(void* buf, int w, int h);
int tetris_up(void* buf, int w, int h);
int tetris_down(void* buf, int w, int h);




typedef struct stucture
{
	int x;
	int y;
	int type;
	int direction;
	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;
	int x4;
	int y4;
}structure;
static structure that;
static u8 data[HEIGHT][WIDTH];




static void tetris_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u32 c;
	int x, y, cx, cy, ww, hh;
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
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			//say("%d ",buf[y*WIDTH+x]);
			if(0 == data[y][x])continue;
			else if(1 == data[y][x])c = 0xffffff;
			else c = 0x00ff00;

			drawsolid_rect(win, c,
				cx-ww+1 + ((x+0)*2*ww)/WIDTH,
				cy-hh+1 + ((y+0)*2*hh)/HEIGHT,
				cx-ww-1 + ((x+1)*2*ww)/WIDTH,
				cy-hh-1 + ((y+1)*2*hh)/HEIGHT);
		}
		//say("\n");
	}
	//say("\n");
/*
	//print cubies
	cubie(win, 1,
		cx+(that.x1-16)*ww, cy+(that.y1-20)*hh,
		cx+(that.x1-15)*ww, cy+(that.y1-19)*hh);
	cubie(win, 1,
		cx+(that.x2-16)*ww, cy+(that.y2-20)*hh,
		cx+(that.x2-15)*ww, cy+(that.y2-19)*hh);
	cubie(win, 1,
		cx+(that.x3-16)*ww, cy+(that.y3-20)*hh,
		cx+(that.x3-15)*ww, cy+(that.y3-19)*hh);
	cubie(win, 1,
		cx+(that.x4-16)*ww, cy+(that.y4-20)*hh,
		cx+(that.x4-15)*ww, cy+(that.y4-19)*hh);
*/
}
static void tetris_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			f[0] = (2.0*x+1.0)/WIDTH - 1.0;
			f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
			f[2] = 1.0/36;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / (WIDTH+1);
			tr[1] = vr[1] / (WIDTH+1);
			tr[2] = vr[2] / (WIDTH+1);
			tf[0] = vf[0] / (HEIGHT+1);
			tf[1] = vf[1] / (HEIGHT+1);
			tf[2] = vf[2] / (HEIGHT+1);
			tu[0] = vu[0] * f[2];
			tu[1] = vu[1] * f[2];
			tu[2] = vu[2] * f[2];
			carvesolid_prism4(win, 0xffffff, tc, tr, tf ,tu);
		}
	}
}
static void tetris_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void tetris_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y;

	//<head>
	htmlprintf(win, 1,
		".tetrisbg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".tetrisfg{width:%f%%;height:%f%%;float:left;background-color:#ccc;margin:0.1%%;}\n",
		100.0/WIDTH-0.21, 100.0/HEIGHT-0.22
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"tetrisbg\">\n");
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			htmlprintf(win, 2, "<div class=\"tetrisfg\"></div>\n");
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void tetris_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y;
	int w = win->stride;
	int h = win->height;
	char* p = (char*)(win->buf);

	for(x=0;x<w*h*4;x++)p[x]=0;
	if(h>=HEIGHT)
	{
		for(y=0;y<HEIGHT;y++)
		{
			for(x=0;x<WIDTH;x++)
			{
				if(data[y][x])
				{
					p[(y*w+x)<<2]='#';
				}
			}
		}
		p[(that.x1 + that.y1*w)<<2]='#';
		p[(that.x2 + that.y2*w)<<2]='#';
		p[(that.x3 + that.y3*w)<<2]='#';
		p[(that.x4 + that.y4*w)<<2]='#';
	}
	else
	{
		for(y=0;y<h;y++)
		{
			for(x=0;x<WIDTH;x++)
			{
				if(data[y+HEIGHT-h][x])
				{
					p[(y*w+x)<<2]='#';
				}
			}
		}
		p[(that.x1 + (that.y1-HEIGHT+h)*w)<<2]='#';
		p[(that.x2 + (that.y2-HEIGHT+h)*w)<<2]='#';
		p[(that.x3 + (that.y3-HEIGHT+h)*w)<<2]='#';
		p[(that.x4 + (that.y4-HEIGHT+h)*w)<<2]='#';
	}
}
static void tetris_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void tetris_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)tetris_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)tetris_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)tetris_draw_html(act, pin, win, sty);
	else if(fmt == _json_)tetris_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)tetris_draw_vbo(act, pin, win, sty);
	else tetris_draw_pixel(act, pin, win, sty);
}
static void tetris_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	int ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == _kbd_)
	{
		if(key==0x48)tetris_up(data, WIDTH, HEIGHT);
		else if(key==0x4b)tetris_left(data, WIDTH, HEIGHT);
		else if(key==0x4d)tetris_right(data, WIDTH, HEIGHT);
		else if(key==0x50)tetris_down(data, WIDTH, HEIGHT);
	}
	else if(type == _char_)
	{
		if(key=='a')tetris_left(data, WIDTH, HEIGHT);
		else if(key=='d')tetris_right(data, WIDTH, HEIGHT);
		else if(key=='w')tetris_up(data, WIDTH, HEIGHT);
		else if(key=='s')tetris_down(data, WIDTH, HEIGHT);
		else if(key==' ')
		{
			for(ret=0;ret<20;ret++)
			{
				if(1 == tetris_down(data, WIDTH, HEIGHT))return;
			}
		}
	}
}




static void tetris_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//tetris_draw(act, pin, win, sty);
}
static void tetris_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//tetris_event(act, pin, win, sty, ev, 0);
}
static void tetris_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void tetris_start(struct halfrel* self, struct halfrel* peer)
{
}




static void tetris_search(struct actor* act)
{
}
static void tetris_modify(struct actor* act)
{
}
static void tetris_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void tetris_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(WIDTH*HEIGHT);

	tetris_generate(data, WIDTH, HEIGHT);
}




void tetris_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 't', 'r', 'i', 's', 0, 0);

	p->oncreate = (void*)tetris_create;
	p->ondelete = (void*)tetris_delete;
	p->onsearch = (void*)tetris_search;
	p->onmodify = (void*)tetris_modify;

	p->onstart = (void*)tetris_start;
	p->onstop  = (void*)tetris_stop;
	p->onread  = (void*)tetris_read;
	p->onwrite = (void*)tetris_write;
}
