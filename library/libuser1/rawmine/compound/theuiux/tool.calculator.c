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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
}
static void calculator_draw_vbo(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j,x,y,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	for(j=0;j<3;j++){
		tf[j] = vf[j] / 2;
		tc[j] = vc[j] + vf[j]/2;
	}
	carvesolid_rect(ctx, 0x000020, tc, vr, tf);
	for(j=0;j<3;j++)tc[j] = vc[j] - vf[j]/2;
	carvesolid_rect(ctx, 0x200000, tc, vr, tf);

	//display
	for(j=0;j<3;j++){
		tr[j] = vr[j] / 8;
		tf[j] = vf[j] / 8;
	}
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*7/8 + vt[j]/64;
	carvestring_center(ctx, 0xffffff, tc, tr, tf, buffer, 0);
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*5/8 + vt[j]/64;
	carvestring_center(ctx, 0xffffff, tc, tr, tf, infix, 0);
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*3/8 + vt[j]/64;
	carvestring_center(ctx, 0xffffff, tc, tr, tf, postfix, 0);
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*1/8 + vt[j]/64;
	carvestring_center(ctx, 0xffffff, tc, tr, tf, result, 0);

	//keypad
	for(j=0;j<3;j++){
		tr[0] = vr[0] / 8;
		tf[2] = vf[2] / 8;
	}
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			rgb = 0x444444;
			if(x<4)rgb += (y<<4) + (x<<20);
			else rgb += (x<<4) + (y<<20);

			for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]*(2*x-7)/8 + vf[j]*(-2*y-1)/8 + vt[j]/32;
			carvesolid_rect(ctx, rgb, tc, tr, tf);
			for(j=0;j<3;j++)tc[j] += vt[j]/32;
			carveascii_center(ctx, 0xffffff, tc, tr, tf, table[y][x]);
		}
	}
}
static void calculator_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void calculator_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void calculator_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("calc(%x,%x,%x)\n",win,act,sty);
	say("buffer:%s\n", infix);
	say("postfix:%s\n", postfix);
	say("result:%s\n", result);
}
static void calculator_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
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




static void calculator_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//scene -> calc
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)calculator_draw_vbo(act,slot, scn,geom, wnd,area);
	}
}
static void calculator_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
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




static void calculator_search(struct entity* act)
{
}
static void calculator_modify(struct entity* act)
{
}
static void calculator_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void calculator_create(struct entity* act)
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




void calculator_register(struct entity* p)
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
