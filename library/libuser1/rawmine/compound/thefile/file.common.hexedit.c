#include "libuser.h"
int utf2unicode(u8* src,u32* dst);
int windowread(int type, void* buf);
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);




static u8* universe = 0;




static int hexedit_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
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
				drawbyte(win, 0xffffff, cx+x, cy+y, universe[j]);
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
	//carvesolid_rect(wnd, 0x00ff00, vc,vr,vf);

	u8 ch;
	u8* buf = act->buf0;
	for(y=0;y<32;y++){
		for(x=0;x<32;x++){
			for(j=0;j<3;j++){
				tr[j] = vr[j]/32;
				tf[j] = vf[j]/32;
				tc[j] = vc[j] + tr[j]*(2*x-31) + tf[j]*(31-2*y);
			}
			carveline_rect(wnd, 0xff0000, tc,tr,tf);

			ch = buf[act->iz0 + y*32 + x];
			if((x == act->ixn)&&(y == act->iyn))rgb = 0x00ff00;
			else rgb = 0xffffff;
			if(act->iw0)carveascii_center(wnd, rgb, tc,tr,tf,ch);
			else carvehex8_center(wnd, rgb, tc,tr,tf,ch);
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
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"hex\" style=\"width:50%%;height:100px;float:left;background-color:#a8a7d9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
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
static void hexedit_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)hexedit_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)hexedit_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)hexedit_draw_html(act, pin, win, sty);
	else if(fmt == _json_)hexedit_draw_json(act, pin, win, sty);
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




static void hexedit_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> texball
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;
//say("@freecam_read_byeye:%c\n",len);

	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;slot = self->pfoot;
		if('v' == len)hexedit_draw_gl41(act,slot, wrd,geom, wnd,area);
	}
//say("@freecam_read_byeye.end\n");
}
static void hexedit_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
	gl41data_before(wnd);
	hexedit_draw_gl41(cam, 0, 0,(void*)&fs, wnd,area);
	gl41data_tmpcam(wnd);
	gl41data_after(wnd);
}
static int hexedit_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* sup = peer->pchip;
	switch(sup->fmt){
		case _gl41wnd0_:hexedit_read_bywnd(self, peer, stack, rsp, buf, len);break;
		default:        hexedit_read_bycam(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static void hexedit_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* ent = self->pchip;
	struct supply* sup = peer->pchip;
	switch(sup->fmt){
		case _gl41wnd0_:hexedit_event(ent, self->pfoot, sup, peer->pfoot, buf);break;
	}
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
	p->onread  = (void*)hexedit_read;
	p->onwrite = (void*)hexedit_write;
}
void hexedit_init(void* addr)
{
	universe = addr - 0xe00000;
}
