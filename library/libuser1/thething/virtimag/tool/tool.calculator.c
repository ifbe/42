#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);
//
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
{'0', '1', '2', '3', '+', '-', '*', '/'},
{'4', '5', '6', '7', '|', '%', '!', '^'},
{'8', '9', 'a', 'b', '<', '>', '(', ')'},
{'c', 'd', 'e', 'f', '.', ' ', 0xd, '='}
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
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
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
static void calculator_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j,x,y,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	for(j=0;j<3;j++){
		tf[j] = vf[j] / 2;
		tc[j] = vc[j] + vf[j]/2;
	}
	gl41solid_rect(ctx, 0x000020, tc, vr, tf);
	for(j=0;j<3;j++)tc[j] = vc[j] - vf[j]/2;
	gl41solid_rect(ctx, 0x200000, tc, vr, tf);

	//display
	for(j=0;j<3;j++){
		tr[j] = vr[j] / 8;
		tf[j] = vf[j] / 8;
	}
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*7/8 + vt[j]/64;
	gl41string_center(ctx, 0xffffff, tc, tr, tf, buffer, 0);
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*5/8 + vt[j]/64;
	gl41string_center(ctx, 0xffffff, tc, tr, tf, infix, 0);
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*3/8 + vt[j]/64;
	gl41string_center(ctx, 0xffffff, tc, tr, tf, postfix, 0);
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*1/8 + vt[j]/64;
	gl41string_center(ctx, 0xffffff, tc, tr, tf, result, 0);

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
			gl41solid_rect(ctx, rgb, tc, tr, tf);
			for(j=0;j<3;j++)tc[j] += vt[j]/32;
			gl41ascii_center(ctx, 0xffffff, tc, tr, tf, table[y][x]);
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




void calculator_char(struct entity* ent, struct style* slot, int key)
{
	int j;
	double final;
	if(0x8 == key)
	{
		if(count <= 0)return;

		count--;
		buffer[count] = 0x20;
	}
	else if(0xd == key)
	{
		//清空输入区
		for(j=0;j<count;j++)
		{
			infix[j] = buffer[j];
			buffer[j] = 0x20;
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
static void calculator_write_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area, struct event* ev)
{
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		//say("%f,%f\n",xyz[0], xyz[1]);

		if(0x2b70 == ev->what){
			int x = (int)(xyz[0]*8);
			int y = (int)(xyz[1]*8);
			y = 3-y;
			//say("%d,%d\n",x,y);
			if((y>=0)&&(y<=3))calculator_char(ent,slot, table[y][x]);
		}
	}
}




static void calculator_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	calculator_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void calculator_wnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;
	gl41data_before(wnd);
	calculator_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);

	gl41data_01cam(wnd);
}




static void calculator_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		calculator_wnd(ent,slot, caller,area);
		break;
	default:
		calculator_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void calculator_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41list_:{
		calculator_write_bywnd(ent,slot, wnd,area, buf);break;
	}
	}
}
static void calculator_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void calculator_linkup(struct halfrel* self, struct halfrel* peer)
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
}
static void calculator_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf0 = buffer;
	if(_copy_ == act->type)act->buf0 = memorycreate(128, 0);

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

	p->onlinkup = (void*)calculator_linkup;
	p->ondiscon = (void*)calculator_discon;
	p->ontaking = (void*)calculator_taking;
	p->ongiving = (void*)calculator_giving;
}
