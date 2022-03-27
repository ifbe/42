#include "libuser.h"
#define _ann_ hex32('a','n','n', 0)
#define WEIGHT listptr.buf0
#define RESULT listptr.buf1
#define IMAGE listptr.buf2
#define LABEL listptr.buf3
int copypath(u8* path, u8* data);
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);




static int mnist_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static int mnist_modify(_obj* act)
{
	return 0;
}
static int mnist_delete(_obj* act)
{
	return 0;
}
static int mnist_create(_obj* act, void* arg, int argc, u8** argv)
{
	int j;
	u8 path[128];
	u8* image = 0;
	u8* label = 0;
	for(j=1;j<argc;j++){
		if(0 == ncmp(argv[j], "image:", 6))image = argv[j]+6;
		if(0 == ncmp(argv[j], "label:", 6))label = argv[j]+6;
	}
	if(0 == image)return 0;
	if(0 == label)return 0;

	copypath(path, image);
	act->IMAGE = memorycreate(0x800000, 0);
	openreadclose(path, 0, act->IMAGE, 0x800000);

	copypath(path, label);
	act->LABEL = memorycreate(0x10000, 0);
	openreadclose(path, 0, act->LABEL, 0x10000);

	act->whdf.ix0 = act->whdf.iy0 = act->whdf.iz0 = 0;
	return 0;
}




void mnist_draw_gl41_lt(_obj* wnd, u8* img, int id, float* vc, float* vr, float* vf, float* vt)
{
	int x,y,j;
	vec3 tc,tr,tf;

	//left, top: image
	for(y=0;y<28;y++){
	for(x=0;x<28;x++){
		for(j=0;j<3;j++){
			tr[j] = vr[j]/28/2;
			tf[j] = vf[j]/28/2;
			tc[j] = vc[j] -vr[j]+tr[j]*(2*x+1) +vf[j]-tf[j]*(2*y+1);
		}
		gl41solid_rect(wnd, 0x010101*img[x+y*28], tc, tr, tf);
	}
	}
	for(j=0;j<3;j++){
		tr[j] = vr[j]/16;
		tf[j] = vf[j]/16;
		tc[j] = vc[j] -vr[j] +vf[j]-tf[j]+vt[j]/100.0;
	}
	gl41hexadecimal(wnd, 0xffffff, tc, tr, tf, id);
}
void mnist_draw_gl41_lb(_obj* wnd, int val, float* vc, float* vr, float* vf, float* vt)
{
	int y,j;
	vec3 tc,tr,tf;

	//left, bot: label
	for(y=0;y<10;y++){
		for(j=0;j<3;j++){
			tr[j] = vr[j]/10;
			tf[j] = vf[j]/10;
			tc[j] = vc[j] -vr[j]/2 -tf[j]*(y+1);
		}
		//gl41solid_rect(wnd, 0x400000, tc, tr, tf);
		if(y == val)gl41float(wnd, 0xff0000, tc, tr, tf, 1.0);
		else gl41float(wnd, 0xffffff, tc, tr, tf, 0.0);
	}
}
void mnist_draw_gl41_rt(_obj* wnd, float* weight, float* vc, float* vr, float* vf, float* vt)
{
	int x,y,j;
	vec3 tc,tr,tf;

	//right,top: weight
	for(y=1;y<10;y++){
		for(j=0;j<3;j++){
			tr[j] = vr[j]/4;
			tf[j] = vf[j]/4;
			tc[j] = vc[j] +vr[j]/4+tr[j]*y/4.5 +vf[j]/4+tf[j]*y/4.5 -vt[j]*y/100.0;
		}
		gl41solid_rect(wnd, 0x101010*(12-y), tc,tr,tf);
		for(j=0;j<3;j++)tc[j] += vt[j]/1000000.0;
		gl41ascii_center(wnd, 0, tc,tr,tf, 0x30+y);
	}
if(0 == weight)return;

	for(y=0;y<28;y++){
	for(x=0;x<28;x++){
		for(j=0;j<3;j++){
			tr[j] = vr[j]/28/4;
			tf[j] = vf[j]/28/4;
			tc[j] = vc[j] +tr[j]*(2*x+1) +vf[j]/2-tf[j]*(2*y+1);
		}
		gl41solid_rect(wnd, (int)(255*weight[x+y*28])*0x010101, tc, tr, tf);
	}
	}
}
void mnist_draw_gl41_rb(_obj* wnd, float* result, float* vc, float* vr, float* vf, float* vt)
{
	int x,y,j;
	vec3 tc,tr,tf;

	x = 0;
	for(y=0;y<10;y++){
		if(result[x] < result[y])x = y;
	}

	//left, bot: label
	for(y=0;y<10;y++){
		for(j=0;j<3;j++){
			tr[j] = vr[j]/10;
			tf[j] = vf[j]/10;
			tc[j] = vc[j] +vr[j]/2 -tf[j]*(y+1);
		}
		//gl41solid_rect(wnd, 0x400000, tc, tr, tf);
		if(y == x)gl41float(wnd, 0xff0000, tc, tr, tf, result[y]);
		else gl41float(wnd, 0xffffff, tc, tr, tf, result[y]);
	}
}
void mnist_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,z;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(wnd, 0x808080, vc, vr, vf);

	int j;
	int id = act->whdf.iz0;
	u8* image = act->IMAGE;
	u8* label = act->LABEL;
	if(0 == image)return;
	if(0 == label)return;
	mnist_draw_gl41_lt(wnd, &image[16+id*28*28], id, vc,vr,vf,vt);
	mnist_draw_gl41_lb(wnd, label[8+id], vc,vr,vf,vt);

	float* weight = act->WEIGHT;
	float* result = act->RESULT;
	if(0 == weight)return;
	if(0 == result)return;
	if(act->whdf.iw0)give_data_into_peer(act,_ann_, 0,0, 0,0, &image[16+id*28*28], label[8+id]);
	else take_data_from_peer(act,_ann_, 0,0, 0,0, &image[16+id*28*28], label[8+id]);
	mnist_draw_gl41_rt(wnd, weight, vc,vr,vf,vt);
	mnist_draw_gl41_rb(wnd, result, vc,vr,vf,vt);

	//update weight
	if(act->whdf.iw0){
		act->whdf.iz0 = (act->whdf.iz0+1)%10000;
	}
}
void mnist_draw_pixel(_obj* win, struct style* sty)
{
}
void mnist_draw_html(_obj* win, struct style* sty)
{
}
void mnist_draw_tui(_obj* win, struct style* sty)
{
}
void mnist_draw_cli(_obj* win, struct style* sty)
{
}




