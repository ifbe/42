#include "libuser.h"
int openreadclose(void*, u64, void*, u64);
int openwriteclose(void*, u64, void*, u64);
int windowread(int type, void* buf);
int windowwrite(int type, void* buf);
void carvestl(void*, u32, vec3, vec3, vec3, vec3, void*, int);




void stl_prep(struct actor* act, void* name)
{
	float* p;
	float* vl = act->target.vl;	//left
	float* vr = act->target.vr;	//right
	float* vn = act->target.vn;	//near
	float* vf = act->target.vf;	//far
	float* vb = act->target.vb;	//bot
	float* vu = act->target.vu;	//top
	float* vv = act->target.vv;	//info
	float* vc = act->target.vc;	//center
	int j,ret;
	
	act->len = openreadclose(name, 0, act->buf, 0x800000);
	say("stllen=%x\n", (act->len));
	if((act->len) <= 0)return;

	vl[0] = 100000.0;
	vl[1] = 0.0;
	vl[2] = 0.0;
	vr[0] = -100000.0;
	vr[1] = 0.0;
	vr[2] = 0.0;

	vn[0] = 0.0;
	vn[1] = 100000.0;
	vn[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = -100000.0;
	vf[2] = 0.0;

	vb[0] = 0.0;
	vb[1] = 0.0;
	vb[2] = 100000.0;
	vu[0] = 0.0;
	vu[1] = 0.0;
	vu[0] = -100000.0;

	ret = *(u32*)((act->buf)+80);
	say("len=%x, count=%x\n", (act->len), ret);
	ret = ret%(0x200000/36);

	for(j=0;j<ret;j++)
	{
		p = (void*)(act->buf) + 84 + j*50;

		if(p[ 3] < vl[0])vl[0] = p[3];
		if(p[ 3] > vr[0])vr[0] = p[3];
		if(p[ 4] < vn[1])vn[1] = p[4];
		if(p[ 4] > vf[1])vf[1] = p[4];
		if(p[ 5] < vb[2])vb[2] = p[5];
		if(p[ 5] > vu[2])vu[2] = p[5];

		if(p[ 6] < vl[0])vl[0] = p[6];
		if(p[ 6] > vr[0])vr[0] = p[6];
		if(p[ 7] < vn[1])vn[1] = p[7];
		if(p[ 7] > vf[1])vf[1] = p[7];
		if(p[ 8] < vb[2])vb[2] = p[8];
		if(p[ 8] > vu[2])vu[2] = p[8];

		if(p[ 9] < vl[0])vl[0] = p[9];
		if(p[ 9] > vr[0])vr[0] = p[9];
		if(p[10] < vn[1])vn[1] = p[10];
		if(p[10] > vf[1])vf[1] = p[10];
		if(p[11] < vb[2])vb[2] = p[11];
		if(p[11] > vu[2])vu[2] = p[11];
	}
	say(
		"l=%f, r=%f, n=%f, f=%f, b=%f, u=%f\n",
		vl[0], vr[0], vn[1], vf[1], vb[2], vu[2]
	);

	vv[0] = vr[0] - vl[0];
	vv[1] = vf[1] - vn[1];
	vv[2] = vu[2] - vb[2];
	vc[0] = (vl[0] + vr[0])/2;
	vc[1] = (vn[1] + vf[1])/2;
	vc[2] = (vb[2] + vu[2])/2;
	say(
		"w=%f, h=%f, d=%f, x=%f, y=%f, z=%f\n",
		vv[0], vv[0], vv[1], vc[1], vc[2], vc[2]
	);
}




static void stl_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* p;
	float f;
	float v[3][3];
	int j,ret;
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
/*
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	if(0 == (act->buf))return;
	if(0 == (act->len))return;

	if((act->width) > (act->height))f = 2.0*ww/(act->width);
	else f = 2.0*hh/(act->height);

	ret = *(u32*)((act->buf)+80);
	ret = ret % ((0x800000-0x84)/50);
	for(j=0;j<ret;j++)
	{
		p = (void*)(act->buf) + 84 + j*50;

		v[0][0] = cx + (p[ 3]-(act->cx))*f;
		v[0][1] = cy + (p[ 4]-(act->cy))*f;
		v[1][0] = cx + (p[ 6]-(act->cx))*f;
		v[1][1] = cy + (p[ 7]-(act->cy))*f;
		v[2][0] = cx + (p[ 9]-(act->cx))*f;
		v[2][1] = cy + (p[10]-(act->cy))*f;

		drawline(win, 0xffffff, v[0][0], v[0][1], v[1][0], v[1][1]);
		drawline(win, 0xffffff, v[0][0], v[0][1], v[2][0], v[2][1]);
		drawline(win, 0xffffff, v[1][0], v[1][1], v[2][0], v[2][1]);
	}
*/
}
static void stl_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vr;
	if(act->buf == 0)return;
	if(act->len == 0)return;

	carvestl(win, 0xffffff, vc, vr, vf, vu, act, 0);
}
static void stl_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stl_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"stl\" style=\"width:50%%;height:100px;float:left;background-color:#3368a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void stl_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stl_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("stl(%x,%x,%x)\n",win,act,sty);
}
static void stl_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)stl_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)stl_read_tui(win, sty, act, pin);
	else if(fmt == _html_)stl_read_html(win, sty, act, pin);
	else if(fmt == _json_)stl_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)stl_read_vbo(win, sty, act, pin);
	else stl_read_pixel(win, sty, act, pin);
}




static void stl_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(0x2d70 == type)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
	else if(_drag_ == type)
	{
		char buffer[0x1000];
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
		stl_prep(act, buffer);
	}
}




static void stl_list()
{
}
static void stl_change()
{
}
static void stl_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stl_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	stl_prep(act, "42.stl");
}
static void stl_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void stl_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = memorycreate(0x800000);
	act->len = 0;
}




void stl_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('s', 't', 'l', 0);

	p->oncreate = (void*)stl_create;
	p->ondelete = (void*)stl_delete;
	p->onstart  = (void*)stl_start;
	p->onstop   = (void*)stl_stop;
	p->onlist   = (void*)stl_list;
	p->onchoose = (void*)stl_change;
	p->onread   = (void*)stl_read;
	p->onwrite  = (void*)stl_write;
}
