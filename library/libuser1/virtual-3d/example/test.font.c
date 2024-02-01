#include "libuser.h"
void drawascii_bitmap(u8* buf, int ch);
void dx11ascii_test(_obj* wnd, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41ascii_test(_obj* wnd, u32 rgb, vec3 vc, vec3 vr, vec3 vf);




static int chosen = 0x4040;



/*
static void font_dx11draw(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	dx11line_rect(wnd, 0xffffff, vc, vr, vf);
	dx11ascii_test(wnd, 0xffffff, vc, vr, vf);
}*/
static void font_gl41draw(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(wnd, 0xffffff, vc, vr, vf);
	gl41ascii_test(wnd, 0xffffff, vc, vr, vf);
}
static void font_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
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

//logtoall("%d,%d\n",x,y);
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void font_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void font_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void font_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y;
	u8 ch;
	u8 buf[0x20];
	drawascii_bitmap(buf, chosen);

	logtoall("\n%02x0 1 2 3 4 5 6 7 8 9 a b c d e f\n", chosen);
	for(y=0;y<16;y++)
	{
		if(y<=9)logtoall("%c ", 0x30+y);
		else logtoall("%c ", 0x57+y);

		ch = buf[y];
		for(x=0;x<16;x++)
		{
			if(x >= 8)logtoall("**");
			else
			{
				if((ch&0x80) != 0)logtoall("%@");
				else logtoall("--");
				ch = ch<<1;
			}
		}
		logtoall("\n");
	}
}




static void font_event(
	_obj* act, struct style* pin,
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




static void font_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->vfmt){
	case _dx11list_:
	case _mt20list_:
	case _vk12list_:
		break;
	case _gl41list_:
		font_gl41draw(ent,slot, wor,geom, wnd,area);
		break;
	}
}
static void font_read_byworld_bywnd(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key)
{
}
static void font_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		font_draw_pixel(ent,slot, wnd,area);
		break;
	case _gl41list_:
		break;
	}
}




static void font_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		font_read_bywnd(ent,slot, caller,area);
		break;
	default:
		font_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void font_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	printmemory(buf,16);
	font_event(ent, 0, buf, len);
}
static void font_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void font_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void font_search(_obj* act)
{
}
static void font_modify(_obj* act)
{
}
static void font_delete(_obj* act)
{
	if(0 == act)return;
}
static void font_create(_obj* act)
{
	if(0 == act)return;
}




void font_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('f', 'o', 'n', 't');

	p->oncreate = (void*)font_create;
	p->ondelete = (void*)font_delete;
	p->onreader = (void*)font_search;
	p->onwriter = (void*)font_modify;

	p->onattach = (void*)font_attach;
	p->ondetach = (void*)font_detach;
	p->ontaking = (void*)font_taking;
	p->ongiving = (void*)font_giving;
}
