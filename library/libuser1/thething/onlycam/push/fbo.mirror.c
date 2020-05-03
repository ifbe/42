#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
void matproj_transpose(void* m, struct fstyle* sty);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);


#define CAMBUF buf0
#define CTXBUF buf1
struct mirrbuf{
	mat4 mvp;
	u8 data[0];
};




char* mirror_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* mirror_glsl_f =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"mediump vec3 c = 0.8*texture(tex0, uvw).rgb + vec3(0.2, 0.2, 0.2);\n"
	"FragColor = vec4(c, 1.0);\n"
"}\n";




static void mirror_frustum(struct fstyle* frus, struct fstyle* obb, vec3 cam)
{
	float x,y,z,t;

//-------------p,q---------------
	//mirror.n
	x = obb->vt[0];
	y = obb->vt[1];
	z = obb->vt[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//op*cos(on,op): t = op * mirror.n
	t = (cam[0] - obb->vc[0])*x
	  + (cam[1] - obb->vc[1])*y
	  + (cam[2] - obb->vc[2])*z;

	//foot of a perpendicular: q = p - t*mirror.n
	frus->vq[0] = cam[0] - t*x;
	frus->vq[1] = cam[1] - t*y;
	frus->vq[2] = cam[2] - t*z;

	//reflected point: p' = p - 2*t*mirror.n
	frus->vc[0] = cam[0] - 2*t*x;
	frus->vc[1] = cam[1] - 2*t*y;
	frus->vc[2] = cam[2] - 2*t*z;


//----------------n,f----------------
	//vec(frus.n) = vec(mirr.n), len(frus.n) > len(cam to plane)
	frus->vn[0] = x;
	frus->vn[1] = y;
	frus->vn[2] = z;
	frus->vn[3] = t*1.001;

	frus->vf[0] = x;
	frus->vf[1] = y;
	frus->vf[2] = z;
	frus->vf[3] = 1e20;


//--------------l,r--------------------
	//nr = -norm(mirror.r)
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
	//nt = norm(mirror.t)
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
static void mirror_forfbo_update(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wrl, struct style* camg,
	struct supply* fbo, struct style* area)
{
	//frus from shape and eye
	struct fstyle* shap = &geom->fshape;
	struct fstyle* frus = &geom->frustum;
	mirror_frustum(frus, shap, camg->frus.vc);

	//mvp from frus
	struct mirrbuf* mirr = act->CAMBUF;
	if(0 == mirr)return;
	matproj_transpose(mirr->mvp, frus);
//printstyle(shap);
//printstyle(frus);
	//
	struct glsrc* src = (void*)(mirr->data);
	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = mirr->mvp;
	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = frus->vc;
	fbo->gl_camera[0] = (void*)(mirr->data);
}
void mirror_forfbo_prepare(struct glsrc* src)
{
}




static void mirror_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct mirrbuf* mirr;
	struct glsrc* src;
	float (*vbuf)[6];
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	//gl41solid_rect(win, 0x404040, vc, vr, vf);

	mirr = act->CTXBUF;
	if(0 == mirr)return;
	src = (void*)(mirr->data);
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
	gl41data_insert(wnd, 's', src, 1);
}
void mirror_forwnd_update(struct entity* act, struct style* slot, struct supply* fbo, struct style* area)
{
	struct mirrbuf* mirr = act->CTXBUF;
	if(0 == mirr)return;

	struct glsrc* own = (void*)(mirr->data);
	if(0 == own)return;

	own->tex[0].glfd = fbo->tex0;
	own->tex[0].name = "tex0";
	own->tex[0].fmt = '!';
	own->tex[0].enq += 1;
}
void mirror_forwnd_prepare(struct glsrc* src)
{
	//shader
	src->vs = mirror_glsl_v;
	src->fs = mirror_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->geometry = 3;
	src->opaque = 0;

	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 0;
}




//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam,gl41 -> wor,camg		//the camera taking photo
//[-2,-1]: wor,geom -> ent,gl41		//the entity being taken
static void mirror_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* dup;struct style* camg;
	struct entity* wnd;struct style* area;
	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	if('v' == key){
		mirror_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	if('?' == key){
		//search for myown fbo
		int ret;
		struct halfrel* rel[2];
		ret = relationsearch(ent, _fbo_, &rel[0], &rel[1]);
		if(ret <= 0)return;

		//update matrix for fbo
		struct supply* fbo = rel[1]->pchip;
		struct style* rect = rel[1]->pfoot;
		mirror_forfbo_update(ent,slot, wor,geom, dup,camg, fbo,rect);

		//wnd.data -> fbo.texture
		relationwrite(ent,_fbo_, stack,sp, 0,0, 0,0);

		//fbo.texture -> my.data -> wnd.data
		mirror_forwnd_update(ent,slot, fbo,rect);
	}
}
static void mirror_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mirror_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mirror_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mirror_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mirror_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void mirror_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	mirror_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
}
static void mirror_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mirror_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mirror_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mirror_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
}
static void mirror_modify(struct entity* act)
{
}
static void mirror_delete(struct entity* act)
{
}
static void mirror_create(struct entity* act, void* str)
{
	struct mirrbuf* mirr;
	struct glsrc* src;
	if(0 == act)return;

	mirr = act->CTXBUF = memorycreate(0x1000, 0);
	if(0 == mirr)return;
	src = (void*)(mirr->data);
	mirror_forwnd_prepare(src);

	mirr = act->CAMBUF = memorycreate(0x1000, 0);
	if(0 == mirr)return;
	src = (void*)(mirr->data);
	mirror_forfbo_prepare(src);
}




void mirror_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'i', 'r', 'r', 'o', 'r', 0, 0);

	p->oncreate = (void*)mirror_create;
	p->ondelete = (void*)mirror_delete;
	p->onsearch = (void*)mirror_search;
	p->onmodify = (void*)mirror_modify;

	p->onlinkup = (void*)mirror_linkup;
	p->ondiscon = (void*)mirror_discon;
	p->onread  = (void*)mirror_read;
	p->onwrite = (void*)mirror_write;
}
