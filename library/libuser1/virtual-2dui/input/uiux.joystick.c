#include "libuser.h"
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);
//
void drawarrorkey2d(void*, u32, int x0, int y0, int x1, int y1, u8*, int);
void carvearrorkey(void*, u32, vec3 vc, vec3 vr, vec3 vf, u16*, int);


struct privdata{
	u16 lx;
	u16 ly;
	u16 lz;
	u16 lw;

	u16 rx;
	u16 ry;
	u16 rz;
	u16 rw;
};


static int vjoy_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static int vjoy_modify(_obj* win)
{
	return 0;
}
static int vjoy_delete(_obj* win)
{
	return 0;
}
static int vjoy_create(_obj* win, u8* str)
{
	return 0;
}




void vjoy_draw_pixel(_obj* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,m,n;
	int w = win->whdf.width;
	int h = win->whdf.height;
	//if(win->vjoyw < 0)return;

	//drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

	ch[0] = 'l';
	ch[1] = 'r';
	ch[2] = 'n';
	ch[3] = 'f';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '-';
	drawarrorkey2d(win, 0xff00ff, 0, h*13/16, h*3/16, h, ch, 1);

	ch[0] = 'x';
	ch[1] = 'b';
	ch[2] = 'a';
	ch[3] = 'y';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '+';
	drawarrorkey2d(win, 0xff00ff, w-h*3/16, h*13/16, w, h, ch, -1);
}
void vjoy_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//int w = wnd->whdf.fbwidth * area->fs.vq[0];
	//int h = wnd->whdf.fbheight * area->fs.vq[1];

	struct privdata* priv = (void*)act->priv_256b;

	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j];
		tc[j] = vc[j] - vr[j]+tr[j];
	}
	carvearrorkey(wnd, 0xff00ff, tc, tr, tf, &priv->lx,-1);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j];
		tc[j] = vc[j] + vr[j]-tr[j];
	}
	carvearrorkey(wnd, 0xff00ff, tc, tr, tf, &priv->rx, 1);
}
void vjoy_draw_html(_obj* win, struct style* sty)
{
}
void vjoy_draw_tui(_obj* win, struct style* sty)
{
}
void vjoy_draw_cli(_obj* win, struct style* sty)
{
}




int vjoy_event(struct event* ev, int x, int y, int z)
{
	//logtoall("%d,%d, z\n", x, y, z);
	short* tmp = (void*)&ev;
	int val = x + y*4;
	if('l' == z){
		switch(val){
		case  1:tmp[3] = joyl_down;break;
		case  4:tmp[3] = joyl_left;break;
		case  5:tmp[3] = joyl_thumb;break;
		case  6:tmp[3] = joyl_right;break;
		case  9:tmp[3] = joyl_up;break;
		case 11:tmp[3] = joyl_select;break;
		case 12:tmp[3] = joyl_trigger;break;
		case 14:tmp[3] = joyl_bumper;break;
		default:return 0;
		}
		ev->what = joy_left;
	}
	if('r' == z){
		switch(val){
		case  2:tmp[3] = joyr_down;break;
		case  5:tmp[3] = joyr_left;break;
		case  6:tmp[3] = joyr_thumb;break;
		case  7:tmp[3] = joyr_right;break;
		case  8:tmp[3] = joyr_start;break;
		case 10:tmp[3] = joyr_up;break;
		case 13:tmp[3] = joyr_trigger;break;
		case 15:tmp[3] = joyr_bumper;break;
		default:return 0;
		}
		ev->what = joy_right;
	}

	ev->when = 0;
	ev->where = 0;
	ev->why = 0;
	return 1;
}




static void vjoy_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	struct fstyle fs;

	switch(wnd->vfmt){
	case _bgra8888_:
	case _rgba8888_:
		break;
	case _gl41list_:
		fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.5;
		fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
		fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
		fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-0.5;
		gl41data_before(wnd);
		vjoy_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		gl41data_nocam(wnd);
		gl41data_after(wnd);
		break;
	}
}
static void vjoy_write_bywnd(_obj* ent,void* foot, _syn* stack,int sp, struct event* ev,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	struct privdata* priv = (void*)ent->priv_256b;
	printmemory(ev,16);

	if(joy_left == (ev->what&joy_mask) ){
		*(u64*)&priv->lx = ev->why;
	}
	if(joy_right == (ev->what&joy_mask) ){
		*(u64*)&priv->rx = ev->why;
	}

return;
	if('p' == (ev->what&0xff)){
		int ww,hh,x0,y0,dx,dy;
		ww = wnd->whdf.width;hh = wnd->whdf.height;
		x0 = ww * area->fs.vc[0];y0 = hh * area->fs.vc[1];
		dx = ww * area->fs.vq[0];dy = hh * area->fs.vq[1];

		short* aa = (void*)ev;
		int x = aa[0] - x0;
		int y = (hh-1-aa[1]) - y0;
		int z = -1;
		if(x<dy){
			x = x*4/dy;
			y = y*4/dy;
			z = 'l';

			struct event e;
			int ret = vjoy_event(&e, x, y, z);
			if(ret)give_data_into_peer(ent,_evto_, stack,sp, 0,0, &e,0x20);
		}
		if(x > dx-dy){
			x = (x-dx+dy)*4/dy;
			y = y*4/dy;
			z = 'r';

			struct event e;
			int ret = vjoy_event(&e, x, y, z);
			if(ret)give_data_into_peer(ent,_evto_, stack,sp, 0,0, &e,0x20);
		}
	}
}




static int vjoy_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->type){
	case _wnd_:
	case _render_:
		vjoy_read_bywnd(ent,foot, wnd,area);break;
	}
	return 0;
}
static int vjoy_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	switch(wnd->type){
	case _wnd_:
	case _render_:
		vjoy_write_bywnd(ent,foot, stack,sp, buf,len);break;
	}
	return 0;
}
static int vjoy_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vjoy_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vjoy_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('v', 'j', 'o', 'y');

	p->oncreate = (void*)vjoy_create;
	p->ondelete = (void*)vjoy_delete;
	p->onreader = (void*)vjoy_search;
	p->onwriter = (void*)vjoy_modify;

	p->onattach = (void*)vjoy_attach;
	p->ondetach = (void*)vjoy_detach;
	p->ontaking = (void*)vjoy_taking;
	p->ongiving = (void*)vjoy_giving;
}
