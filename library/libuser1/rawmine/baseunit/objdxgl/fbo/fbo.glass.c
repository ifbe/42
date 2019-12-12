#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
void fixmatrix(void* m, struct fstyle* sty);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);


#define CAMBUF buf0
#define CTXBUF buf1
struct glassbuf{
	mat4 mvp;
	u8 data[0];
};




char* glass_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main()\n"
"{\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* glass_glsl_f =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"void main()\n"
"{\n"
	"mediump vec3 c = texture(tex0, uvw).rgb * vec3(1.0, 0.8, 0.6);\n"
	"FragColor = vec4(c, 0.5);\n"
"}\n";

void glass_forfbo(struct glsrc* src)
{
}
void glass_forwnd(struct glsrc* src)
{
	src->geometry = 3;
	src->method = 'v';
	src->opaque = 1;

	//
	src->vs = glass_glsl_v;
	src->fs = glass_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 0;

	//texture
	src->tex[0].name = "tex0";
}




static void glass_search(struct entity* act)
{
}
static void glass_modify(struct entity* act)
{
}
static void glass_delete(struct entity* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void glass_create(struct entity* act, void* str)
{
	struct glassbuf* glass;
	struct glsrc* src;
	if(0 == act)return;

	glass = act->CTXBUF = memorycreate(0x1000, 0);
	if(0 == glass)return;
	src = (void*)(glass->data);
	glass_forwnd(src);

	glass = act->CAMBUF = memorycreate(0x1000, 0);
	if(0 == glass)return;
	src = (void*)(glass->data);
	glass_forfbo(src);
}




static void glass_draw_pixel(
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
static void glass_draw_vbo(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	struct glassbuf* glass;
	struct glsrc* src;
	float (*vbuf)[6];
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

	glass = act->CTXBUF;
	if(0 == glass)return;
	src = (void*)(glass->data);
	if(0 == src)return;
	vbuf = (void*)(src->vbuf);
	if(0 == vbuf)return;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 1.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 0.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 1.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 0.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 1.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 0.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
	gl41data_insert(ctx, 'o', src, 1);
}
static void glass_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void glass_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void glass_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void glass_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void glass_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)glass_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)glass_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)glass_draw_html(act, pin, win, sty);
	else if(fmt == _json_)glass_draw_json(act, pin, win, sty);
	else glass_draw_pixel(act, pin, win, sty);
}




