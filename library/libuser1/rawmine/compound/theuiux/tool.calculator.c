#include "libuser.h"
void postfix2binarytree(void* postfix, void* out);
void infix2postfix(void* infix, void* postfix);
double calculator(void* postfix, u64 x, u64 y);




static int count=0;
static u8 buffer[128];
static u8 infix[128];
static u8 postfix[128];
static u8 result[128];
//
static u8 table[4][8] = {
'0', '1', '2', '3', '+', '-', '*', '/',
'4', '5', '6', '7', '^', '%', '!', ' ',
'8', '9', 'a', 'b', '<', '>', '(', ')',
'c', 'd', 'e', 'f', '.', ' ', '`', '=',
};




static void calculator_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u32 fg;
	int x,y;
	int cx, cy, ww, hh;
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
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy);

	//display
	drawstring(win, 0xffffff, cx-ww, cy-hh, buffer, 0);
	drawstring(win, 0xffffff, cx-ww, cy-hh+16, infix, 0);
	drawstring(win, 0xffffff, cx-ww, cy-hh+32, postfix, 0);
	drawstring(win, 0xffffff, cx-ww, cy-hh+48, result, 0);

	//keypad
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			fg = 0x444444;
			if(x<4)fg += (y<<4) + (x<<20);
			else fg += (x<<4) + (y<<20);

			drawsolid_rect(win, fg,
				cx+ww*(x-4)/4, cy+hh*(y+0)/4,
				cx+ww*(x-3)/4, cy+hh*(y+1)/4
			);
			drawascii(win, 0xffffff,
				cx+ww*(x-4)/4, cy+hh*y/4, table[y][x]
			);
		}
	}
}/*
static void calculator_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y,rgb;
	vec3 tc,tr,tf;
	if(0 == sty)sty = defaultstyle_vbo2d();
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	tf[0] = vf[0] / 2;
	tf[1] = vf[1] / 2;
	tf[2] = vf[2] / 2;
	tc[0] = vc[0] + vf[0]/2;
	tc[1] = vc[1] + vf[1]/2;
	tc[2] = vc[2] + vf[2]/2;
	carvesolid2d_rect(win, 0x000020, tc, vr, tf);
	tc[0] = vc[0] - vf[0]/2;
	tc[1] = vc[1] - vf[1]/2;
	tc[2] = vc[2] - vf[2]/2;
	carvesolid2d_rect(win, 0x200000, tc, vr, tf);

	//display
	tr[0] = vr[0] / 8;
	tr[1] = vr[1] / 8;
	tr[2] = vr[2] / 8;
	tf[0] = vf[0] / 8;
	tf[1] = vf[1] / 8;
	tf[2] = vf[2] / 8;
	tc[0] = vc[0] + vf[0]*7/8;
	tc[1] = vc[1] + vf[1]*7/8;
	tc[2] = vc[2] + vf[2]*7/8 - 0.1;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, buffer, 0);
	tc[0] = vc[0] + vf[0]*5/8;
	tc[1] = vc[1] + vf[1]*5/8;
	tc[2] = vc[2] + vf[2]*5/8 - 0.1;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, infix, 0);
	tc[0] = vc[0] + vf[0]*3/8;
	tc[1] = vc[1] + vf[1]*3/8;
	tc[2] = vc[2] + vf[2]*3/8 - 0.1;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, postfix, 0);
	tc[0] = vc[0] + vf[0]*1/8;
	tc[1] = vc[1] + vf[1]*1/8;
	tc[2] = vc[2] + vf[2]*1/8 - 0.1;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, result, 0);

	//keypad
	tr[0] = vr[0] / 8;
	tr[1] = vr[1] / 8;
	tr[2] = vr[2] / 8;
	tf[0] = vf[0] / 8;
	tf[1] = vf[1] / 8;
	tf[2] = vf[2] / 8;
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			rgb = 0x444444;
			if(x<4)rgb += (y<<4) + (x<<20);
			else rgb += (x<<4) + (y<<20);

			tc[0] = vc[0] + vr[0]*(2*x-7)/8 + vf[0]*(7-2*y)/8;
			tc[1] = vc[1] + vr[1]*(2*x-7)/8 + vf[1]*(7-2*y)/8;
			tc[2] = vc[2] + vr[2]*(2*x-7)/8 + vf[2]*(7-2*y)/8 - 0.1;
			carvesolid2d_rect(win, rgb, tc, tr, tf);
			tc[2] -= 0.1;
			carve2d_ascii(win, 0xffffff, tc, tr, tf, table[y][x]);
		}
	}
}*/
static void calculator_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
}
static void calculator_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void calculator_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"calc\" style=\"width:50%%;height:100px;float:left;background-color:#8194ea;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void calculator_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void calculator_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("calc(%x,%x,%x)\n",win,act,sty);
	say("buffer:%s\n", infix);
	say("postfix:%s\n", postfix);
	say("result:%s\n", result);
}
static void calculator_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)calculator_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)calculator_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)calculator_draw_html(act, pin, win, sty);
	else if(fmt == _json_)calculator_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)calculator_draw_vbo2d(act, pin, win, sty);
		//else calculator_draw_vbo3d(act, pin, win, sty);
	}
	else calculator_draw_pixel(act, pin, win, sty);
}
static void calculator_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	double final;
	int x,y,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(_char_ == type)
	{
		if(0x8 == key)
		{
			if(count <= 0)return;

			count--;
			buffer[count] = 0x20;
		}
		else if(0xd == key)
		{
			//清空输入区
			for(ret=0;ret<count;ret++)
			{
				infix[ret] = buffer[ret];
				buffer[ret] = 0x20;
			}
			infix[count] = 0;
			count=0;
			say("buffer:%s\n", infix);

			infix2postfix(infix, postfix);
			say("postfix:%s\n", postfix);

			final = calculator(postfix, 0, 0);
			double2decstr(final, result);
			say("result:%s\n", result);
		}
		else
		{
			if(count<128)
			{
				buffer[count] = key;
				count++;
			}
		}
	}
}




static void calculator_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//calculator_draw(act, pin, win, sty);
}
static void calculator_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//calculator_event(act, pin, win, sty, ev, 0);
}
static void calculator_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void calculator_start(struct halfrel* self, struct halfrel* peer)
{
}




static void calculator_search(struct actor* act)
{
}
static void calculator_modify(struct actor* act)
{
}
static void calculator_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void calculator_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(128, 0);

	buffer[0] = '1';
	buffer[1] = '+';
	buffer[2] = '2';
	buffer[3] = 0;
	count = 3;
}




void calculator_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('c', 'a', 'l', 'c');

	p->oncreate = (void*)calculator_create;
	p->ondelete = (void*)calculator_delete;
	p->onsearch = (void*)calculator_search;
	p->onmodify = (void*)calculator_modify;

	p->onstart = (void*)calculator_start;
	p->onstop  = (void*)calculator_stop;
	p->onread  = (void*)calculator_read;
	p->onwrite = (void*)calculator_write;
}
