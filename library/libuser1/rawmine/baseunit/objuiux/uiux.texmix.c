#include "libuser.h"
#define CTXBUF buf0
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);
void loadtexfromfile(struct glsrc* src, int idx, char* name);




char* texmix_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* texmix_glsl_f =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"void main(){\n"
	"mediump vec3 c0 = texture(tex0, uvw).bgr;\n"
	"mediump vec3 c1 = texture(tex1, uvw).bgr;\n"
	"FragColor = vec4((c0+c1)*0.5, 1.0);\n"
"}\n";
void texmix_ctxforwnd(struct glsrc* src)
{
	//property
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = texmix_glsl_v;
	src->fs = texmix_glsl_f;
	src->shader_enq = 42;

	//texture0
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(src, 0, "datafile/jpg/wall.jpg");
	src->tex[0].enq = 42;

	//texture1
	src->tex[1].fmt = hex32('r','g','b','a');
	src->tex[1].name = "tex1";
	src->tex[1].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(src, 1, "datafile/jpg/cartoon.jpg");
	src->tex[1].enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 42;
}
static void texmix_draw_gl41(
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

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 0.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 0.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 0.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 1.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
	gl41data_insert(ctx, 's', src, 1);
}




static int texmix_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static void texmix_modify(struct entity* act)
{
}
static void texmix_delete(struct entity* act)
{
	if(0 == act)return;
}
static void texmix_create(struct entity* act, void* str)
{
	if(0 == act)return;

	act->CTXBUF = memorycreate(0x200, 0);
	texmix_ctxforwnd(act->CTXBUF);
}





static void texmix_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texmix_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texmix_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texmix_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texmix_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("texmix(%x,%x,%x)\n",win,act,sty);
}




static void texmix_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> texball
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;
//say("@freecam_read_byeye:%c\n",len);

	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;slot = self->pfoot;
		if('v' == len)texmix_draw_gl41(act,slot, wrd,geom, wnd,area);
	}
//say("@freecam_read_byeye.end\n");
}
static void texmix_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	gl41data_before(wnd);
	texmix_draw_gl41(cam, 0, 0,(void*)&fs, wnd,area);
	gl41data_tmpcam(wnd);
	gl41data_after(wnd);
}




static int texmix_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent = peer->pchip;
	switch(ent->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != len)break;
		texmix_read_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	default:{
		texmix_read_bycam(self, peer, stack, rsp, buf, len);break;
	}
	}
	return 0;
}
static void texmix_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void texmix_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void texmix_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void texmix_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'x', 'm', 'i', 'x', 0, 0);

	p->oncreate = (void*)texmix_create;
	p->ondelete = (void*)texmix_delete;
	p->onsearch = (void*)texmix_search;
	p->onmodify = (void*)texmix_modify;

	p->onlinkup = (void*)texmix_linkup;
	p->ondiscon = (void*)texmix_discon;
	p->onread  = (void*)texmix_read;
	p->onwrite = (void*)texmix_write;
}
