#include "libuser.h"
int copypath(u8* path, u8* data);
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);


struct privdata{
	u8* model;
	u8* token;
/*
dim=288
hidden_dim=768
n_layers=6
n_heads=6
n_kv_heads=6
vocab_size=32000
*/
	int n_layers;
};


static int transformer_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static int transformer_modify(_obj* act)
{
	return 0;
}
static int transformer_delete(_obj* act)
{
	return 0;
}
static int transformer_create(_obj* act, void* arg, int argc, u8** argv)
{
	u8 path[128];
	u8* model = 0;
	u8* token = 0;

	struct privdata* priv = (void*)act->priv_256b;
	priv->n_layers = 32;

	int j;
	for(j=1;j<argc;j++){
		if(0 == ncmp(argv[j], "model:", 6))model = argv[j]+6;
		if(0 == ncmp(argv[j], "token:", 6))token = argv[j]+6;
	}
	if(0 == model)return 0;
	if(0 == token)return 0;
/*
	copypath(path, image);
	priv->image = memoryalloc(0x800000, 0);
	openreadclose(path, 0, priv->image, 0x800000);

	copypath(path, label);
	priv->label = memoryalloc(0x10000, 0);
	openreadclose(path, 0, priv->label, 0x10000);
*/
	return 0;
}



void transformer_draw_gl41_input(_obj* wnd, _obj* ent, float* vc, float* vr, float* vf, float* vt)
{
	vec3 tc,tr,tf;
	int j;
	for(j=0;j<3;j++){
		tr[j] = vr[j]*0.3;
		tf[j] = vf[j]*0.05;
		tc[j] = vc[j] + vf[j]*0.9 + vt[j]/100;
	}
	gl41solid_rect(wnd, 0x800000, tc, tr, tf);
}
/*
cnt=2: -1/2, 1/2
cnt=3: -2/3, 0, 2/3
cnt=4: -3/4, -1/4, 1/4, 3/4
cnt=5: -4/5, -2/5, 0, 2/5, 4/5
cnt=6: -5/6, -3/6, -1/6, 1/6, 3/6, 5/6
*/
void transformer_draw_gl41_block(_obj* wnd, _obj* ent, float* vc, float* vr, float* vf, float* vt, int layer, int layercount)
{
	vec3 tc,tr,tf;
	int j;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/2;
		tf[j] = vf[j]*0.8/layercount;
		tc[j] = vc[j] + 0.8*vf[j]*(2*layer+1-layercount)/layercount + vt[j]/100;
		tf[j] *= 0.9;
	}
	gl41solid_rect(wnd, 0x008000, tc, tr, tf);
}
void transformer_draw_gl41_logits(_obj* wnd, _obj* ent, float* vc, float* vr, float* vf, float* vt)
{
	vec3 tc,tr,tf;
	int j;
	for(j=0;j<3;j++){
		tr[j] = vr[j]*0.8;
		tf[j] = vf[j]*0.05;
		tc[j] = vc[j] - vf[j]*0.9 + vt[j]/100;
	}
	gl41solid_rect(wnd, 0x000080, tc, tr, tf);
}
void transformer_draw_gl41(
	_obj* ent, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41solid_rect(wnd, 0, vc, vr, vf);

	struct privdata* priv = (void*)ent->priv_256b;
	int layercount = priv->n_layers;

	transformer_draw_gl41_input(wnd,ent, vc,vr,vf,vt);
	int j;
	for(j=0;j<layercount;j++){
		transformer_draw_gl41_block(wnd,ent, vc,vr,vf,vt, j, layercount);
	}
	transformer_draw_gl41_logits(wnd,ent, vc,vr,vf,vt);
}
void transformer_draw_pixel(_obj* win, struct style* sty)
{
}
void transformer_draw_html(_obj* win, struct style* sty)
{
}
void transformer_draw_tui(_obj* win, struct style* sty)
{
}
void transformer_draw_cli(_obj* win, struct style* sty)
{
}




static void transformer_write_bywnd(_obj* ent, struct event* ev)
{
	struct privdata* priv = (void*)ent->priv_256b;
	if(_kbd_ == ev->what){
	}
}




static void transformer_byworld_bycam_byglwnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	transformer_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void transformer_byworld_byglwnd(_obj* ent,void* foot, _syn* stack,int sp)
{
}
static void transformer_byglwnd(_obj* ent,void* foot, _syn* stack,int sp)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	_obj* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.5;fs.vc[1] = 0.5;fs.vc[2] = 0.0;
	fs.vr[0] = 0.5;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 0.5;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 0.5;
	gl41data_before(wnd);
	transformer_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}




static void transformer_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		transformer_byglwnd(ent,foot, stack,sp);
		break;
	default:
		transformer_byworld_bycam_byglwnd(ent,foot, stack,sp);
		break;
	}
}
static int transformer_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* xxx = stack[sp-2].pchip;
	switch(xxx->hfmt){
	case _gl41list_:transformer_write_bywnd(ent,buf);break;
	}
	return 0;
}
static int transformer_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int transformer_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void transformer_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('t', 'r', 'a', 'n', 'f', 'o', 'r', 'm');

	p->oncreate = (void*)transformer_create;
	p->ondelete = (void*)transformer_delete;
	p->onreader = (void*)transformer_search;
	p->onwriter = (void*)transformer_modify;

	p->onattach = (void*)transformer_attach;
	p->ondetach = (void*)transformer_detach;
	p->ontaking = (void*)transformer_taking;
	p->ongiving = (void*)transformer_giving;
}
