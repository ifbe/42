#include "libuser.h"
void dx11solid_rect(_obj* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);


struct privxyzw{
	short x0;
	short y0;
	short z0;
	short w0;
};
struct privdata{
	struct privxyzw down[1];
	struct privxyzw curr[1];
};


static int vmouse_reader(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
static int vmouse_writer(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
static int vmouse_delete(_obj* act)
{
	return 0;
}
static int vmouse_create(_obj* act, u8* str)
{
	struct privdata* priv = (void*)act->priv_256b;
	priv->down[0].w0 = 0;
	return 0;
}




void vmouse_draw_pixel(_obj* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,x0,y0,x1,y1;
	int w = win->whdf.width;
	int h = win->whdf.height;
	//if(win->vmousew < 0)return;
}
void vmouse_draw_gl41(
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

	ta[0] = (priv->curr[0].x0 / (float)wnd->whdf.width)*2-1.0;
	ta[1] = 1.0-(priv->curr[0].y0 / (float)wnd->whdf.height)*2;
	ta[2] = 0.5;
	gl41solid_circle(wnd, 0x0000ff, ta,tr,tf);

	if(priv->down[0].w0){
		tb[0] = (priv->down[0].x0 / (float)wnd->whdf.width)*2-1.0;
		tb[1] = 1.0-(priv->down[0].y0 / (float)wnd->whdf.height)*2;
		tb[2] = 0.5;
		gl41solid_circle(wnd, 0xff0000, tb,tr,tf);
		gl41line_arrow(wnd, 0xffffff, tb, ta, vt);
	}
}
void vmouse_draw_html(_obj* win, struct style* sty)
{
}
void vmouse_draw_tui(_obj* win, struct style* sty)
{
}
void vmouse_draw_cli(_obj* win, struct style* sty)
{
}



/*
static void vmouse_read_bydx11(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	dx11data_before(wnd);
	vmouse_draw_dx11(ent, 0, 0,(void*)&fs, wnd,area);
	dx11data_01cam(wnd);
	dx11data_after(wnd);
}*/
static void vmouse_wnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.5;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-0.5;

	switch(wnd->hfmt){
	case _dx11list_:
		dx11data_before(wnd);
		vmouse_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		dx11data_nocam(wnd);
		dx11data_after(wnd);
		break;
	case _gl41list_:
		gl41data_before(wnd);
		vmouse_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		gl41data_nocam(wnd);
		gl41data_after(wnd);
		break;
	case _mt20list_:
		mt20data_before(wnd);
		vmouse_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		mt20data_nocam(wnd);
		mt20data_after(wnd);
		break;
	}
}
static void vmouse_bywnd_event(_obj* ent,void* foot, _syn* stack,int sp, struct event* ev,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	//say("vmouse_bywnd_event\n");
	printmemory(ev,16);

	int type = (ev->what&0xff);
	int subt = (ev->what>>8)&0xff;
	//say("type=%x,subt=%x\n",type,subt);
	if( ('t'==type) | ('p'==type) ){
		struct privdata* priv = (void*)ent->priv_256b;
		*(u64*)priv->curr = ev->why;
		if('+' == subt){
			*(u64*)priv->down = ev->why;
			priv->down[0].w0 = 1;
		}
		else if('-' == subt){
			priv->down[0].w0 = 0;
		}
	}
}




static int vmouse_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->hfmt){
	case _dx11list_:
	case _gl41list_:
	case _mt20list_:
		vmouse_wnd(ent,foot, wnd,area);break;
	}
	return 0;
}
static int vmouse_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	switch(wnd->hfmt){
	case _dx11list_:
	case _gl41list_:
		vmouse_bywnd_event(ent,foot, stack,sp, buf,len);break;
	}
	return 0;
}
static int vmouse_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vmouse_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vmouse_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('v', 'm', 'o', 'u', 's', 'e', 0, 0);

	p->oncreate = (void*)vmouse_create;
	p->ondelete = (void*)vmouse_delete;
	p->onreader = (void*)vmouse_reader;
	p->onwriter = (void*)vmouse_writer;

	p->onattach = (void*)vmouse_attach;
	p->ondetach = (void*)vmouse_detach;
	p->ontaking = (void*)vmouse_taking;
	p->ongiving = (void*)vmouse_giving;
}
