#include "libuser.h"
#define _data_ hex32('d','a','t','a')




char* spectrum_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char* spectrum_glsl_f =
GLSL_VERSION
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
"}\n";




static void spectrum_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}
}
static void spectrum_draw_gl41(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	struct mysrc* src = (void*)(pin->data[0]);
	src->vbuf_enq += 1;
	//say("@spectrum_draw_gl41\n");
}
static void spectrum_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spectrum_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spectrum_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spectrum_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void spectrum_data(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	float* buf, int len)
{
	int t = act->vlen;
	float* f = act->vbuf;
	//say("@spectrum_data:%d\n", len);

	f[3*t + 2] = buf[0];
	act->vlen = (act->vlen + len/3) % 0x10000;

	//say("%d: %f,%f,%f\n", j, ff[3*j], ff[3*j+1], ff[3*j+2]);
}




static void spectrum_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void spectrum_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void spectrum_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void spectrum_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct datapair* pair;
	struct mysrc* src;
	struct gldst* dst;
	_obj* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	_obj* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	if(_data_ == self->flag)return;
/*
	//
	pair = alloc_winobj(win, 's');
	src = &pair->src;
	dst = &pair->dst;
	pin->foot[0] = (u64)src;
	sty->foot[0] = (u64)dst;

	//
	src->geometry = 2;
	src->method = 'i';

	//
	src->vs = spectrum_glsl_v;
	src->fs = spectrum_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf = act->vbuf;
	src->vbuf_fmt = vbuffmt_3;
	src->vbuf_len = 4*3*0x10000;
	src->vbuf_w = 4*3;
	src->vbuf_h = 0x10000;
	src->vbuf_enq = 1;

	//index
	src->ibuf = act->ibuf;
	src->ibuf_fmt = 0x22;
	src->ibuf_len = 2*2*0x10000;
	src->ibuf_w = 2*2;
	src->ibuf_h = 0xffff;
	src->ibuf_enq = 1;
*/
}




static void spectrum_search(_obj* act)
{
}
static void spectrum_modify(_obj* act)
{
}
static void spectrum_delete(_obj* act)
{
	if(0 == act)return;
	if(act->ibuf){
		memorydelete(act->ibuf);
		act->ibuf = 0;
	}
	if(act->vbuf){
		memorydelete(act->vbuf);
		act->vbuf = 0;
	}
}
static void spectrum_create(_obj* act, void* str)
{
	int j,k;
	u16* uu;
	float* ff;
	if(0 == act)return;
	if(act->vbuf)return;
	if(act->ibuf)return;

	act->vlen = 0;

	ff = act->vbuf = memorycreate(4*3*0x10000, 0);
	for(j=0;j<0x10000;j++){
		ff[j*3 + 0] = j;
		ff[j*3 + 1] = 0.0;

		k = j%16;
		if(0 != k)k = 16;
		ff[j*3 + 2] = k;
	}

	uu = act->ibuf = memorycreate(2*2*0x10000, 0);
	for(j=0;j<0xffff;j++){
		uu[j*2 + 0] = j;
		uu[j*2 + 1] = j+1;
	}
}




void spectrum_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s', 'p', 'e', 'c', 't', 'r', 'u', 'm');

	p->oncreate = (void*)spectrum_create;
	p->ondelete = (void*)spectrum_delete;
	p->onsearch = (void*)spectrum_search;
	p->onmodify = (void*)spectrum_modify;

	p->onlinkup = (void*)spectrum_linkup;
	p->ondiscon = (void*)spectrum_discon;
	p->ontaking = (void*)spectrum_taking;
	p->ongiving = (void*)spectrum_giving;
}
