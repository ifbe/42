#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




static int vkbd_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static int vkbd_modify(struct entity* act)
{
	return 0;
}
static int vkbd_delete(struct entity* act)
{
	return 0;
}
static int vkbd_create(struct entity* act, u8* str)
{
	act->iw0 = 0;
	return 0;
}




void vkbd_draw_pixel(struct entity* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,x0,y0,x1,y1;
	int w = win->width;
	int h = win->height;
	//if(win->vkbdw < 0)return;

	//drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

	for(y=0;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			l = 2;
			c = x+(y<<4);
			//if(c == (win->vkbdz))rgb = 0xffff00ff;
			//else rgb = 0x20808080;

			//joystick area
			if((y>8)&&(y<15))
			{
				if((x>0)&&(x<7))continue;
				if((x>8)&&(x<15))continue;
			}

			if(0x0 == c)c = hex32('\\','0',0,0);
			else if(0x7 == c)c = hex32('\\','a',0,0);
			else if(0x8 == c)c = hex32('\\','b',0,0);
			else if(0x9 == c)c = hex32('\\','t',0,0);
			else if(0xa == c)c = hex32('\\','n',0,0);
			else if(0xd == c)c = hex32('\\','r',0,0);
			else if(0xf0 <= c)
			{
				if(0xfa <= c)
				{
					l = 3;
					c = ((c-0xfa)<<16) + hex32('f','1','0',0);
				}
				else
				{
					l = 2;
					c = ((c-0xf0)<<8) + hex32('f','0',0,0);
				}
			}
			else if(0x80 <= c)
			{
				l = 1;
				c = ' ';
			}
			else l = 1;

			x0 = (x*w/16)+1;
			y0 = h+1-((y+1)*h/32)+1;
			x1 = ((x+1)*w/16)-1;
			y1 = h-1-(y*h/32);
			drawhyaline_rect(win, 0x7fffffff, x0, y0, x1, y1);
			drawstring_fit(win, rgb, x0, y0, x1, y1, (u8*)&c, l);
		}
	}
}
void vkbd_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y,j;
	int c,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	//gl41opaque_rect(wnd, 0x800000ff, vc, vr, vf);

	for(y=0;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			for(j=0;j<3;j++){
				tr[j] = vr[j]/17;
				tf[j] = vf[j]/8.5;
				tc[j] = vc[j] + vr[j]*(x-7.5)/8.0 + vf[j]*(y-3.5)/4.0;
			}
			rgb = 0x80808080;
			if((act->iw0)&&(x == act->ix0)&&(y == act->iy0))rgb = 0x80ff0000;
			gl41opaque_rect(wnd, rgb, tc, tr, tf);

			c = x+(y<<4);
			if((0==c)|(7==c)|(8==c)|(9==c)|(0xa==c)|(0xd==c))
			{
				if(0x0 == c)c = '0';
				else if(0x7 == c)c = 'a';
				else if(0x8 == c)c = 'b';
				else if(0x9 == c)c = 't';
				else if(0xa == c)c = 'n';
				else if(0xd == c)c = 'r';
			}

			carveascii_center(wnd, 0xffffff, tc, tr, tf, c);
		}
	}
}
void vkbd_draw_html(struct entity* win, struct style* sty)
{
}
void vkbd_draw_tui(struct entity* win, struct style* sty)
{
}
void vkbd_draw_cli(struct entity* win, struct style* sty)
{
}




static int vkbd_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	short tmp[4];
	int x,y,w,h,ret;
	//say("vkbd_keyboard_write\n");
	//if(win->vkbdw <= 0)return 0;

	w = win->width;
	h = win->height;
	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y < h*3/4)return 0;

	if(hex32('p','-',0,0) == ev->what)
	{
		x = 16*x/w;
		y = 31 - 32*y/h;
		//say("x=%d,y=%d\n",x,y);
		eventwrite(x+(y*16), _char_, (u64)win, 0);
	}
	return 1;
}




static void vkbd_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-1.0;

	gl41data_before(wnd);
	vkbd_draw_gl41(cam, 0, 0,(void*)&fs, wnd,area);
	gl41data_tmpcam(wnd);
	gl41data_after(wnd);
}
static void vkbd_write_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* wnd;struct style* area;
	struct entity* ent;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	ent = self->pchip;gl41 = self->pfoot;

	struct event* ev = buf;
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		ent->ix0 = xyz[0] * 16;
		ent->iy0 = xyz[1] * 8;

		if(0x2b70 == ev->what)ent->iw0 = 1;
		if(0x2d70 == ev->what){
			ent->iw0 = 0;

			struct event tmp;
			int x = ent->ix0;
			int y = ent->iy0;
			tmp.why = x + y*16;
			tmp.what = _char_;
			relationwrite(ent, _ev_, 0, 0, &tmp, 0x20);
		}
	}
}




static int vkbd_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent = peer->pchip;
	switch(ent->fmt){
		case _gl41wnd0_:vkbd_read_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static int vkbd_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent = peer->pchip;
	switch(ent->fmt){
		case _gl41wnd0_:vkbd_write_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static int vkbd_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vkbd_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vkbd_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('v', 'k', 'b', 'd');

	p->oncreate = (void*)vkbd_create;
	p->ondelete = (void*)vkbd_delete;
	p->onsearch = (void*)vkbd_search;
	p->onmodify = (void*)vkbd_modify;

	p->onlinkup = (void*)vkbd_linkup;
	p->ondiscon = (void*)vkbd_discon;
	p->onread  = (void*)vkbd_read;
	p->onwrite = (void*)vkbd_write;
}
