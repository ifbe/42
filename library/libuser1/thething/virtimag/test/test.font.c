#include "libuser.h"
void drawascii_bitmap(u8* buf, int ch);
void dx11ascii_test(struct entity* wnd, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41ascii_test(struct entity* wnd, u32 rgb, vec3 vc, vec3 vr, vec3 vf);




static int chosen = 0x4040;



/*
static void font_dx11draw(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	dx11line_rect(wnd, 0xffffff, vc, vr, vf);
	dx11ascii_test(wnd, 0xffffff, vc, vr, vf);
}*/
static void font_gl41draw(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(wnd, 0xffffff, vc, vr, vf);
	gl41ascii_test(wnd, 0xffffff, vc, vr, vf);
}
static void font_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y,m,n;
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
	drawsolid_rect(win, 0x101010, cx-ww, cy-hh, cx+ww, cy+hh);

	ww &= 0xfff0;
	hh &= 0xfff0;
	for(y=-hh;y<hh;y+=16)
	{
		for(x=-ww;x<ww;x+=16)
		{
			m = (x>>4) + (chosen&0xff);
			if(m < 0)continue;
			if(m > 0xff)continue;

			n = (y>>4) + (chosen>>8);
			if(n < 0)continue;
			if(n > 0xff)continue;

//say("%d,%d\n",x,y);
			if((0 == x)|(0 == y))
			{
				drawsolid_rect(
					win, (255-m)+(0x008000)+(n<<16),
					cx+x, cy+y,
					cx+x+15, cy+y+15
				);
			}

			m = m + (n<<8);
			if((m > 0x20)&&(m < 0x80))
			{
				drawascii(win, 0xffffff, cx+x, cy+y, m);
			}
			else
			{
				drawunicode(win, 0xffffff, cx+x, cy+y, m);
			}
		}
	}

	drawsolid_rect(win, 0x0000ff, cx-32, cy-16, cx-1, cy-1);
	drawhexadecimal(win, 0xff0000, cx-32, cy-16, chosen);
}
static void font_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void font_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void font_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void font_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	u8 ch;
	u8 buf[0x20];
	drawascii_bitmap(buf, chosen);

	say("\n%02x0 1 2 3 4 5 6 7 8 9 a b c d e f\n", chosen);
	for(y=0;y<16;y++)
	{
		if(y<=9)say("%c ", 0x30+y);
		else say("%c ", 0x57+y);

		ch = buf[y];
		for(x=0;x<16;x++)
		{
			if(x >= 8)say("**");
			else
			{
				if((ch&0x80) != 0)say("%@");
				else say("--");
				ch = ch<<1;
			}
		}
		say("\n");
	}
}




static void font_event(
	struct entity* act, struct style* pin,
	struct event* ev, int len)
{
	int k = 0;
	if(_kbd_ == ev->what){
		switch(ev->why){
		case 0x48:k = 'u';break;
		case 0x50:k = 'd';break;
		case 0x4b:k = 'l';break;
		case 0x4d:k = 'r';break;
		default:return;
		}
	}
	else if(_char_ == ev->what){
		switch(ev->why){
		case 'w':k = 'u';break;
		case 's':k = 'd';break;
		case 'a':k = 'l';break;
		case 'd':k = 'r';break;
		default:return;
		}
	}
	else return;

	switch(k){
	case 'u':{
		if(chosen >= 256)chosen -= 256;
		break;
	}
	case 'd':{
		if(chosen <= 65535-256)chosen += 256;
		break;
	}
	case 'l':{
		if((chosen&0xff) > 0)chosen--;
		break;
	}
	case 'r':{
		if((chosen&0xff) < 0xff)chosen++;
		break;
	}
	}//switch
}




static void font_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		font_gl41draw(ent,slot, wor,geom, wnd,area);
		break;
	}
}
static void font_wrl_wnd(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key)
{
}
static void font_wnd(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key)
{
}




static void font_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		font_draw_pixel(ent,slot, caller,area);
		break;
	case _gl41list_:
		break;
	default:
		font_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void font_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	printmemory(buf,16);
	font_event(ent, 0, buf, len);
}
static void font_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void font_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void font_search(struct entity* act)
{
}
static void font_modify(struct entity* act)
{
}
static void font_delete(struct entity* act)
{
	if(0 == act)return;
}
static void font_create(struct entity* act)
{
	if(0 == act)return;
}




void font_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('f', 'o', 'n', 't');

	p->oncreate = (void*)font_create;
	p->ondelete = (void*)font_delete;
	p->onsearch = (void*)font_search;
	p->onmodify = (void*)font_modify;

	p->onlinkup = (void*)font_linkup;
	p->ondiscon = (void*)font_discon;
	p->ontaking = (void*)font_taking;
	p->ongiving = (void*)font_giving;
}
