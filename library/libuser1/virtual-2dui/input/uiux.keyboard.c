#include "libuser.h"
void dx11solid_rect(_obj* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);


struct privxyzw{
	short x0;
	short y0;
	short z0;
	short w0;
	short xn;
	short yn;
	short zn;
	short wn;
};
struct privdata{
	struct privxyzw mouse[1];
	struct privxyzw touch[1];
};


static int vkbd_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static int vkbd_modify(_obj* act)
{
	return 0;
}
static int vkbd_delete(_obj* act)
{
	return 0;
}
static int vkbd_create(_obj* act, u8* str)
{
	struct privdata* priv = (void*)act->priv_256b;
	priv->mouse[0].w0 = 0;
	priv->touch[0].w0 = 0;
	return 0;
}




void vkbd_draw_pixel(_obj* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,x0,y0,x1,y1;
	int w = win->whdf.width;
	int h = win->whdf.height;
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
			drawopaque_rect(win, 0x7fffffff, x0, y0, x1, y1);
			drawstring_fit(win, 0, x0, y0, x1, y1, (u8*)&c, l);
		}
	}
}/*
void vkbd_draw_dx11(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,j,dat;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
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
			if((act->whdf.iw0)&&(x == act->whdf.ix0)&&(y == act->whdf.iy0)){
				dx11solid_rect(wnd, 0x80ff0000, tc, tr, tf);
			}
			else{
				dx11line_rect(wnd, 0x80808080, tc, tr, tf);
			}

			dat = x+(y<<4);
			switch(dat){
				case 0x0:dat = '0';break;
				case 0x7:dat = 'a';break;
				case 0x8:dat = 'b';break;
				case 0x9:dat = 't';break;
				case 0xa:dat = 'n';break;
				case 0xd:dat = 'r';break;
			}

			for(j=0;j<3;j++)tc[j] += vt[j]*0.01;
			dx11ascii_center(wnd, 0xffffff, tc, tr, tf, dat);
		}
	}
}*/
void vkbd_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,j;
	int dat,flag;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//gl41opaque_rect(wnd, 0x800000ff, vc, vr, vf);

	struct privdata* priv = (void*)act->priv_256b;
	for(y=0;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			for(j=0;j<3;j++){
				tr[j] = vr[j]/17;
				tf[j] = vf[j]/8.5;
				tc[j] = vc[j] + vr[j]*(x-7.5)/8.0 + vf[j]*(y-3.5)/4.0;
			}
			if( (priv->mouse[0].w0)&&(x == priv->mouse[0].x0)&&(y == priv->mouse[0].y0) ){
				gl41opaque_rect(wnd, 0x80ff0000, tc, tr, tf);
			}
			else if( (priv->touch[0].w0)&&(x == priv->touch[0].x0)&&(y == priv->touch[0].y0) ){
				gl41opaque_rect(wnd, 0x800000ff, tc, tr, tf);
			}
			else{
				gl41line_rect(wnd, 0x80808080, tc, tr, tf);
			}

			dat = x+(y<<4);
			flag = 0;
			switch(dat){
				case 0x0:dat = '0';flag = 1;break;
				case 0x7:dat = 'a';flag = 1;break;
				case 0x8:dat = 'b';flag = 1;break;
				case 0x9:dat = 't';flag = 1;break;
				case 0xa:dat = 'n';flag = 1;break;
				case 0xd:dat = 'r';flag = 1;break;
			}//switch

			if(flag){
				for(j=0;j<3;j++)tc[j] -= vr[j]/32;
				gl41ascii_center(wnd, 0xffffff, tc, tr, tf, '\\');
				for(j=0;j<3;j++)tc[j] += vr[j]/16;
				gl41ascii_center(wnd, 0xffffff, tc, tr, tf, dat);
			}
			else{
				gl41ascii_center(wnd, 0xffffff, tc, tr, tf, dat);
			}
		}//forx
	}//fory
}
void vkbd_draw_html(_obj* win, struct style* sty)
{
}
void vkbd_draw_tui(_obj* win, struct style* sty)
{
}
void vkbd_draw_cli(_obj* win, struct style* sty)
{
}