void glass_frustum(struct fstyle* frus, struct fstyle* obb, vec3 cam)
{
	float x,y,z,t;


//-------------p,q---------------
	//glass.n
	x = obb->vt[0];
	y = obb->vt[1];
	z = obb->vt[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//op*cos(on,op): t = op * glass.n
	t = (cam[0] - obb->vc[0])*x
	  + (cam[1] - obb->vc[1])*y
	  + (cam[2] - obb->vc[2])*z;

	//foot of a perpendicular: q = p - t*glass.n
	frus->vq[0] = cam[0] - t*x;
	frus->vq[1] = cam[1] - t*y;
	frus->vq[2] = cam[2] - t*z;

	//reflected point: p' = p - 2*t*glass.n
	frus->vc[0] = cam[0] - 2*t*x;
	frus->vc[1] = cam[1] - 2*t*y;
	frus->vc[2] = cam[2] - 2*t*z;


//----------------n,f----------------
	//vec(frus.n) = vec(glass.n), len(frus.n) > len(cam to plane)
	frus->vn[0] = x;
	frus->vn[1] = y;
	frus->vn[2] = z;
	frus->vn[3] = t*1.001;

	frus->vf[0] = x;
	frus->vf[1] = y;
	frus->vf[2] = z;
	frus->vf[3] = 1e20;


//--------------l,r--------------------
	//nr = -norm(glass.r)
	x = -obb->vr[0];
	y = -obb->vr[1];
	z = -obb->vr[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//l.len = (l-q) * nr
	t = (obb->vc[0] + obb->vr[0] - frus->vq[0]) * x
	  + (obb->vc[1] + obb->vr[1] - frus->vq[1]) * y
	  + (obb->vc[2] + obb->vr[2] - frus->vq[2]) * z;
	frus->vl[0] = -x;
	frus->vl[1] = -y;
	frus->vl[2] = -z;
	frus->vl[3] = t;

	//r.len = (r-q) * nr
	t = (obb->vc[0] - obb->vr[0] - frus->vq[0]) * x
	  + (obb->vc[1] - obb->vr[1] - frus->vq[1]) * y
	  + (obb->vc[2] - obb->vr[2] - frus->vq[2]) * z;
	frus->vr[0] = x;
	frus->vr[1] = y;
	frus->vr[2] = z;
	frus->vr[3] = t;


//----------------b,t-----------------
	//nt = norm(glass.t)
	x = obb->vf[0];
	y = obb->vf[1];
	z = obb->vf[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//b.len =  = (b-q) * nt
	t = (obb->vc[0] - obb->vf[0] - frus->vq[0]) * x
	  + (obb->vc[1] - obb->vf[1] - frus->vq[1]) * y
	  + (obb->vc[2] - obb->vf[2] - frus->vq[2]) * z;
	frus->vb[0] = x;
	frus->vb[1] = y;
	frus->vb[2] = z;
	frus->vb[3] = t;

	//t.len = (u-q) * nt
	t = (obb->vc[0] + obb->vf[0] - frus->vq[0]) * x
	  + (obb->vc[1] + obb->vf[1] - frus->vq[1]) * y
	  + (obb->vc[2] + obb->vf[2] - frus->vq[2]) * z;
	frus->vt[0] = x;
	frus->vt[1] = y;
	frus->vt[2] = z;
	frus->vt[3] = t;
}
static void glass_matrix(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wrl, struct style* camg,
	struct supply* fbo, struct style* area)
{
	//frus from shape and eye
	struct fstyle* shap = &geom->fshape;
	struct fstyle* frus = &geom->frustum;
	glass_frustum(frus, shap, camg->frus.vc);

	//mvp from frus
	struct glassbuf* glass = act->CAMBUF;
	if(0 == glass)return;
	fixmatrix(glass->mvp, frus);
	mat4_transpose(glass->mvp);

	//give arg(matrix and position) to fbo
	struct glsrc* src = (void*)(glass->data);
	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = glass->mvp;
	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = frus->vc;
	fbo->gl_camera[0] = (void*)(glass->data);
}
void glass_findfbo(struct entity* act, struct style* slot, struct supply** fbo, struct style** rect)
{
	struct relation* rel = act->orel0;
	if(0 == rel)return;

	*fbo = rel->pdstchip;
	*rect = rel->pdstfoot;
}
void glass_update(struct entity* act, struct style* slot, struct supply* fbo, struct style* rect)
{
	struct glassbuf* glass = act->CTXBUF;
	if(0 == glass)return;

	struct glsrc* own = (void*)(glass->data);
	if(0 == own)return;

	own->tex[0].glfd = fbo->tex0;
	own->tex[0].name = "tex0";
	own->tex[0].fmt = '!';
	own->tex[0].enq += 1;
}




static void glass_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> glass
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;
//fbo,rect
	struct supply* fbo;struct style* rect;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len){
			glass_draw_vbo(act,slot, win,geom, wnd,area);
		}
		if('?' == len){
			fbo = 0;rect = 0;
			glass_findfbo(act,slot, &fbo,&rect);
			if((0 == fbo)|(0 == rect))return;

			glass_matrix(act,slot, win,geom, wrd,camg, fbo,rect);
			relationread(act,_fbo_, stack,rsp, buf,len);

			glass_update(act,slot, fbo,rect);
		}
	}
}
static void glass_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void glass_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void glass_start(struct halfrel* self, struct halfrel* peer)
{
	say("glass_start: %.4s\n",self->flag);
}




void glass_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'l', 'a', 's', 's', 0, 0, 0);

	p->oncreate = (void*)glass_create;
	p->ondelete = (void*)glass_delete;
	p->onsearch = (void*)glass_search;
	p->onmodify = (void*)glass_modify;

	p->onstart = (void*)glass_start;
	p->onstop  = (void*)glass_stop;
	p->onread  = (void*)glass_read;
	p->onwrite = (void*)glass_write;
}
