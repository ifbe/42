#include "libuser.h"
#define CTXBUF buf0
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




char* picture_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* picture_glsl_f =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"uniform mediump float angle;\n"
"void main(){\n"
	"mediump vec4 bgra = texture(tex0, uvw).bgra;\n"
	"FragColor = bgra;\n"
"}\n";


static void picture_ctxforwnd(struct glsrc* src, char* str, float* angle)
{
	//shader
	src->vs = picture_glsl_v;
	src->fs = picture_glsl_f;
	src->shader_enq = 42;
/*
	//arg
	src->arg[0].fmt = 'f';
	src->arg[0].name = "angle";
	src->arg[0].data = angle;
*/
	//texture0
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[0], str);
	src->tex[0].enq = 42;

	//vertex
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 42;
}
static void picture_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	struct glsrc* src;
	float (*vbuf)[6];
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	if(0 == act->CTXBUF)return;
/*
	act->fx0 = ((timeread()%5000000)/5000000.0)*tau;
	//say("%f\n",act->fx0);
*/
	src = act->CTXBUF;
	vbuf = (void*)(src->vbuf);

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
	gl41data_insert(ctx, 'o', act->CTXBUF, 1);
}




static void picture_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void picture_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void picture_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void picture_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void picture_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("picture(%x,%x,%x)\n",win,act,sty);
}




static void picture_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		picture_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void picture_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@picture_write\n");
	relationwrite(ent,_evto_, stack,sp, 0,0, "calibrate\n", 10);
}
static void picture_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void picture_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void picture_search(struct entity* act)
{
}
static void picture_modify(struct entity* act)
{
}
static void picture_delete(struct entity* act)
{
	if(0 == act)return;
	memorydelete(act->CTXBUF);
	act->CTXBUF = 0;
}
static void picture_create(struct entity* act, void* str)
{
	if(0 == act)return;

	act->CTXBUF = memorycreate(0x200, 0);
	if(0 == act->CTXBUF)return;

	if(0 == str)str = "datafile/jpg/cartoon.jpg";
	picture_ctxforwnd(act->CTXBUF, str, &act->fx0);
}




void picture_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'i', 'c', 't', 'u', 'r', 'e', 0);

	p->oncreate = (void*)picture_create;
	p->ondelete = (void*)picture_delete;
	p->onsearch = (void*)picture_search;
	p->onmodify = (void*)picture_modify;

	p->onlinkup = (void*)picture_linkup;
	p->ondiscon = (void*)picture_discon;
	p->onread  = (void*)picture_read;
	p->onwrite = (void*)picture_write;
}
