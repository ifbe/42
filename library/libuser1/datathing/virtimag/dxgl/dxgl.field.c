#include "libuser.h"
#define COUNT (0x100000/36)



/*
static void field_draw_dx11(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,z;
	vec3 ta,tb;
	float tx,ty,tl;
	float ax,ay,az;
	float dx,dy,dz;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	float* vec = act->listptr.buf0;
	for(z=0;z<20;z++){
	for(y=0;y<20;y++){
	for(x=0;x<20;x++){
		//field
		ax = x/10.0-0.95;
		ay = y/10.0-0.95;
		az = z/10.0-0.95;

		ta[0] = vc[0] + vr[0]*ax + vf[0]*ay + vt[0]*az;
		ta[1] = vc[1] + vr[1]*ax + vf[1]*ay + vt[1]*az;
		ta[2] = vc[2] + vr[2]*ax + vf[2]*ay + vt[2]*az;

		tx = ta[0] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		ty = ta[1] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		tl = squareroot(ta[2]*ta[2] + tx*tx + ty*ty) / 10000.0;
		dx = tx * tl;
		dy = ty * tl;
		dz =-ta[2] * tl;
		tb[0] = ta[0] + dx - ta[1]/1000;
		tb[1] = ta[1] + dy + ta[0]/1000;
		tb[2] = ta[2] + dz;
		dx11line(wnd, 0x007f7f, ta, tb);

		ta[0] = vec[(z*20*20 + y*20 + x)*3 + 0];
		ta[1] = vec[(z*20*20 + y*20 + x)*3 + 1];
		ta[2] = vec[(z*20*20 + y*20 + x)*3 + 2];

		tx = ta[0] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		ty = ta[1] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		tl = squareroot(ta[2]*ta[2] + tx*tx + ty*ty) / 100000.0;
		dx = tx * tl;
		dy = ty * tl;
		dz =-ta[2] * tl;
		vec[(z*20*20 + y*20 + x)*3 + 0] += dx - ta[1]/1000;
		vec[(z*20*20 + y*20 + x)*3 + 1] += dy + ta[0]/1000;
		vec[(z*20*20 + y*20 + x)*3 + 2] += dz;

		dx11point(wnd, 0xffffff, ta);
	}
	}
	}
}*/
static void field_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,z;
	float tx,ty,tl;
	float ax,ay,az;
	float dx,dy,dz;
	float* vec;

	vec3 ta,tb;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vt);

	vec = act->listptr.buf0;
	for(z=0;z<20;z++){
	for(y=0;y<20;y++){
	for(x=0;x<20;x++){

		//field
		ax = x/10.0-0.95;
		ay = y/10.0-0.95;
		az = z/10.0-0.95;

		ta[0] = vc[0] + vr[0]*ax + vf[0]*ay + vt[0]*az;
		ta[1] = vc[1] + vr[1]*ax + vf[1]*ay + vt[1]*az;
		ta[2] = vc[2] + vr[2]*ax + vf[2]*ay + vt[2]*az;

		tx = ta[0] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		ty = ta[1] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		tl = squareroot(ta[2]*ta[2] + tx*tx + ty*ty) / 10000.0;
		dx = tx * tl;
		dy = ty * tl;
		dz =-ta[2] * tl;
		tb[0] = ta[0] + dx - ta[1]/1000;
		tb[1] = ta[1] + dy + ta[0]/1000;
		tb[2] = ta[2] + dz;

		gl41line_arrow(wnd, 0x007f7f, ta, tb, vt);


		//particle
		ta[0] = vec[(z*20*20 + y*20 + x)*3 + 0];
		ta[1] = vec[(z*20*20 + y*20 + x)*3 + 1];
		ta[2] = vec[(z*20*20 + y*20 + x)*3 + 2];

		tx = ta[0] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		ty = ta[1] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		tl = squareroot(ta[2]*ta[2] + tx*tx + ty*ty) / 100000.0;
		dx = tx * tl;
		dy = ty * tl;
		dz =-ta[2] * tl;
		vec[(z*20*20 + y*20 + x)*3 + 0] += dx - ta[1]/1000;
		vec[(z*20*20 + y*20 + x)*3 + 1] += dy + ta[0]/1000;
		vec[(z*20*20 + y*20 + x)*3 + 2] += dz;

		gl41point(wnd, 0xffffff, ta);
	}
	}
	}
}
static void field_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void field_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void field_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void field_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void field_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("field(%x,%x,%x)\n",win,act,sty);
}




static void field_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->hfmt){
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		field_draw_gl41(ent,slot, wor,geom, wnd,area);
		break;
	}
}




static void field_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		field_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void field_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void field_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void field_attach(struct halfrel* self, struct halfrel* peer)
{
	int x,y,z;
	float ax,ay,az;

	_obj* act = (void*)(self->chip);
	float* vec = act->listptr.buf0;

	struct fstyle* sty = (void*)(peer->foot);
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vt;

	for(z=0;z<20;z++){
	for(y=0;y<20;y++){
	for(x=0;x<20;x++){
		ax = x/10.0-0.95;
		ay = y/10.0-0.95;
		az = z/10.0-0.95;

		vec[(z*20*20 + y*20 + x)*3 + 0] = vc[0] + vr[0]*ax + vf[0]*ay + vu[0]*az;
		vec[(z*20*20 + y*20 + x)*3 + 1] = vc[1] + vr[1]*ax + vf[1]*ay + vu[1]*az;
		vec[(z*20*20 + y*20 + x)*3 + 2] = vc[2] + vr[2]*ax + vf[2]*ay + vu[2]*az;
	}
	}
	}
}




static void field_search(_obj* act)
{
}
static void field_modify(_obj* act)
{
}
static void field_delete(_obj* act)
{
	if(act->listptr.buf0){
		memorydelete(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void field_create(_obj* act)
{
	float* buf = act->listptr.buf0 = memorycreate(0x100000, 0);
}




void field_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('f', 'i', 'e', 'l', 'd', 0, 0, 0);

	p->oncreate = (void*)field_create;
	p->ondelete = (void*)field_delete;
	p->onreader = (void*)field_search;
	p->onwriter = (void*)field_modify;

	p->onattach = (void*)field_attach;
	p->ondetach = (void*)field_detach;
	p->ontaking = (void*)field_taking;
	p->ongiving = (void*)field_giving;
}
