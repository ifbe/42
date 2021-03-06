#include "libuser.h"




static u32 colortable[8] = {
0x000000, 0x0000ff, 0x00ff00, 0x00ffff,
0xff0000, 0xff00ff, 0xffff00, 0xffffff
};
static int mmioedit_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	return 0;
}
static void mmioedit_modify(struct entity* act)
{
}
static void mmioedit_delete(struct entity* act)
{
	if(0 == act)return;
}
static void mmioedit_create(struct entity* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;
	act->buf0 = (void*)0x0;
	act->ix0 = 4;
	act->iy0 = 2;
}




static void mmioedit_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* wnd, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty){
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else{
		cx = wnd->width/2;
		cy = wnd->height/2;
		ww = wnd->width/2;
		hh = wnd->height/2;
	}
	drawsolid_rect(wnd, 0, cx-ww, cy-hh, cx+ww, cy+hh);

	int byteperline;
	if(ww*2 < 16*4)return;
	else if(ww*2 < 16*8){
		byteperline = 4;
	}
	else if(ww*2 < 16*16){
		byteperline = 8;
	}
	else if(ww*2 < 16*32){
		byteperline = 16;
	}
	else if(ww*2 < 16*64){
		byteperline = 32;
	}
	else{
		byteperline = 64;
	}

	int lineperrect;
	lineperrect = (hh*2-1)/16;
	//say("hh=%x,lpr=%x\n",hh,lineperrect);
//say("bpl=%x,lpr=%x\n", byteperline, lineperrect);

	u32* addr;
	int x,y,rgb;
	int selx = act->ix0;
	int sely = act->iy0;

	for(y=0;y<lineperrect;y++){
		addr = (u32*)(act->buf0 + y*byteperline);
		for(x=0;x<byteperline;x+=4){
			//bg
			rgb = colortable[(x>>2)%8];
			drawsolid_rect(
				wnd, ~rgb,
				cx+((x+0-byteperline/2)<<4), cy+((y+0-lineperrect/2)<<4),
				cx+((x+4-byteperline/2)<<4), cy+((y+1-lineperrect/2)<<4)
			);
			//sel
			if((selx == x)&&(sely == y)){
			drawline_rect(
				wnd, rgb,
				cx+((x+0-byteperline/2)<<4), cy+((y+0-lineperrect/2)<<4),
				cx+((x+4-byteperline/2)<<4), cy+((y+1-lineperrect/2)<<4)
			);
			}
			//fg
			drawhex32(
				wnd, rgb,
				cx+((x-byteperline/2)<<4), cy+((y-lineperrect/2)<<4),
				addr[x>>2]
			);
		}//x
	}//y
}
static void mmioedit_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41solid_rect(wnd, 0x00ff00, vc,vr,vf);
}
static void mmioedit_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mmioedit_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mmioedit_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mmioedit_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void mmioedit_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mmioedit_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void mmioedit_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _rgba_:
		mmioedit_draw_pixel(ent,slot, wnd,area);
		break;
	case _gl41list_:
		break;
	default:
		mmioedit_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static int mmioedit_giving(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@mmioedit_give\n");

	struct event* ev = buf;
	if(_kbd_ == ev->what){
		switch(ev->why){
		case 0x4b:ent->ix0 -= 1;break;
		case 0x4d:ent->ix0 += 1;break;
		case 0x50:ent->iy0 += 1;break;
		case 0x48:ent->iy0 -= 1;break;
		}
	}
	return 0;
}
static void mmioedit_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mmioedit_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void mmioedit_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m','m','i','o','e','d','i','t');

	p->oncreate = (void*)mmioedit_create;
	p->ondelete = (void*)mmioedit_delete;
	p->onsearch = (void*)mmioedit_search;
	p->onmodify = (void*)mmioedit_modify;

	p->onlinkup = (void*)mmioedit_linkup;
	p->ondiscon = (void*)mmioedit_discon;
	p->ontaking = (void*)mmioedit_taking;
	p->ongiving = (void*)mmioedit_giving;
}