static int vkbd_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	short tmp[4];
	int x,y,w,h,ret;
	//logtoall("vkbd_keyboard_write\n");
	//if(win->vkbdw <= 0)return 0;

	w = win->whdf.width;
	h = win->whdf.height;
	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y < h*3/4)return 0;

	if(hex32('p','-',0,0) == ev->what)
	{
		x = 16*x/w;
		y = 31 - 32*y/h;
		//logtoall("x=%d,y=%d\n",x,y);
		eventwrite(x+(y*16), _char_, (u64)win, 0);
	}
	return 1;
}



/*
static void vkbd_read_bydx11(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	dx11data_before(wnd);
	vkbd_draw_dx11(ent, 0, 0,(void*)&fs, wnd,area);
	dx11data_01cam(wnd);
	dx11data_after(wnd);
}*/
static void vkbd_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.5;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-0.5;

	switch(wnd->vfmt){
	case _bgra8888_:
	case _rgba8888_:
		break;
	case _dx11list_:
		dx11data_before(wnd);
		vkbd_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		dx11data_nocam(wnd);
		dx11data_after(wnd);
		break;
	case _gl41list_:
		gl41data_before(wnd);
		vkbd_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		gl41data_nocam(wnd);
		gl41data_after(wnd);
		break;
	case _mt20list_:
		mt20data_before(wnd);
		vkbd_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		mt20data_nocam(wnd);
		mt20data_after(wnd);
		break;
	}
}
static void vkbd_bywnd_event(_obj* ent,void* foot, _syn* stack,int sp, struct event* ev,int len)
{
	_obj* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	//logtoall("vkbd_bywnd_event\n");
	//printmemory(ev,16);

	if(_char_ == ev->what){
		give_data_into_peer(ent,_evto_, stack,sp, 0,0, ev,0x20);
		return;
	}
	int type = (ev->what&0xff);
	if( ('t'==type) | ('p'==type) ){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);

		struct privdata* priv = (void*)ent->priv_256b;
		if('p' == type){
			priv->mouse[0].x0 = xyz[0] * 16;
			priv->mouse[0].y0 = xyz[1] * 8;

			if(point_onto == ev->what)priv->mouse[0].w0 = 1;
			if(point_away == ev->what){
				priv->mouse[0].w0 = 0;

				struct event tmp;
				int x = priv->mouse[0].x0;
				int y = priv->mouse[0].y0;
				tmp.why = x + y*16;
				tmp.what = _char_;
				give_data_into_peer(ent,_evto_, stack,sp, 0,0, &tmp,0x20);
			}
			return;
		}
		if('t' == type){
			priv->touch[0].x0 = xyz[0] * 16;
			priv->touch[0].y0 = xyz[1] * 8;

			if(touch_onto == ev->what)priv->touch[0].w0 = 1;
			if(touch_away == ev->what){
				priv->touch[0].w0 = 0;

				struct event tmp;
				int x = priv->touch[0].x0;
				int y = priv->touch[0].y0;
				tmp.why = x + y*16;
				tmp.what = _char_;
				give_data_into_peer(ent,_evto_, stack,sp, 0,0, &tmp,0x20);
			}
			return;
		}
	}
}




static int vkbd_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->type){
	case _wnd_:
	case _render_:
		vkbd_read_bywnd(ent,foot, wnd,area);
		break;
	}
	return 0;
}
static int vkbd_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	switch(wnd->type){
	case _wnd_:
		if(0 == cmd)vkbd_bywnd_event(ent,foot, stack,sp, buf,len);break;
	}
	return 0;
}
static int vkbd_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vkbd_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vkbd_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('v', 'k', 'b', 'd');

	p->oncreate = (void*)vkbd_create;
	p->ondelete = (void*)vkbd_delete;
	p->onreader = (void*)vkbd_search;
	p->onwriter = (void*)vkbd_modify;

	p->onattach = (void*)vkbd_attach;
	p->ondetach = (void*)vkbd_detach;
	p->ontaking = (void*)vkbd_taking;
	p->ongiving = (void*)vkbd_giving;
}
