#include "libuser.h"
#define OWNBUF buf0
int copypath(u8* path, u8* data);
void loadtexfromfile(struct glsrc* src, int idx, char* name);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




static void ground_ctxforwnd(struct glsrc* src, char* str, char* vs, char* fs)
{
	//
	src->geometry = 3;
	src->method = 'v';
//say("%s\n%s\n%s\n",str,vs,fs);
	//
	src->vs = memorycreate(0x1000, 0);
	openreadclose(vs, 0, src->vs, 0x1000);
	src->fs = memorycreate(0x1000, 0);
	openreadclose(fs, 0, src->fs, 0x1000);
	src->shader_enq = 42;

	//texture
	src->tex[0].name = "tex0";
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(src, 0, str);
	src->tex[0].enq = 42;

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
	struct glsrc* src;
	float (*vbuf)[9];
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	//gl41solid_rect(ctx, 0xffffff, vc, vr, vf);

	src = act->OWNBUF;
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
	gl41data_insert(wnd, 's', act->OWNBUF, 1);
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




static void ground_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//world -> texball
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;part = self->pfoot;
		if('v' == len)ground_draw_gl41(act,part, win,geom, wnd,area);
	}
}
static void ground_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
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
	if(0 == vs)vs = "datafile/shader/ground/vert.glsl";
	if(0 == fs)fs = "datafile/shader/ground/frag.glsl";
	if(0 == str)str = "datafile/jpg/wall.jpg";

	act->OWNBUF = memorycreate(0x200, 0);
	if(act->OWNBUF)ground_ctxforwnd(act->OWNBUF, str, vs, fs);
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
