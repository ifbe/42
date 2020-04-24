#include "libuser.h"
#define OWNBUF buf0
int copypath(u8* path, u8* data);
void loadtexfromfile(struct glsrc* src, int idx, char* name);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);


struct privdata{
	u8 vs[128];
	u8 fs[128];
	u8 albedo[128];
	u8 normal[128];

	struct gl41data gl41;
};


static void ground_ctxfordx11(struct glsrc* src, char* tex0, char* tex1, char* vs, char* fs)
{
}
static void ground_ctxforgl41(struct glsrc* src, char* tex0, char* tex1, char* vs, char* fs)
{
	//
	src->geometry = 3;
	src->method = 'v';
say("%s\n%s\n%s\n%s\n",tex0,tex1,vs,fs);
	//
	src->vs = memorycreate(0x10000, 0);
	openreadclose(vs, 0, src->vs, 0x10000);
	src->fs = memorycreate(0x10000, 0);
	openreadclose(fs, 0, src->fs, 0x10000);
	src->shader_enq = 42;

	//texture
	src->tex[0].name = "tex0";
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(src, 0, tex0);
	src->tex[0].enq = 42;

	//texture
	src->tex[1].name = "tex1";
	src->tex[1].fmt = hex32('r','g','b','a');
	src->tex[1].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(src, 1, tex1);
	src->tex[1].enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}
static void ground_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct privdata* own;
	struct glsrc* src;
	float (*vbuf)[9];
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	//gl41solid_rect(ctx, 0xffffff, vc, vr, vf);

	own = act->OWNBUF;
	if(0 == own)return;
	src = &own->gl41.src;
	if(0 == src)return;
	vbuf = (void*)(src->vbuf);
	if(0 == vbuf)return;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = vt[0];
	vbuf[0][3] = vt[1];
	vbuf[0][3] = vt[2];
	vbuf[0][6] = 0.0;
	vbuf[0][7] = 0.0;
	vbuf[0][8] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = vt[0];
	vbuf[1][3] = vt[1];
	vbuf[1][3] = vt[2];
	vbuf[1][6] = 1.0;
	vbuf[1][7] = 1.0;
	vbuf[1][8] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = vt[0];
	vbuf[2][3] = vt[1];
	vbuf[2][3] = vt[2];
	vbuf[2][6] = 0.0;
	vbuf[2][7] = 1.0;
	vbuf[2][8] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = vt[0];
	vbuf[3][3] = vt[1];
	vbuf[3][3] = vt[2];
	vbuf[3][6] = 1.0;
	vbuf[3][7] = 1.0;
	vbuf[3][8] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = vt[0];
	vbuf[4][3] = vt[1];
	vbuf[4][3] = vt[2];
	vbuf[4][6] = 0.0;
	vbuf[4][7] = 0.0;
	vbuf[4][8] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = vt[0];
	vbuf[5][3] = vt[1];
	vbuf[5][3] = vt[2];
	vbuf[5][6] = 1.0;
	vbuf[5][7] = 0.0;
	vbuf[5][8] = 0.0;

	src->vbuf_enq += 1;
	gl41data_insert(wnd, 's', src, 1);
}




static void ground_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void ground_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ground_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ground_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ground_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void ground_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		ground_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void ground_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void ground_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void ground_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void ground_search(struct entity* act)
{
}
static void ground_modify(struct entity* act)
{
}
static void ground_delete(struct entity* act)
{
}
static void ground_create(struct entity* act, void* str, int argc, u8** argv)
{
	int j;
	if(0 == act)return;

	struct privdata* own = act->OWNBUF = memorycreate(0x1000, 0);
	if(0 == own)return;

	//char* dxvs = 0;
	//char* dxfs = 0;
	char* glvs = 0;
	char* glfs = 0;
	char* albedo = 0;
	char* normal = 0;
	for(j=0;j<argc;j++){
		//say("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "glvs:", 5)){
			copypath(own->vs, argv[j]+5);
			glvs = (void*)own->vs;
		}
		if(0 == ncmp(argv[j], "glfs:", 5)){
			copypath(own->fs, argv[j]+5);
			glfs = (void*)own->fs;
		}
		if(0 == ncmp(argv[j], "albedo:", 7)){
			copypath(own->albedo, argv[j]+7);
			albedo = (void*)own->albedo;
		}
		if(0 == ncmp(argv[j], "normal:", 7)){
			copypath(own->normal, argv[j]+7);
			normal = (void*)own->normal;
		}
	}
	//if(0 == dxvs)dxvs = "datafile/shader/ground/fv.glsl";
	//if(0 == dxfs)dxfs = "datafile/shader/ground/ff.glsl";
	if(0 == glvs)glvs = "datafile/shader/ground/fv.glsl";
	if(0 == glfs)glfs = "datafile/shader/ground/ff.glsl";
	if(0 == albedo)albedo = "datafile/jpg/wall.jpg";
	if(0 == normal)normal = "datafile/jpg/wallnormal.jpg";

	//ground_ctxforgl41(&own->gl41.src, albedo, normal, dxvs, dxfs);
	ground_ctxforgl41(&own->gl41.src, albedo, normal, glvs, glfs);
}




void ground_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'r', 'o', 'u', 'n', 'd', 0, 0);

	p->oncreate = (void*)ground_create;
	p->ondelete = (void*)ground_delete;
	p->onsearch = (void*)ground_search;
	p->onmodify = (void*)ground_modify;

	p->onlinkup = (void*)ground_linkup;
	p->ondiscon = (void*)ground_discon;
	p->onread  = (void*)ground_read;
	p->onwrite = (void*)ground_write;
}
