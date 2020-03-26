#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);
//
void drawarrorkey2d(void*, u32, int x0, int y0, int x1, int y1, u8*, int);
void carvearrorkey(void*, u32, vec3 vc, vec3 vr, vec3 vf, u8*, int);




static int vjoy_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static int vjoy_modify(struct entity* win)
{
	return 0;
}
static int vjoy_delete(struct entity* win)
{
	return 0;
}
static int vjoy_create(struct entity* win, u8* str)
{
	return 0;
}




void vjoy_draw_pixel(struct entity* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,m,n;
	int w = win->width;
	int h = win->height;
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
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	u8 ch[8];
	int x,y,j;
	vec3 tc,tr,tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	int w = wnd->fbwidth * area->fs.vq[0];
	int h = wnd->fbheight * area->fs.vq[1];

	ch[0] = 'l';
	ch[1] = 'r';
	ch[2] = 'n';
	ch[3] = 'f';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '-';
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j];
		tc[j] = vc[j] - vr[j]+tr[j];
	}
	carvearrorkey(wnd, 0xff00ff, tc, tr, tf, ch,-1);

	ch[0] = 'x';
	ch[1] = 'b';
	ch[2] = 'a';
	ch[3] = 'y';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '+';
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j];
		tc[j] = vc[j] + vr[j]-tr[j];
	}
	carvearrorkey(wnd, 0xff00ff, tc, tr, tf, ch, 1);
}
void vjoy_draw_html(struct entity* win, struct style* sty)
{
}
void vjoy_draw_tui(struct entity* win, struct style* sty)
{
}
void vjoy_draw_cli(struct entity* win, struct style* sty)
{
}




int vjoy_event(struct entity* act, int x, int y, int z)
{
	int val;
	short* tmp;
	struct event ev;
	//say("%d,%d, z\n", x, y, z);

	ev.when = 0;
	ev.where = 0;
	ev.what = joy_left;
	ev.why = 0;
	tmp = (void*)&ev;
	val = x + y*4;
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
		relationwrite(act, _ev_, 0, 0, &ev, 0x20);
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
		relationwrite(act, _ev_, 0, 0, &ev, 0x20);
	}
	return 0;
}




static void vjoy_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	gl41data_before(wnd);
	vjoy_draw_gl41(cam, 0, 0,(void*)&fs, wnd,area);
	gl41data_tmpcam(wnd);
	gl41data_after(wnd);
}
static void vjoy_write_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;

	struct event* ev = buf;
	if('p' == (ev->what&0xff)){
		int ww,hh,x0,y0,dx,dy;
		ww = wnd->width;hh = wnd->height;
		x0 = ww * area->fs.vc[0];y0 = hh * area->fs.vc[1];
		dx = ww * area->fs.vq[0];dy = hh * area->fs.vq[1];

		short* aa = buf;
		int x = aa[0] - x0;
		int y = (hh-1-aa[1]) - y0;
		int z = -1;
		if(x<dy){
			x = x*4/dy;
			y = y*4/dy;
			z = 'l';
			vjoy_event(cam, x, y, z);
		}
		if(x > dx-dy){
			x = (x-dx+dy)*4/dy;
			y = y*4/dy;
			z = 'r';
			vjoy_event(cam, x, y, z);
		}
	}
}




static int vjoy_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* sup = peer->pchip;
	switch(sup->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != len)break;
		vjoy_read_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	}
	return 0;
}
static int vjoy_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* sup = peer->pchip;
	switch(sup->fmt){
		case _gl41wnd0_:vjoy_write_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static int vjoy_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vjoy_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vjoy_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('v', 'j', 'o', 'y');

	p->oncreate = (void*)vjoy_create;
	p->ondelete = (void*)vjoy_delete;
	p->onsearch = (void*)vjoy_search;
	p->onmodify = (void*)vjoy_modify;

	p->onlinkup = (void*)vjoy_linkup;
	p->ondiscon = (void*)vjoy_discon;
	p->onread  = (void*)vjoy_read;
	p->onwrite = (void*)vjoy_write;
}
