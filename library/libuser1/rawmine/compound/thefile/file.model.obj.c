#include "libuser.h"
#define OBJBUF buf0
#define CTXBUF buf1
int copypath(u8* path, u8* data);
void local2world(struct fstyle* src, struct fstyle* dst, mat4 mat);
void loadtexfromfile(struct glsrc* src, int idx, char* name);
void parsevertfromobj(struct glsrc* ctx, struct fstyle* sty, u8* buf, int len);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




static void obj3d_ctxforwnd(struct glsrc* src, char* tex, char* vs, char* fs)
{
	float* tmp;
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = memorycreate(0x1000, 0);
	openreadclose(vs, 0, src->vs, 0x1000);
	src->fs = memorycreate(0x1000, 0);
	openreadclose(fs, 0, src->fs, 0x1000);
	src->shader_enq = 42;

	//texture
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(src, 0, tex);
	src->tex[0].enq = 42;
	//say("w=%d,h=%d\n",src->tex[0].w, src->tex[0].h);

	//argument
	src->arg[0].fmt = 'm';
	src->arg[0].name = "objmat";
	src->arg[0].data = memorycreate(4*4*4, 0);

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

	//vertex
	src->vbuf_len = 0x1000000;
	src->vbuf = memorycreate(src->vbuf_len, 0);
}
static void obj3d_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* none)
{
	if(0 == act)return;
	if(act->CTXBUF == 0)return;

	struct glsrc* src = act->CTXBUF;
	local2world(&part->fs, &geom->fs, (void*)src->arg[0].data);

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




static void obj3d_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//world -> obj3d
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack && ('v' == len)){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		scn = peer->pchip;geom = peer->pfoot;
		act = self->pchip;slot = self->pfoot;
		obj3d_draw_gl41(act,slot, scn,geom, wrd,camg, wnd,area);
	}
}
static void obj3d_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void obj3d_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void obj3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct str* tmp;
	struct glsrc* src;
	struct style* pin = (void*)(self->foot);
	struct entity* act = (void*)(self->chip);
	if(0 == act)return;
	if(0 == pin)return;

	tmp = act->OBJBUF;

	//vertex
	src = act->CTXBUF;
	parsevertfromobj(src, &pin->fs, tmp->buf, tmp->len);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*9;
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
{
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
	if(0 == vs)vs = "datafile/shader/model/vert.glsl";
	if(0 == fs)fs = "datafile/shader/model/frag.glsl";
	if(0 == tex)tex = "datafile/jpg/cartoon.jpg";

	act->CTXBUF = memorycreate(0x200, 0);
	if(act->CTXBUF)obj3d_ctxforwnd(act->CTXBUF, tex, vs, fs);

	if(0 == arg)arg = "datafile/stl/bunny-lowpoly.stl";
	tmp = act->OBJBUF = memorycreate(0x10000, 0);
	tmp->len = 0x1000;
	openreadclose(arg, 0, tmp->buf, tmp->len);
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
