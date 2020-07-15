#include "libuser.h"
#define _ann_ hex32('a','n','n', 0)
#define IMAGE buf0
#define LABEL buf1
#define WEIGH buf2
int copypath(u8* path, u8* data);
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);




static int mnist_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static int mnist_modify(struct entity* act)
{
	return 0;
}
static int mnist_delete(struct entity* act)
{
	return 0;
}
static int mnist_create(struct entity* act, void* arg, int argc, u8** argv)
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
	act->buf0 = memorycreate(0x800000, 0);
	openreadclose(path, 0, act->buf0, 0x800000);

	copypath(path, label);
	act->buf1 = memorycreate(0x10000, 0);
	openreadclose(path, 0, act->buf1, 0x10000);

	act->ix0 = act->iy0 = act->iz0 = 0;
	return 0;
}




void mnist_draw_gl41_lt(struct entity* wnd, u8* img, int id, float* vc, float* vr, float* vf, float* vt)
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
	carvehexadecimal(wnd, 0xffffff, tc, tr, tf, id);
}
void mnist_draw_gl41_lb(struct entity* wnd, int val, float* vc, float* vr, float* vf, float* vt)
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
		if(y == val)carvefloat(wnd, 0xff0000, tc, tr, tf, 1.0);
		else carvefloat(wnd, 0xffffff, tc, tr, tf, 0.0);
	}
}
void mnist_draw_gl41_rt(struct entity* wnd, float* weight, float* vc, float* vr, float* vf, float* vt)
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
void mnist_draw_gl41_rb(struct entity* wnd, float* result, float* vc, float* vr, float* vf, float* vt)
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
		if(y == x)carvefloat(wnd, 0xff0000, tc, tr, tf, result[y]);
		else carvefloat(wnd, 0xffffff, tc, tr, tf, result[y]);
	}
}
void mnist_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y,z;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(wnd, 0x808080, vc, vr, vf);

	int j;
	int id = act->iz0;
	u8* image = act->buf0;
	u8* label = act->buf1;
	if(0 == image)return;
	if(0 == label)return;
	mnist_draw_gl41_lt(wnd, &image[16+id*28*28], id, vc,vr,vf,vt);
	mnist_draw_gl41_lb(wnd, label[8+id], vc,vr,vf,vt);

	float* weight = act->buf2;
	float* result = act->buf3;
	if(0 == weight)return;
	if(0 == result)return;
	if(act->iw0)give_data_into_peer(act,_ann_, 0,0, 0,0, &image[16+id*28*28], label[8+id]);
	else take_data_from_peer(act,_ann_, 0,0, 0,0, &image[16+id*28*28], label[8+id]);
	mnist_draw_gl41_rt(wnd, weight, vc,vr,vf,vt);
	mnist_draw_gl41_rb(wnd, result, vc,vr,vf,vt);

	//update weight
	if(act->iw0){
		act->iz0 = (act->iz0+1)%10000;
	}
}
void mnist_draw_pixel(struct entity* win, struct style* sty)
{
}
void mnist_draw_html(struct entity* win, struct style* sty)
{
}
void mnist_draw_tui(struct entity* win, struct style* sty)
{
}
void mnist_draw_cli(struct entity* win, struct style* sty)
{
}




static void mnist_read_bywnd(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	struct entity* wnd;struct style* area;
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
static void mnist_write_bywnd(struct entity* ent, struct event* ev)
{
//say("%x,%x\n",ev->what,ev->why);
	if(_kbd_ == ev->what){
		if(kbd_left == ev->why)ent->iz0 -= 1;
		if(kbd_right == ev->why)ent->iz0 += 1;
say("%d\n", ent->iz0);
	}
	if(_char_ == ev->what){
		if(0x20 == ev->why)ent->iw0 ^= 1;
	}
}




static int mnist_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* xxx = stack[sp-2].pchip;
	switch(xxx->fmt){
	case _gl41full_:{
		if('v' != key)break;
		mnist_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	}
	return 0;
}
static int mnist_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* xxx = stack[sp-2].pchip;
	switch(xxx->fmt){
	case _gl41full_:mnist_write_bywnd(ent,buf);break;
	}
	return 0;
}
static int mnist_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int mnist_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent;
	struct artery* art;
	if(_ann_ == self->flag){
		ent = self->pchip;
		art = peer->pchip;
		ent->buf2 = art->buf0;
		ent->buf3 = art->buf1;
	}
	return 0;
}




void mnist_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'n', 'i', 's', 't', 0, 0, 0);

	p->oncreate = (void*)mnist_create;
	p->ondelete = (void*)mnist_delete;
	p->onsearch = (void*)mnist_search;
	p->onmodify = (void*)mnist_modify;

	p->onlinkup = (void*)mnist_linkup;
	p->ondiscon = (void*)mnist_discon;
	p->ontaking = (void*)mnist_taking;
	p->ongiving = (void*)mnist_giving;
}
