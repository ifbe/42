#include "libuser.h"
#define OWNBUF buf0
int copypath(u8* path, u8* data);
void local2world(struct fstyle* src, struct fstyle* dst, mat4 mat);
void parsevertfromobj(struct glsrc* ctx, struct fstyle* sty, u8* buf, int len);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);


struct privdata{
	u8 vs[128];
	u8 fs[128];

	u8 albedo[128];
	u8 matter[128];

	int objlen;
	u8* objbuf;

	mat4 objmat;
	struct gl41data gl41;
};


static void obj3d_ctxforgl41(struct glsrc* src, char* albedo, char* matter, char* vs, char* fs)
{
	src->geometry = 3;
	src->method = 'v';
say("%s\n%s\n%s\n%s\n",albedo,matter,vs,fs);

	//shader
	src->vs = memorycreate(0x10000, 0);
	openreadclose(vs, 0, src->vs, 0x10000);
	src->fs = memorycreate(0x10000, 0);
	openreadclose(fs, 0, src->fs, 0x10000);
	src->shader_enq = 42;

	//argument
	src->arg[0].fmt = 'm';
	src->arg[0].name = "objmat";
/*
	src->arg[1].fmt = 'v';
	src->arg[1].name = "KA";
	tmp = src->arg[1].data = memorycreate(4*4, 0);
	tmp[0] = tmp[1] = tmp[2] = 0.231250;

	src->arg[2].fmt = 'v';
	src->arg[2].name = "KD";
	tmp = src->arg[2].data = memorycreate(4*4, 0);
	tmp[0] = tmp[1] = tmp[2] = 0.277500;

	src->arg[3].fmt = 'v';
	src->arg[3].name = "KS";
	tmp = src->arg[3].data = memorycreate(4*4, 0);
	tmp[0] = tmp[1] = tmp[2] = 0.773911;
*/
	//albedo
	src->tex[0].name = "albedomap";
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[0], albedo);
	src->tex[0].enq = 42;

	//matter
	src->tex[1].name = "mattermap";
	src->tex[1].fmt = hex32('r','g','b','a');
	src->tex[1].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[1], matter);
	src->tex[1].enq = 42;
}
static void obj3d_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* none)
{
	struct privdata* own = act->OWNBUF;
	if(0 == own)return;

	local2world(&part->fs, &geom->fs, own->objmat);

	struct glsrc* src = &own->gl41.src;
	gl41data_insert(ctx, 's', src, 1);
}



static void obj3d_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float* p;
	float f;
	float v[3][3];
	int j,ret;
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
/*
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	if(0 == (act->buf))return;
	if(0 == (act->len))return;

	if((act->width) > (act->height))f = 2.0*ww/(act->width);
	else f = 2.0*hh/(act->height);

	ret = *(u32*)((act->buf)+80);
	ret = ret % ((0x800000-0x84)/50);
	for(j=0;j<ret;j++)
	{
		p = (void*)(act->buf) + 84 + j*50;

		v[0][0] = cx + (p[ 3]-(act->cx))*f;
		v[0][1] = cy + (p[ 4]-(act->cy))*f;
		v[1][0] = cx + (p[ 6]-(act->cx))*f;
		v[1][1] = cy + (p[ 7]-(act->cy))*f;
		v[2][0] = cx + (p[ 9]-(act->cx))*f;
		v[2][1] = cy + (p[10]-(act->cy))*f;

		drawline(win, 0xffffff, v[0][0], v[0][1], v[1][0], v[1][1]);
		drawline(win, 0xffffff, v[0][0], v[0][1], v[2][0], v[2][1]);
		drawline(win, 0xffffff, v[1][0], v[1][1], v[2][0], v[2][1]);
	}
*/
}
static void obj3d_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void obj3d_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void obj3d_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void obj3d_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("obj3d(%x,%x,%x)\n",win,act,sty);
}




static void obj3d_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(0x2d70 == type)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
}




