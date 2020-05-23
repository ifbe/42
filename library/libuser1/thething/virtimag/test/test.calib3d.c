#include "libuser.h"
#define nbuf buf0
#define wbuf buf1
#define vbuf buf2
#define ibuf buf3
#define _data_ hex32('d','a','t','a')
struct mag_minmax{
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	float zmin;
	float zmax;
};




char* calib3d_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char* calib3d_glsl_f =
GLSL_VERSION
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
"}\n";




static void calib3d_draw_pixel(
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
static void calib3d_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	//struct glsrc* src = (void*)(pin->data[0]);
	//src->vbuf_enq += 1;
	//say("@calib3d_draw_gl41\n");
}
static void calib3d_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void calib3d_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void calib3d_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void calib3d_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void calib3d_data(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	float* buf, int len)
{
	int j;
	int t = act->vlen;
	float* f = act->vbuf;
	struct mag_minmax* priv = act->buf0;
	//say("@calib3d_data:%d\n", len);

	for(j=0;j<len;j+=3){
		f[3*t + j+0] = buf[j+0];
		f[3*t + j+1] = buf[j+1];
		f[3*t + j+2] = buf[j+2];
	}
	act->vlen = (act->vlen + len/3) % 0x10000;

	if(buf[0] < priv->xmin)priv->xmin = buf[0];
	if(buf[0] > priv->xmax)priv->xmax = buf[0];
	if(buf[1] < priv->ymin)priv->ymin = buf[1];
	if(buf[1] > priv->ymax)priv->ymax = buf[1];
	if(buf[2] < priv->zmin)priv->zmin = buf[2];
	if(buf[2] > priv->zmax)priv->zmax = buf[2];
	say("%f,%f,%f,%f,%f,%f\n",priv->xmin,priv->xmax,priv->ymin,priv->ymax,priv->zmin,priv->zmax);
}




static void calib3d_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void calib3d_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void calib3d_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void calib3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
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
	src->vs = calib3d_glsl_v;
	src->fs = calib3d_glsl_f;
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




static void calib3d_search(struct entity* act)
{
}
static void calib3d_modify(struct entity* act)
{
}
static void calib3d_delete(struct entity* act)
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
static void calib3d_create(struct entity* act, void* str)
{
	int j;
	u16* uu;
	float* ff;
	struct mag_minmax* priv;
	if(0 == act)return;
	if(act->vbuf)return;
	if(act->ibuf)return;

	act->vlen = 0;

	ff = act->vbuf = memorycreate(4*3*0x10000, 0);
	for(j=0;j<0x10000;j++){
		ff[j*3 + 0] = 0.0;
		ff[j*3 + 1] = 0.0;
		ff[j*3 + 2] = j;
	}

	uu = act->ibuf = memorycreate(2*2*0x10000, 0);
	for(j=0;j<0xffff;j++){
		uu[j*2 + 0] = j;
		uu[j*2 + 1] = j+1;
	}

	priv = act->buf0 = memorycreate(sizeof(struct mag_minmax), 0);
	priv->xmin = 99999.0;
	priv->xmax = -99999.0;
	priv->ymin = 99999.0;
	priv->ymax = -99999.0;
	priv->zmin = 99999.0;
	priv->zmax = -99999.0;
}




void calib3d_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'a', 'l', 'i', 'b', '3', 'd', 0);

	p->oncreate = (void*)calib3d_create;
	p->ondelete = (void*)calib3d_delete;
	p->onsearch = (void*)calib3d_search;
	p->onmodify = (void*)calib3d_modify;

	p->onlinkup = (void*)calib3d_linkup;
	p->ondiscon = (void*)calib3d_discon;
	p->ontaking = (void*)calib3d_taking;
	p->ongiving = (void*)calib3d_giving;
}
