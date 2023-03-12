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


struct privdata{
	u8* image;
	u8* label;
	float* weight;
	float* result;

	int draw_w_and_b;
	int which_to_learn;
	int back_propagation;
};


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
	struct privdata* priv = (void*)act->priv_256b;
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
	priv->image = memorycreate(0x800000, 0);
	openreadclose(path, 0, priv->image, 0x800000);

	copypath(path, label);
	priv->label = memorycreate(0x10000, 0);
	openreadclose(path, 0, priv->label, 0x10000);
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
		tc[j] = vc[j] -vr[j] +vf[j]-tf[j] +vt[j]/100.0;
	}
	gl41string(wnd, 0xffffff, tc, tr, tf, (void*)"image", 5);

	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j]-tf[j]*2 +vt[j]/100.0;
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

	for(j=0;j<3;j++){
		tr[j] = vr[j]/16;
		tf[j] = vf[j]/16;
		tc[j] = vc[j] -vr[j] -tf[j] +vt[j]/100.0;
	}
	gl41string(wnd, 0xffffff, tc, tr, tf, (void*)"label", 5);
}
void mnist_draw_gl41_rt(_obj* wnd, _obj* act, float* vc, float* vr, float* vf, float* vt)
{
	float f;
	int x,y,j;
	vec3 tc,tr,tf;

	struct privdata* priv = (void*)act->priv_256b;
	int id = priv->draw_w_and_b;
	float* weight = priv->weight;
	if(0 == weight)return;
	float* bias = &weight[28*28*10];
	for(y=0;y<28;y++){
	for(x=0;x<28;x++){
		for(j=0;j<3;j++){
			tr[j] = vr[j]/28/4;
			tf[j] = vf[j]/28/4;
			tc[j] = vc[j] +tr[j]*(2*x+1) +vf[j]/2-tf[j]*(2*y+1);
		}
		f = weight[x+y*28+id*28*28]/10.0;
		if(f<-1.0)f =-1.0;
		if(f>1.0)f = 1.0;
		if(f<0.0){
			gl41solid_rect(wnd, (int)(-255*f)*0x000001, tc, tr, tf);
		}
		else{
			gl41solid_rect(wnd, (int)(255*f)*0x010000, tc, tr, tf);
		}
	}
	}

	for(y=0;y<28;y++){
	for(x=0;x<28;x++){
		for(j=0;j<3;j++){
			tr[j] = vr[j]/28/4;
			tf[j] = vf[j]/28/4;
			tc[j] = vc[j] +tr[j]*(2*x+1+28*2) +vf[j]/2-tf[j]*(2*y+1);
		}
		f = bias[x+y*28+id*28*28]/10.0;
		if(f<-1.0)f =-1.0;
		if(f>1.0)f = 1.0;
		if(f<0.0){
			gl41solid_rect(wnd, (int)(-255*f)*0x000101, tc, tr, tf);
		}
		else{
			gl41solid_rect(wnd, (int)(255*f)*0x010100, tc, tr, tf);
		}
	}
	}

	for(j=0;j<3;j++){
		tr[j] = vr[j]/16;
		tf[j] = vf[j]/16;
		tc[j] = vc[j] +vf[j]-tf[j] +vt[j]/100.0;
	}
	gl41string(wnd, 0xffffff, tc, tr, tf, (void*)"weight", 6);

	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j]-tf[j] +vt[j]/100.0;
	}
	gl41string(wnd, 0xffffff, tc, tr, tf, (void*)"bias", 4);

	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]-tf[j]*2 +vt[j]/100.0;
	}
	gl41decimal(wnd, 0xffffff, tc, tr, tf, id);
}
void mnist_draw_gl41_rb(_obj* wnd, float* result, float* vc, float* vr, float* vf, float* vt)
{
	int x,y,j;
	vec3 tc,tr,tf;

	x = 0;
	for(y=0;y<10;y++){
		if(result[x] < result[y])x = y;
	}

	//right, bot: result
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

	for(j=0;j<3;j++){
		tr[j] = vr[j]/16;
		tf[j] = vf[j]/16;
		tc[j] = vc[j] -tf[j] +vt[j]/100.0;
	}
	gl41string(wnd, 0xffffff, tc, tr, tf, (void*)"result", 6);
}
void mnist_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct privdata* priv = (void*)act->priv_256b;
	int x,y,z;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(wnd, 0x808080, vc, vr, vf);

	int j;
	int id = priv->which_to_learn;

	u8* image = priv->image;
	if(0 == image)return;
	mnist_draw_gl41_lt(wnd, &image[16+id*28*28], id, vc,vr,vf,vt);

	u8* label = priv->label;
	if(0 == label)return;
	mnist_draw_gl41_lb(wnd, label[8+id], vc,vr,vf,vt);

	if(priv->back_propagation)give_data_into_peer(act,_ann_, 0,0, 0,0, &image[16+id*28*28], label[8+id]);
	else take_data_from_peer(act,_ann_, 0,0, 0,0, &image[16+id*28*28], label[8+id]);

	mnist_draw_gl41_rt(wnd, act, vc,vr,vf,vt);

	float* result = priv->result;
	if(0 == result)return;
	mnist_draw_gl41_rb(wnd, result, vc,vr,vf,vt);

	//update weight
	if(priv->back_propagation){
		priv->which_to_learn = (priv->which_to_learn+1)%10000;
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
	struct privdata* priv = (void*)ent->priv_256b;
//say("%x,%x\n",ev->what,ev->why);
	if(_kbd_ == ev->what){
		if(kbd_left == ev->why)priv->which_to_learn -= 1;
		if(kbd_right == ev->why)priv->which_to_learn += 1;
		say("%d\n", priv->which_to_learn);
	}
	if(_char_ == ev->what){
		if(0x20 == ev->why)priv->back_propagation ^= 1;
		if(ev->why>='0'&&ev->why<='9')priv->draw_w_and_b = ev->why - '0';
	}
}




static void mnist_byworld_bycam_byglwnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mnist_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void mnist_byworld_byglwnd(_obj* ent,void* foot, _syn* stack,int sp)
{
}
static void mnist_byglwnd(_obj* ent,void* foot, _syn* stack,int sp)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	_obj* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.5;fs.vc[1] = 0.5;fs.vc[2] = 0.0;
	fs.vr[0] = 0.5;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 0.5;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-0.5;
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
		mnist_byglwnd(ent,foot, stack,sp);
		break;
	default:
		mnist_byworld_bycam_byglwnd(ent,foot, stack,sp);
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
		struct privdata* priv = (void*)ent->priv_256b;
		priv->weight = art->WEIGHT;
		priv->result = art->RESULT;
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