static void mnist_write_bywnd(_obj* ent, struct event* ev)
{
//say("%x,%x\n",ev->what,ev->why);
	if(_kbd_ == ev->what){
		if(kbd_left == ev->why)ent->whdf.iz0 -= 1;
		if(kbd_right == ev->why)ent->whdf.iz0 += 1;
say("%d\n", ent->whdf.iz0);
	}
	if(_char_ == ev->what){
		if(0x20 == ev->why)ent->whdf.iw0 ^= 1;
	}
}




static void mnist_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mnist_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void mnist_wrl_ywnd(_obj* ent,void* foot, _syn* stack,int sp)
{
}
static void mnist_wnd(_obj* ent,void* foot, _syn* stack,int sp)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	_obj* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;
	gl41data_before(wnd);
	mnist_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}




static void mnist_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		mnist_wnd(ent,foot, stack,sp);
		break;
	default:
		mnist_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static int mnist_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* xxx = stack[sp-2].pchip;
	switch(xxx->hfmt){
	case _gl41list_:mnist_write_bywnd(ent,buf);break;
	}
	return 0;
}
static int mnist_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int mnist_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent;
	_obj* art;
	if(_ann_ == self->foottype){
		ent = self->pchip;
		art = peer->pchip;
		ent->WEIGHT = art->WEIGHT;
		ent->RESULT = art->RESULT;
	}
	return 0;
}




void mnist_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('m', 'n', 'i', 's', 't', 0, 0, 0);

	p->oncreate = (void*)mnist_create;
	p->ondelete = (void*)mnist_delete;
	p->onreader = (void*)mnist_search;
	p->onwriter = (void*)mnist_modify;

	p->onattach = (void*)mnist_attach;
	p->ondetach = (void*)mnist_detach;
	p->ontaking = (void*)mnist_taking;
	p->ongiving = (void*)mnist_giving;
}
