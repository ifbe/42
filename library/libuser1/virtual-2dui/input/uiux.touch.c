#include "libuser.h"
void dx11solid_rect(_obj* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);


struct privxyzw{
	short x0;
	short y0;
	short z0;
	short w0;
};
struct finger{
	struct privxyzw down;
	struct privxyzw curr;
};
struct privdata{
	struct finger f[8];
};


static int vtouch_reader(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
static int vtouch_writer(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
static int vtouch_delete(_obj* act)
{
	return 0;
}
static int vtouch_create(_obj* act, u8* str)
{
	struct privdata* priv = (void*)act->priv_256b;
	int j=0;
	for(j=0;j<8;j++)priv->f[j].down.w0 = 0;
	return 0;
}




void vtouch_draw_pixel(_obj* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,x0,y0,x1,y1;
	int w = win->whdf.width;
	int h = win->whdf.height;
	//if(win->vtouchw < 0)return;
}
void vtouch_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(wnd, 0xffffff, vc, vr, vf);

	vec4 ta,tb,tr,tf;
	int j;
	struct privdata* priv = (void*)act->priv_256b;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/20;
		tf[j] = vf[j]/20;
	}

	int id;
	for(id=0;id<8;id++){
		ta[0] = (priv->f[id].curr.x0 / (float)wnd->whdf.width)*2-1.0;
		ta[1] = 1.0-(priv->f[id].curr.y0 / (float)wnd->whdf.height)*2;
		ta[2] = 0.5;
		gl41solid_circle(wnd, 0x0000ff, ta,tr,tf);

		if(priv->f[id].down.w0){
			tb[0] = (priv->f[id].down.x0 / (float)wnd->whdf.width)*2-1.0;
			tb[1] = 1.0-(priv->f[id].down.y0 / (float)wnd->whdf.height)*2;
			tb[2] = 0.5;
			gl41solid_circle(wnd, 0xff0000, tb,tr,tf);
			gl41line_arrow(wnd, 0xffffff, tb, ta, vt);
		}
	}
}
void vtouch_draw_html(_obj* win, struct style* sty)
{
}
void vtouch_draw_tui(_obj* win, struct style* sty)
{
}
void vtouch_draw_cli(_obj* win, struct style* sty)
{
}



/*
static void vtouch_read_bydx11(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	dx11data_before(wnd);
	vtouch_draw_dx11(ent, 0, 0,(void*)&fs, wnd,area);
	dx11data_01cam(wnd);
	dx11data_after(wnd);
}*/
static void vtouch_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
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
		vtouch_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		dx11data_nocam(wnd);
		dx11data_after(wnd);
		break;
	case _gl41list_:
		gl41data_before(wnd);
		vtouch_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		gl41data_nocam(wnd);
		gl41data_after(wnd);
		break;
	case _mt20list_:
		mt20data_before(wnd);
		vtouch_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		mt20data_nocam(wnd);
		mt20data_after(wnd);
		break;
	}
}
static void vtouch_bywnd_event(_obj* ent,void* foot, _syn* stack,int sp, struct event* ev,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	//logtoall("vtouch_bywnd_event\n");
	printmemory(ev,16);

	int type = (ev->what&0xff);
	int subt = (ev->what>>8)&0xff;
	//logtoall("type=%x,subt=%x\n",type,subt);
	if(0){
	}
	else if('t'==type){
		struct privdata* priv = (void*)ent->priv_256b;

		short* data = (void*)&ev->why;
		int id = data[3]&0x7;
		*(u64*)&priv->f[id].curr = ev->why;

		if('+' == subt){
			*(u64*)&priv->f[id].down = ev->why;
			priv->f[id].down.w0 = 1;
		}
		else if('-' == subt){
			priv->f[id].down.w0 = 0;
		}
	}
	else if('p'==type){
		struct privdata* priv = (void*)ent->priv_256b;
		*(u64*)&priv->f[0].curr = ev->why;
		if('+' == subt){
			*(u64*)&priv->f[0].down = ev->why;
			priv->f[0].down.w0 = 1;
		}
		else if('-' == subt){
			priv->f[0].down.w0 = 0;
		}
	}
}




static int vtouch_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->type){
	case _wnd_:
	case _render_:
		vtouch_read_bywnd(ent,foot, wnd,area);break;
	}
	return 0;
}
static int vtouch_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	switch(wnd->type){
	case _wnd_:
	case _render_:
		vtouch_bywnd_event(ent,foot, stack,sp, buf,len);break;
	}
	return 0;
}
static int vtouch_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vtouch_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vtouch_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('v', 't', 'o', 'u', 'c', 'h', 0, 0);

	p->oncreate = (void*)vtouch_create;
	p->ondelete = (void*)vtouch_delete;
	p->onreader = (void*)vtouch_reader;
	p->onwriter = (void*)vtouch_writer;

	p->onattach = (void*)vtouch_attach;
	p->ondetach = (void*)vtouch_detach;
	p->ontaking = (void*)vtouch_taking;
	p->ongiving = (void*)vtouch_giving;
}
