#include "libuser.h"
void gl41data_whcam(_obj* wnd, struct style* area);


float scalerange(float dstl, float dstr, float srcl, float srcr)
{
	float l = (dstr-dstl)*srcl;
	float r = (dstr-dstl)*srcr;
	return (r-l)/2;
}


static void x8664_search(_obj* act, u8* buf)
{
}
static void x8664_modify(_obj* act, u8* buf)
{
}
static void x8664_delete(_obj* act, u8* buf)
{
	//write cpustat to file

	//free 64m
}
static void x8664_create(_obj* act, void* arg, int argc, u8** argv)
{
	//alloc 64m
	act->listptr.buf0 = memoryalloc(0x100000, 0);
	if(0 == act->listptr.buf0)return;

	//read cpustat from file
}



static char* reg = "ipivdpdvr0r1r1r3";
static char* mod = "jmp\0calladd\0sub\0and\0orr\0mul\0div\0";
static void x86core_draw_gl41_onecore(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	//logtoall("%s\n", __func__);
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(ctx, 0x888888, vc,vr,vf);

	//cpucore
	int j,k;
	vec3 tc,tr,tf;
	for(j=0;j<3;j++){
		tr[j] = vr[j] / 4;
		tf[j] = vf[j] / 4;
	}
	gl41string_center(ctx, 0x888888, vc,tr,tf, (u8*)"core", 4);

	for(j=0;j<3;j++){
		tr[j] = vr[j] / 16;
		tf[j] = vf[j] / 16;
	}

	//add,sub,jmp...
	for(j=0;j<8;j++){
		for(k=0;k<3;k++)tc[k] = vc[k] + vr[k]*(2*j-7)/8 + vf[k]/2 + vu[k]*0.1;
		gl41solid_rect(ctx, 0x444444, tc, tr, tf);
		for(k=0;k<3;k++)tc[k] += vu[k]*0.1;
		gl41string_center(ctx, 0xff0000, tc, tr, tf, (u8*)mod+4*j, 4);
	}

	//ip,code,dp,data,r0,r1,r2,r3...
	for(j=0;j<8;j++){
		for(k=0;k<3;k++)tc[k] = vc[k] + vr[k]*(2*j-7)/8 - vf[k]/2 + vu[k]*0.1;
		gl41solid_rect(ctx, 0x444444, tc, tr, tf);
		for(k=0;k<3;k++)tc[k] += vu[k]*0.1;
		gl41string_center(ctx, 0xff, tc, tr, tf, (u8*)reg+2*j, 2);
	}
}
static void x86core_draw_gl41_onearch(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	//logtoall("%s\n", __func__);
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	//arch
	int j,k;
	struct fstyle fs;
	for(j=0;j<3;j++){
		fs.vr[j] = vr[j] * 0.99;
		fs.vf[j] = vf[j] * 0.99;
	}
	gl41line_rect(ctx, 0x888888, vc, fs.vr, fs.vf);

	//cpucore
	for(j=0;j<3;j++){
		fs.vr[j] = vr[j] * 0.45;
		fs.vf[j] = vf[j] * 0.45;
		fs.vt[j] = vu[j];
	}
	for(j=0;j<2;j++){
		for(k=0;k<3;k++)fs.vc[k] = vc[k] + vr[k]*(2*j-1)/2 + vf[k]/2 + vu[k]*0.1;
		x86core_draw_gl41_onecore(act,part, win,(void*)&fs, ctx,area);
	}

	//l1
	for(j=0;j<3;j++){
		fs.vf[j] = vf[j] * 0.1;
		fs.vt[j] = vu[j];
	}
	for(j=0;j<2;j++){
		for(k=0;k<3;k++){
			fs.vr[k] = vr[k] * 0.45;
			fs.vc[k] = vc[k] + vr[k]*(2*j-1)/2 - vf[k]/4 + vu[k]*0.1;
		}
		gl41line_rect(ctx, 0x777777, fs.vc, fs.vr, fs.vf);
		for(k=0;k<3;k++){
			fs.vr[k] = vr[k] * 0.25;
		}
		gl41string_center(ctx, 0x777777, fs.vc, fs.vr, fs.vf, (u8*)"l1", 2);
	}

	//l2
	for(j=0;j<3;j++){
		fs.vc[j] = vc[j] - vf[j]*3/4 + vu[j]*0.1;
		fs.vr[j] = vr[j] * 0.9;
		fs.vf[j] = vf[j] * 0.1;
		fs.vt[j] = vu[j];
	}
	gl41line_rect(ctx, 0x777777, fs.vc, fs.vr, fs.vf);
	for(j=0;j<3;j++){
		fs.vr[j] = vr[j] * 0.25;
	}
	gl41string_center(ctx, 0x777777, fs.vc, fs.vr, fs.vf, (u8*)"l2", 2);
}
static void x86core_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	//logtoall("%s\n", __func__);
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	//soc
	int j,k;
	struct fstyle fs;
	for(j=0;j<3;j++){
		fs.vr[j] = vr[j] * 0.99;
		fs.vf[j] = vf[j] * 0.99;
	}
	gl41solid_rect(ctx, 0, vc, fs.vr, fs.vf);

	//cpu
	for(j=0;j<3;j++){
		fs.vr[j] = vr[j] * 0.45;
		fs.vf[j] = vf[j] * 0.45;
		fs.vt[j] = vu[j];
	}
	for(k=0;k<3;k++)fs.vc[k] = vc[k] - vr[k]/2 + vf[k]/2 + vu[k]*0.1;
	x86core_draw_gl41_onearch(act,part, win,(void*)&fs, ctx,area);

	//gpu
	for(k=0;k<3;k++)fs.vc[k] = vc[k] + vr[k]/2 + vf[k]/2 + vu[k]*0.1;
	gl41line_rect(ctx, 0x888888, fs.vc, fs.vr, fs.vf);
	for(j=0;j<3;j++){
		fs.vr[j] = vr[j] * 0.1;
		fs.vf[j] = vf[j] * 0.1;
	}
	gl41string_center(ctx, 0x777777, fs.vc, fs.vr, fs.vf, (u8*)"gpu", 3);

	//l3
	for(j=0;j<3;j++){
		fs.vc[j] = vc[j] - vf[j]*1/4 + vu[j]*0.1;
		fs.vr[j] = vr[j] * 0.9;
		fs.vf[j] = vf[j] * 0.1;
		fs.vt[j] = vu[j];
	}
	gl41line_rect(ctx, 0x777777, fs.vc, fs.vr, fs.vf);
	for(j=0;j<3;j++){
		fs.vr[j] = vr[j] * 0.25;
	}
	gl41string_center(ctx, 0x777777, fs.vc, fs.vr, fs.vf, (u8*)"l3", 2);

	//l4/ram
	for(j=0;j<3;j++){
		fs.vc[j] = vc[j] - vf[j]*3/4 + vu[j]*0.1;
		fs.vr[j] = vr[j] * 0.9;
		fs.vf[j] = vf[j] * 0.1;
		fs.vt[j] = vu[j];
	}
	gl41line_rect(ctx, 0x777777, fs.vc, fs.vr, fs.vf);
	for(j=0;j<3;j++){
		fs.vr[j] = vr[j] * 0.25;
	}
	gl41string_center(ctx, 0x777777, fs.vc, fs.vr, fs.vf, (u8*)"l4/ram", 6);
}
static void x86core_draw_gl41_nocam(
	_obj* act, struct style* part,
	_obj* wnd, struct style* area)
{
	int j;
	struct fstyle fs;
	for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
	fs.vc[0] = scalerange(0, wnd->whdf.fbwidth , area->fs.vc[0], area->fs.vq[0]);
	fs.vc[1] = scalerange(0, wnd->whdf.fbheight, area->fs.vc[1], area->fs.vq[1]);
	fs.vr[0] = wnd->whdf.fbwidth * (area->fs.vq[0]-area->fs.vc[0]) * 0.5;
	fs.vf[1] = wnd->whdf.fbheight* (area->fs.vq[1]-area->fs.vc[1]) * 0.5;
	fs.vt[2] = 1.0;
	//logtoall("%f,%f,%f,%f\n", fs.vc[0], fs.vc[1], fs.vr[0], fs.vf[1]);

	gl41data_before(wnd);
	gl41data_whcam(wnd, area);
	x86core_draw_gl41(act,part, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);
}
static void x86core_read_bywnd(
_obj* ent,void* slot,
_obj* wnd,struct style* area,
_syn* stack,int sp)
{
	//logtoall("%s vfmt=%.4s\n", __func__, &wnd->vfmt);
	switch(wnd->vfmt){
	case _gl41list_:
		x86core_draw_gl41_nocam(ent,slot, wnd,area);
		break;
	}
}
static void x8664_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		x86core_read_bywnd(ent,slot, caller,area, stack,sp);
		break;
	}
}
static void x8664_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void x8664_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void x8664_attach(struct halfrel* self, struct halfrel* peer)
{
}




void x8664_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('x','8','6','6','4', 0, 0, 0);

	p->oncreate = (void*)x8664_create;
	p->ondelete = (void*)x8664_delete;
	p->onreader = (void*)x8664_search;
	p->onwriter = (void*)x8664_modify;

	p->onattach = (void*)x8664_attach;
	p->ondetach = (void*)x8664_detach;
	p->ontaking = (void*)x8664_taking;
	p->ongiving = (void*)x8664_giving;
}
