#include "libuser.h"




static u32 colortable[8] = {
0x000000, 0x0000ff, 0x00ff00, 0x00ffff,
0xff0000, 0xff00ff, 0xffff00, 0xffffff
};
static int mmioedit_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	return 0;
}
static void mmioedit_modify(_obj* act)
{
}
static void mmioedit_delete(_obj* act)
{
	if(0 == act)return;
}
static void mmioedit_create(_obj* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;
	act->listptr.buf0 = (void*)0x0;
	act->whdf.ix0 = 4;
	act->whdf.iy0 = 2;
}




static void mmioedit_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty){
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else{
		cx = wnd->whdf.width/2;
		cy = wnd->whdf.height/2;
		ww = wnd->whdf.width/2;
		hh = wnd->whdf.height/2;
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
	//logtoall("hh=%x,lpr=%x\n",hh,lineperrect);
//logtoall("bpl=%x,lpr=%x\n", byteperline, lineperrect);

	u32* addr;
	int x,y,rgb;
	int selx = act->whdf.ix0;
	int sely = act->whdf.iy0;

	for(y=0;y<lineperrect;y++){
		addr = (u32*)(act->listptr.buf0 + y*byteperline);
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
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41solid_rect(wnd, 0x00ff00, vc,vr,vf);
}
static void mmioedit_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mmioedit_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mmioedit_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mmioedit_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void mmioedit_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mmioedit_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void mmioedit_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->hfmt){
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
static int mmioedit_giving(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@mmioedit_give\n");

	struct event* ev = buf;
	if(_kbd_ == ev->what){
		switch(ev->why){
		case 0x4b:ent->whdf.ix0 -= 1;break;
		case 0x4d:ent->whdf.ix0 += 1;break;
		case 0x50:ent->whdf.iy0 += 1;break;
		case 0x48:ent->whdf.iy0 -= 1;break;
		}
	}
	return 0;
}
static void mmioedit_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void mmioedit_attach(struct halfrel* self, struct halfrel* peer)
{
}




void mmioedit_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('m','m','i','o','e','d','i','t');

	p->oncreate = (void*)mmioedit_create;
	p->ondelete = (void*)mmioedit_delete;
	p->onreader = (void*)mmioedit_search;
	p->onwriter = (void*)mmioedit_modify;

	p->onattach = (void*)mmioedit_attach;
	p->ondetach = (void*)mmioedit_detach;
	p->ontaking = (void*)mmioedit_taking;
	p->ongiving = (void*)mmioedit_giving;
}
