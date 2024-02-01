#include "libuser.h"
int utf2unicode(u8* src,u32* dst);
int windowread(int type, void* buf);
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);




static u8* universe = 0;




static int hexedit_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static void hexedit_modify(_obj* act)
{
}
static void hexedit_delete(_obj* act)
{
	if(0 == act)return;
}
static void hexedit_create(_obj* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;
	act->whdf.ix0 = 0;
	act->whdf.iy0 = 0;
	act->whdf.iz0 = 0;
	act->whdf.iw0 = 0;

	act->whdf.ixn = 0;
	act->whdf.iyn = 0;

	if(0 == arg)act->listptr.buf0 = universe;
	else{
		act->listptr.buf0 = memoryalloc(0x100000, 0);
		openreadclose(arg, 0, act->listptr.buf0, 0x100000);
	}
}




static void hexedit_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u32 unicode, color;
	int x,y,j;
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
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	ww &= 0xfff0;
	hh &= 0xfff0;
	if(act->whdf.iw0 == 0)		//hex
	{
		for(y=-hh;y<hh;y+=16)
		{
			for(x=-256;x<256;x+=16)
			{
				if(x >= ww)continue;
				if(x < -ww)continue;
				drawsolid_rect(
					win, ((0xf0-y)<<18)|((0xf0-x)<<2),
					cx+x, cy+y, cx+x+15, cy+y+15);

				j = ((y+hh)<<1) + ((x+256)>>4);
				drawdec8(win, 0xffffff, cx+x, cy+y, universe[j]);
			}
		}
	}
	else
	{
		color = 0xff;
		for(y=-hh;y<hh;y+=16)
		{
			for(x=-256;x<256;x+=16)
			{
				if(x >= ww)continue;
				if(x < -ww)continue;

				j = ((y+hh)<<1) + ((x+256)>>4);
				if(universe[j] < 0x80)
				{
					drawsolid_rect(win, color,
						cx+x, cy+y, cx+x+15, cy+y+15
					);
					drawascii(win, 0xffffff, cx+x, cy+y, universe[j]);
				}
				else
				{
					drawsolid_rect(win, color,
						cx+x, cy+y, cx+x+47, cy+y+47
					);

					j = utf2unicode(&universe[j], &unicode);
					drawunicode(win, 0xffffff, cx+x, cy+y, unicode);
					x += 0x10*(j-1);
				}

				color = (~color) & 0xff00ff;
			}
		}
	}
}
static void hexedit_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,j,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	//gl41solid_rect(wnd, 0x00ff00, vc,vr,vf);

	u8 ch;
	u8* buf = act->listptr.buf0;
	for(y=0;y<32;y++){
		for(x=0;x<32;x++){
			for(j=0;j<3;j++){
				tr[j] = vr[j]/32;
				tf[j] = vf[j]/32;
				tc[j] = vc[j] + tr[j]*(2*x-31) + tf[j]*(31-2*y);
			}
			gl41line_rect(wnd, 0xff0000, tc,tr,tf);

			ch = buf[act->whdf.iz0 + y*32 + x];
			if((x == act->whdf.ixn)&&(y == act->whdf.iyn))rgb = 0x00ff00;
			else rgb = 0xffffff;
			if(act->whdf.iw0)gl41ascii_center(wnd, rgb, tc,tr,tf,ch);
			else gl41hex8_center(wnd, rgb, tc,tr,tf,ch);
		}
	}
}
static void hexedit_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void hexedit_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void hexedit_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void hexedit_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("hex(%x,%x,%x)\n",win,act,sty);
}




static void hexedit_event(
	_obj* act, struct style* slot,
	_obj* wnd, struct style* area,
	struct event* ev)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;
	//logtoall("%x,%x\n",type,key);

	if(_char_ == type){
		if('	' == key)act->whdf.iw0 ^= 1;
	}

	if(_kbd_ == type){
	switch(key){
		case kbd_up:{
			if(act->whdf.iz0 >= 0x20)act->whdf.iz0 -= 0x20;
			break;
		}
		case kbd_down:{
			if(act->whdf.iz0 <= 0x1000000-0x20)act->whdf.iz0 += 0x20;
			break;
		}
		case kbd_left:{
			if(act->whdf.iz0 >= 0x10000)act->whdf.iz0 -= 0x10000;
			break;
		}
		case kbd_right:{
			if(act->whdf.iz0 <= 0x1000000-0x10000)act->whdf.iz0 += 0x10000;
			break;
		}
	}//switch(key)
	logtoall("%x\n",act->whdf.iz0);
	}//if(kbd)

	if(0x4070 == type){
		int w = wnd->whdf.width;
		int h = wnd->whdf.height;
		int x0 = w*area->fs.vc[0];
		int y0 = h*area->fs.vc[1];
		int dx = w*area->fs.vq[0];
		int dy = h*area->fs.vq[1];

		short* t = (void*)ev;
		act->whdf.ixn = (t[0]-x0)*32/dx;
		act->whdf.iyn = (t[1]-y0)*32/dy;
		//logtoall("%d,%d\n",act->whdf.ixn,act->whdf.iyn);
	}
}




static void hexedit_read_bycam(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	hexedit_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void hexedit_read_bywnd(_obj* ent,void* foot, _obj* wnd,void* area)
{
	struct fstyle fs;
	switch(wnd->vfmt){
	case _gl41list_:
		fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
		fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
		fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
		gl41data_before(wnd);
		hexedit_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		gl41data_01cam(wnd);
		gl41data_after(wnd);
		break;
	}
}




static int hexedit_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
		hexedit_read_bywnd(ent,slot, caller,area);break;
	default:
		hexedit_read_bycam(ent,slot, stack,sp);break;
	}
	return 0;
}
static int hexedit_giving(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	default:
		hexedit_event(ent,slot, caller,area, buf);break;
	}
	return 0;
}
static void hexedit_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void hexedit_attach(struct halfrel* self, struct halfrel* peer)
{
}




void hexedit_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('h', 'e', 'x', 'e','d','i','t', 0);

	p->oncreate = (void*)hexedit_create;
	p->ondelete = (void*)hexedit_delete;
	p->onreader = (void*)hexedit_search;
	p->onwriter = (void*)hexedit_modify;

	p->onattach = (void*)hexedit_attach;
	p->ondetach = (void*)hexedit_detach;
	p->ontaking = (void*)hexedit_taking;
	p->ongiving = (void*)hexedit_giving;
}
void hexedit_init(void* addr)
{
	universe = addr - 0xe00000;
}
