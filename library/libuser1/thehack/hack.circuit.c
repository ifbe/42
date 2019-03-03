#include "libuser.h"
u32 getrandom();
int solve_pcbwire(u8* buf, int w, int h, int l);




#define WIDTH 64
#define HEIGHT 64
#define LAYER 4




static void circuit_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 c,val;
	int x,y,z;
	int cx, cy, ww, hh;
	u8 (*data)[HEIGHT][WIDTH] = act->buf;

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

	z = 0;
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			drawline_rect(
				win, 0x404040,
				cx-ww + (x+x+0)*ww/WIDTH,
				cy-hh + (y+y+0)*hh/HEIGHT,
				cx-ww + (x+x+2)*ww/WIDTH,
				cy-hh + (y+y+2)*hh/HEIGHT
			);

			val = data[z][y][x];
			if(1 == val)c = 0xffffff;
			else if(2 == val)c = 0xffff00;
			else if(4 == val)c = 0xff00ff;
			else if(8 == val)c = 0xff0000;
			else if(16 == val)c = 0xffff;
			else if(32 == val)c = 0xff00;
			else if(64 == val)c = 0xff;
			else continue;

			drawsolid_rect(
				win, c,
				cx-ww + (x+x+0)*ww/WIDTH,
				cy-hh + (y+y+0)*hh/HEIGHT,
				cx-ww + (x+x+2)*ww/WIDTH,
				cy-hh + (y+y+2)*hh/HEIGHT
			);
		}
	}
}
static void circuit_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 c,val;
	int x,y,z;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	u8 (*data)[HEIGHT][WIDTH] = act->buf;

	tu[0] = vu[0]/64;
	tu[1] = vu[1]/64;
	tu[2] = vu[2]/64;
	tc[0] = vc[0] + 3*tu[0];
	tc[1] = vc[1] + 3*tu[1];
	tc[2] = vc[2] + 3*tu[2];
	carvesolid_prism4(win, 0x808080, tc, vr, vf, tu);
	for(z=0;z<2;z++)
	{
		for(y=0;y<HEIGHT;y++)
		{
			for(x=0;x<WIDTH;x++)
			{
				val = data[z][y][x];
				if(1 == val)c = 0xffffff;
				else if(2 == val)c = 0xffff00;
				else if(4 == val)c = 0xff00ff;
				else if(8 == val)c = 0xff0000;
				else if(16 == val)c = 0xffff;
				else if(32 == val)c = 0xff00;
				else if(64 == val)c = 0xff;
				else continue;

				f[0] = (x+x+1-WIDTH)  / (float)WIDTH;
				f[1] = (y+y+1-HEIGHT) / (float)HEIGHT;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vr[0] / WIDTH;
				tr[1] = vr[1] / WIDTH;
				tr[2] = vr[2] / WIDTH;
				tf[0] = vf[0] / HEIGHT;
				tf[1] = vf[1] / HEIGHT;
				tf[2] = vf[2] / HEIGHT;
				tu[0] = vu[0]/64;
				tu[1] = vu[1]/64;
				tu[2] = vu[2]/64;
				tc[0] += (z*4+1)*tu[0];
				tc[1] += (z*4+1)*tu[1];
				tc[2] += (z*4+1)*tu[2];
				carvesolid_prism4(win, c, tc, tr, tf, tu);
			}
		}
	}
}
static void circuit_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void circuit_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"circuit\" style=\"width:50%%;height:100px;float:left;background-color:#e8194a;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void circuit_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void circuit_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("circuit(%x,%x,%x)\n",win,act,sty);
}
static void circuit_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)circuit_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)circuit_read_tui(win, sty, act, pin);
	else if(fmt == _html_)circuit_read_html(win, sty, act, pin);
	else if(fmt == _json_)circuit_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)circuit_read_vbo(win, sty, act, pin);
	else circuit_read_pixel(win, sty, act, pin);
}
static void circuit_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void circuit_get()
{
}
static void circuit_post()
{
}
static void circuit_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void circuit_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
	int x,y,z,w;
	u8 (*data)[HEIGHT][WIDTH] = leaf->buf;

	for(z=0;z<LAYER;z++)
	{
		for(y=0;y<HEIGHT;y++)
		{
			for(x=0;x<WIDTH;x++)
			{
				data[z][y][x] = 0;
			}
		}
	}

	for(w=0;w<8;w++)
	{
		x = getrandom()%WIDTH;
		y = getrandom()%HEIGHT;
		z = getrandom()%LAYER;
		data[z][y][x] = 1<<w;

		x = getrandom()%WIDTH;
		y = getrandom()%HEIGHT;
		z = getrandom()%LAYER;
		data[z][y][x] = 1<<w;
	}

	solve_pcbwire((void*)data, WIDTH, HEIGHT, LAYER);
}
static void circuit_delete(struct actor* act)
{
	if(0 == act)return;
	if(act->buf)memorydelete(act->buf);
}
static void circuit_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = memorycreate(WIDTH*HEIGHT*LAYER);
}




void circuit_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('c', 'i', 'r', 'c', 'u', 'i', 't', 0);

	p->oncreate = (void*)circuit_create;
	p->ondelete = (void*)circuit_delete;
	p->onstart  = (void*)circuit_start;
	p->onstop   = (void*)circuit_stop;
	p->onget    = (void*)circuit_get;
	p->onpost   = (void*)circuit_post;
	p->onread   = (void*)circuit_read;
	p->onwrite  = (void*)circuit_write;
}