static void obj3d_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wrd;struct style* camg;
	struct entity* wnd;struct style* area;

	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wrd = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		obj3d_draw_gl41(ent,slot, scn,geom, wrd,camg, wnd,area);
	}
}
static void obj3d_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void obj3d_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void obj3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act = self->pchip;
	if(0 == act)return;
	struct privdata* own = act->buf0;
	if(0 == own)return;

	struct style* pin = self->pfoot;
	if(0 == pin)return;
	//struct style* sty = peer->pfoot;
	//if(0 == sty)return;

	//vertex
	struct glsrc* src = &own->gl41.src;
	src->vbuf_len = 0x100000;
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*9;
	parsevertfromobj(src, &pin->fs, own->objbuf, own->objlen);
	src->vbuf_enq = 42;
}




static void obj3d_search(struct entity* act)
{
}
static void obj3d_modify(struct entity* act)
{
}
static void obj3d_delete(struct entity* act)
{
	if(0 == act)return;
}
static void obj3d_create(struct entity* act, void* arg, int argc, u8** argv)
{/*
	int j;
	struct str* tmp;
	u8 vspath[128];
	u8 fspath[128];
	u8 texpath[128];
	char* vs = 0;
	char* fs = 0;
	char* tex = 0;
	if(0 == act)return;

	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "vs:", 3)){
			copypath(vspath, argv[j]+3);
			vs = (void*)vspath;
		}
		if(0 == ncmp(argv[j], "fs:", 3)){
			copypath(fspath, argv[j]+3);
			fs = (void*)fspath;
		}
		if(0 == ncmp(argv[j], "tex:", 4)){
			copypath(texpath, argv[j]+4);
			tex = (void*)texpath;
		}
	}
	if(0 == vs)vs = "datafile/shader/model/fv.glsl";
	if(0 == fs)fs = "datafile/shader/model/ff.glsl";
	if(0 == tex)tex = "datafile/jpg/cartoon.jpg";

	act->CTXBUF = memorycreate(0x200, 0);
	if(act->CTXBUF)obj3d_ctxforwnd(act->CTXBUF, tex, vs, fs);

	if(0 == arg)arg = "datafile/obj/cube.obj";
	tmp = act->OBJBUF = memorycreate(0x10000, 0);
	tmp->len = 0x1000;
	openreadclose(arg, 0, tmp->buf, tmp->len);*/

	int j;
	if(0 == act)return;

	struct privdata* own = act->OWNBUF = memorycreate(0x1000, 0);
	if(0 == own)return;

	//char* dxvs = 0;
	//char* dxfs = 0;
	char* glvs = 0;
	char* glfs = 0;
	char* albedo = 0;
	char* matter = 0;
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
		if(0 == ncmp(argv[j], "matter:", 7)){
			copypath(own->matter, argv[j]+7);
			matter = (void*)own->matter;
		}
	}
	//if(0 == dxvs)dxvs = "datafile/shader/obj/dxfv.glsl";
	//if(0 == dxfs)dxfs = "datafile/shader/obj/dxff.glsl";
	if(0 == glvs)glvs = "datafile/shader/obj/glfv.glsl";
	if(0 == glfs)glfs = "datafile/shader/obj/glff.glsl";
	if(0 == albedo)albedo = "datafile/jpg/wall.jpg";
	if(0 == matter)matter = "datafile/jpg/wallnormal.jpg";

	//obj3d_ctxforgl41(&own->gl41.src, albedo, normal, dxvs, dxfs);
	obj3d_ctxforgl41(&own->gl41.src, albedo, matter, glvs, glfs);
	own->gl41.src.arg[0].data = own->objmat;

	if(0 == arg)arg = "datafile/obj/cube.obj";
	own->objbuf = memorycreate(0x10000, 0);
	own->objlen = openreadclose(arg, 0, own->objbuf, 0x10000);
}




void obj3d_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 'b', 'j', '3', 'd', 0, 0, 0);

	p->oncreate = (void*)obj3d_create;
	p->ondelete = (void*)obj3d_delete;
	p->onsearch = (void*)obj3d_search;
	p->onmodify = (void*)obj3d_modify;

	p->onlinkup = (void*)obj3d_linkup;
	p->ondiscon = (void*)obj3d_discon;
	p->onread  = (void*)obj3d_read;
	p->onwrite = (void*)obj3d_write;
}
