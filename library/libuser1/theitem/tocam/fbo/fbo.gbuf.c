#include "libuser.h"
#define _fbog_ hex32('f','b','o','g')
#define CTXBUF buf0
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);
//
int copypath(u8* path, u8* data);
int loadtexfromfile(struct glsrc* src, int idx, char* name);




void gbuffer_ctxforwnd(struct glsrc* src, char* vs, char* fs)
{
	//property
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = memorycreate(0x1000, 0);
	openreadclose(vs, 0, src->vs, 0x1000);
	src->fs = memorycreate(0x1000, 0);
	openreadclose(fs, 0, src->fs, 0x1000);
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 42;
}
static void gbuffer_readfrom_gbuffer(struct entity* ent, struct glsrc* src)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)return;
		if(_fbog_ == rel->srcflag)break;
		rel = samesrcnextdst(rel);
	}

	struct supply* sup = rel->pdstchip;

	src->tex[0].glfd = sup->tex[0];
	src->tex[0].name = "tex0";
	src->tex[0].fmt = '!';
	src->tex[0].enq += 1;

	src->tex[1].glfd = sup->tex[1];
	src->tex[1].name = "tex1";
	src->tex[1].fmt = '!';
	src->tex[1].enq += 1;

	src->tex[2].glfd = sup->tex[2];
	src->tex[2].name = "tex2";
	src->tex[2].fmt = '!';
	src->tex[2].enq += 1;

	src->tex[3].glfd = sup->tex[3];
	src->tex[3].name = "tex3";
	src->tex[3].fmt = '!';
	src->tex[3].enq += 1;
	//say("%d,%d,%d,%d\n", src->tex[0].glfd, src->tex[1].glfd, src->tex[2].glfd, src->tex[3].glfd);
}
static void gbuffer_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	if(0 == act->CTXBUF)return;

	struct glsrc* src = act->CTXBUF;
	float (*vbuf)[6] = (void*)(src->vbuf);

	gbuffer_readfrom_gbuffer(act, src);

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
	gl41data_insert(ctx, 's', src, 1);
}




static int gbuffer_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static void gbuffer_modify(struct entity* act)
{
}
static void gbuffer_delete(struct entity* act)
{
	if(0 == act)return;
}
static void gbuffer_create(struct entity* act, void* arg, int argc, u8** argv)
{
	int j;
	u8 vspath[128];
	u8 fspath[128];
	char* vs = 0;
	char* fs = 0;
	if(0 == act)return;

	for(j=0;j<argc;j++){
		//say("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "vs:", 3)){
			copypath(vspath, argv[j]+3);
			vs = (void*)vspath;
		}
		if(0 == ncmp(argv[j], "fs:", 3)){
			copypath(fspath, argv[j]+3);
			fs = (void*)fspath;
		}
	}
	if(0 == vs)vs = "datafile/shader/deferred/vert.glsl";
	if(0 == fs)fs = "datafile/shader/deferred/frag.glsl";

	act->CTXBUF = memorycreate(0x200, 0);
	gbuffer_ctxforwnd(act->CTXBUF, vs, fs);
}





static void gbuffer_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gbuffer_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gbuffer_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gbuffer_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gbuffer_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("gbuffer(%x,%x,%x)\n",win,act,sty);
}




static void gbuffer_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		relationread(ent,_fbog_, stack,sp, 0,0, 0,0);
		gbuffer_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
//say("@freecam_read_byeye.end\n");
}
static void gbuffer_read_bywnd(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	struct entity* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	gl41data_before(wnd);
	relationread(ent,_fbog_, stack,sp, 0,0, 0,0);
	gbuffer_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_tmpcam(wnd);
	gl41data_after(wnd);
}




static int gbuffer_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct supply* sup = stack[sp-2].pchip;
	switch(sup->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != key)break;
		gbuffer_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	default:{
		gbuffer_read_bycam(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	}
	return 0;
}
static void gbuffer_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@gbuffer_write\n");
	if(_wnd_ == foot){
		relationwrite(ent,_fbog_, stack,sp, arg,key, buf,len);
	}
}
static void gbuffer_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void gbuffer_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void gbuffer_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g','b','u','f','f','e','r', 0);

	p->oncreate = (void*)gbuffer_create;
	p->ondelete = (void*)gbuffer_delete;
	p->onsearch = (void*)gbuffer_search;
	p->onmodify = (void*)gbuffer_modify;

	p->onlinkup = (void*)gbuffer_linkup;
	p->ondiscon = (void*)gbuffer_discon;
	p->onread  = (void*)gbuffer_read;
	p->onwrite = (void*)gbuffer_write;
}
