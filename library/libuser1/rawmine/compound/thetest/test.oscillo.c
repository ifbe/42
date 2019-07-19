#include "libuser.h"
#define _data_ hex32('d','a','t','a')




char* oscillo_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char* oscillo_glsl_f =
GLSL_VERSION
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
"}\n";




static void oscillo_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
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
static void oscillo_draw_vbo(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	struct glsrc* src = (void*)(pin->foot[0]);
	src->vbuf_enq += 1;
	//say("@oscillo_draw_vbo\n");
}
static void oscillo_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void oscillo_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void oscillo_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void oscillo_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void oscillo_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)oscillo_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)oscillo_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)oscillo_draw_html(act, pin, win, sty);
	else if(fmt == _json_)oscillo_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)oscillo_draw_vbo(act, pin, win, sty);
	else oscillo_draw_pixel(act, pin, win, sty);
}




static void oscillo_data(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	float* buf, int len)
{
	int t = act->vlen;
	float* f = act->vbuf;
	//say("@oscillo_data:%d\n", len);

	f[3*t + 2] = buf[0];
	act->vlen = (act->vlen + len/3) % 0x10000;

	//say("%d: %f,%f,%f\n", j, ff[3*j], ff[3*j+1], ff[3*j+2]);
}




static void oscillo_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	oscillo_draw(act, pin, win, sty);
}
static void oscillo_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	switch(self->flag){
		case _data_:oscillo_data(act, pin, win, sty, buf, len);break;
	}
}
static void oscillo_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void oscillo_start(struct halfrel* self, struct halfrel* peer)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	if(_data_ == self->flag)return;

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
	src->vs = oscillo_glsl_v;
	src->fs = oscillo_glsl_f;
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
}




static void oscillo_search(struct actor* act)
{
}
static void oscillo_modify(struct actor* act)
{
}
static void oscillo_delete(struct actor* act)
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
static void oscillo_create(struct actor* act, void* str)
{
	int j,k;
	u16* uu;
	float* ff;
	if(0 == act)return;
	if(act->vbuf)return;
	if(act->ibuf)return;

	act->vlen = 0;

	ff = act->vbuf = memorycreate(4*3*0x10000);
	for(j=0;j<0x10000;j++){
		ff[j*3 + 0] = j;
		ff[j*3 + 1] = 0.0;

		k = j%16;
		if(0 != k)k = 16;
		ff[j*3 + 2] = k;
	}

	uu = act->ibuf = memorycreate(2*2*0x10000);
	for(j=0;j<0xffff;j++){
		uu[j*2 + 0] = j;
		uu[j*2 + 1] = j+1;
	}
}




void oscillo_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 's', 'c', 'i', 'l', 'l', 'o', 0);

	p->oncreate = (void*)oscillo_create;
	p->ondelete = (void*)oscillo_delete;
	p->onsearch = (void*)oscillo_search;
	p->onmodify = (void*)oscillo_modify;

	p->onstart = (void*)oscillo_start;
	p->onstop  = (void*)oscillo_stop;
	p->onread  = (void*)oscillo_read;
	p->onwrite = (void*)oscillo_write;
}
