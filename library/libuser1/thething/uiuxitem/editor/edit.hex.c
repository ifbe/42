#include "libuser.h"
int utf2unicode(u8* src,u32* dst);
int windowread(int type, void* buf);
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);




static u8* universe = 0;




static int hexedit_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;

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
static void hexedit_modify(struct entity* act)
{
}
static void hexedit_delete(struct entity* act)
{
	if(0 == act)return;
}
static void hexedit_create(struct entity* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;
	act->ix0 = 0;
	act->iy0 = 0;
	act->iz0 = 0;
	act->iw0 = 0;

	act->ixn = 0;
	act->iyn = 0;

	if(0 == arg)act->buf0 = universe;
	else{
		act->buf0 = memorycreate(0x100000, 0);
		openreadclose(arg, 0, act->buf0, 0x100000);
	}
}




static void hexedit_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	ww &= 0xfff0;
	hh &= 0xfff0;
	if(act->iw0 == 0)		//hex
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
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y,j,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	//gl41solid_rect(wnd, 0x00ff00, vc,vr,vf);

	u8 ch;
	u8* buf = act->buf0;
	for(y=0;y<32;y++){
		for(x=0;x<32;x++){
			for(j=0;j<3;j++){
				tr[j] = vr[j]/32;
				tf[j] = vf[j]/32;
				tc[j] = vc[j] + tr[j]*(2*x-31) + tf[j]*(31-2*y);
			}
			gl41line_rect(wnd, 0xff0000, tc,tr,tf);

			ch = buf[act->iz0 + y*32 + x];
			if((x == act->ixn)&&(y == act->iyn))rgb = 0x00ff00;
			else rgb = 0xffffff;
			if(act->iw0)gl41ascii_center(wnd, rgb, tc,tr,tf,ch);
			else gl41hex8_center(wnd, rgb, tc,tr,tf,ch);
		}
	}
}
static void hexedit_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void hexedit_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void hexedit_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void hexedit_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("hex(%x,%x,%x)\n",win,act,sty);
}




static void hexedit_event(
	struct entity* act, struct style* slot,
	struct supply* wnd, struct style* area,
	struct event* ev)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;
	//say("%x,%x\n",type,key);

	if(_char_ == type){
		if('	' == key)act->iw0 ^= 1;
	}

	if(_kbd_ == type){
	switch(key){
		case kbd_up:{
			if(act->iz0 >= 0x20)act->iz0 -= 0x20;
			break;
		}
		case kbd_down:{
			if(act->iz0 <= 0x1000000-0x20)act->iz0 += 0x20;
			break;
		}
		case kbd_left:{
			if(act->iz0 >= 0x10000)act->iz0 -= 0x10000;
			break;
		}
		case kbd_right:{
			if(act->iz0 <= 0x1000000-0x10000)act->iz0 += 0x10000;
			break;
		}
	}//switch(key)
	say("%x\n",act->iz0);
	}//if(kbd)

	if(0x4070 == type){
		int w = wnd->width;
		int h = wnd->height;
		int x0 = w*area->fs.vc[0];
		int y0 = h*area->fs.vc[1];
		int dx = w*area->fs.vq[0];
		int dy = h*area->fs.vq[1];

		short* t = (void*)ev;
		act->ixn = (t[0]-x0)*32/dx;
		act->iyn = (t[1]-y0)*32/dy;
		//say("%d,%d\n",act->ixn,act->iyn);
	}
}




static void hexedit_read_bycam(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack&&('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		hexedit_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void hexedit_read_bywnd(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	gl41data_before(wnd);
	hexedit_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}




static int hexedit_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(sp < 2)return 0;
	struct entity* sup = stack[sp-2].pchip;

	switch(sup->fmt){
	case _gl41full_:{
		if('v' != key)break;
		hexedit_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	default:{
		hexedit_read_bycam(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	}
	return 0;
}
static int hexedit_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(sp < 2)return 0;
	struct supply* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41full_:{
		hexedit_event(ent,0, wnd,area, buf);break;
	}
	}
	return 0;
}
static void hexedit_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void hexedit_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void hexedit_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('h', 'e', 'x', 'e','d','i','t', 0);

	p->oncreate = (void*)hexedit_create;
	p->ondelete = (void*)hexedit_delete;
	p->onsearch = (void*)hexedit_search;
	p->onmodify = (void*)hexedit_modify;

	p->onlinkup = (void*)hexedit_linkup;
	p->ondiscon = (void*)hexedit_discon;
	p->ontaking = (void*)hexedit_taking;
	p->ongiving = (void*)hexedit_giving;
}
void hexedit_init(void* addr)
{
	universe = addr - 0xe00000;
}
