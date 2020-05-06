#include "libuser.h"
#define _int_ hex32('i','n','t', 0)
#define CTXBUF buf0
int copypath(u8* path, u8* data);
int windowread(int type, void* buf);
int windowwrite(int type, void* buf);
void parsevertfromstl(struct glsrc* ctx, struct fstyle* sty, u8* buf, int len);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);
void gl41solid_spheretest(struct entity* win, u32 rgb, vec3 vc);
//
void world2local(mat4 mat, struct fstyle* src, struct fstyle* dst);
void local2world(mat4 mat, struct fstyle* src, struct fstyle* dst);
void mat4_transposefrom(mat4, mat4);
void vec3_mat4vec3(vec3 o, mat4 m, vec3 v);
//
int ray_trigon(vec3 out, vec3 ro, vec3 rd, vec3 t0, vec3 t1, vec3 t2);


struct privdata{
	u8 vs[128];
	u8 fs[128];
	struct fstyle* localgeom;
	struct fstyle* worldgeom;
	vec4 matter;
	mat4 objmat;
	struct gl41data gl41;
};


static void stl3d_ctxforwnd(struct privdata* own, char* str, char* vs, char* fs)
{
	float* tmp;
	struct glsrc* src = &own->gl41.src;

	//shader
	src->vs = memorycreate(0x10000, 0);
	openreadclose(vs, 0, src->vs, 0x10000);
	src->fs = memorycreate(0x10000, 0);
	openreadclose(fs, 0, src->fs, 0x10000);
	src->shader_enq = 42;

	//argument
	src->arg[0].fmt = 'm';
	src->arg[0].name = "objmat";
	src->arg[0].data = own->objmat;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "matter";
	tmp = src->arg[1].data = own->matter;
	tmp[0] = 0.1;
	tmp[1] = 1.0;
	tmp[2] = 1.0;

	//vertex
	src->geometry = 3;
	src->opaque = 0;

	src->vbuf_len = 0x1000000;
	src->vbuf = memorycreate(src->vbuf_len, 0);
	openreadclose(str, 0, src->vbuf, src->vbuf_len);
}
static void stl3d_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* wnd, struct style* none)
{
	gl41line_prism4(wnd, 0xff00ff, geom->fs.vc, geom->fs.vr, geom->fs.vf, geom->fs.vt);
	gl41line_rect(wnd, 0xff00ff, geom->fs.vc, geom->fs.vr, geom->fs.vf);

	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	mat4 mat;
	local2world(mat, &part->fs, &geom->fs);
	mat4_transposefrom(own->objmat, mat);

	struct glsrc* src = &own->gl41.src;
	gl41data_insert(wnd, 's', src, 1);

	vec3 tc;
	vec3_mat4vec3(tc, mat, &act->fx0);
	gl41solid_spheretest(wnd, 0xff00ff, tc);

	vec3 t0,t1,t2;
	vec3_mat4vec3(t0, mat, src->vbuf + act->data3);
	vec3_mat4vec3(t1, mat, src->vbuf + act->data3 + 24);
	vec3_mat4vec3(t2, mat, src->vbuf + act->data3 + 48);
	gl41solid_triangle(wnd, 0xffff00, t0,t1,t2);
}




static void stl3d_draw_pixel(
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
static void stl3d_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stl3d_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stl3d_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stl3d_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("stl3d(%x,%x,%x)\n",win,act,sty);
}




static void stl3d_event(
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
	else if(_drag_ == type)
	{
		char buffer[0x1000];
		ret = windowread(type, buffer);
		say("%s", buffer);

		for(j=0;j<ret;j++)
		{
			if(buffer[j] < 0x20)
			{
				buffer[j] = 0;
				break;
			}
		}
		//entitycreatefromfile(act, buffer);
	}
}
void stl3d_modify_matter(struct entity* act, int* src, int len)
{
	int j;
	struct privdata* own = act->CTXBUF;
	float* f = own->matter;
	f[0] = src[0]*0.01;
	f[1] = src[1]*0.01;
	f[2] = src[2]*0.01;
	say("%f,%f,%f\n",f[0],f[1],f[2]);
}
void stl3d_modify_ray(struct entity* act, vec3 ray[])
{
	float* ro = ray[0];
	float* rd = ray[1];
	say("world ray: %f,%f,%f -> %f,%f,%f\n",ro[0],ro[1],ro[2], rd[0],rd[1],rd[2]);

	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	int j;
	mat4 mat;
	vec3 r[2];
	world2local(mat, own->worldgeom, own->localgeom);
	for(j=0;j<3;j++)r[0][j] = ro[j]+rd[j];
	vec3_mat4vec3(r[1], mat, r[0]);
	vec3_mat4vec3(r[0], mat, ro);
	for(j=0;j<3;j++)r[1][j]-= r[0][j];
	say("local ray: %f,%f,%f -> %f,%f,%f\n",r[0][0],r[0][1],r[0][2], r[1][0],r[1][1],r[1][2]);
/*
mat4_transpose(mat);
mat4_multiply(mat,own->objmat);
printmat4(own->objmat);
printmat4(mat);
*/
	struct glsrc* src = &own->gl41.src;
	if(0 == src->vbuf_h)return;

	int ret;
	float* f;
	float* out = &act->fx0;
	float* tmp = &act->fxn;
	out[3] = 1000000000.0;
	for(j=0;j<src->vbuf_h/3;j++){
		f = src->vbuf + 4*6*3*j;
		ret = ray_trigon(tmp, r[0],r[1], f, &f[6], &f[12]);
		if(ret <= 0)continue;

		tmp[3] = 
			(tmp[0]-r[0][0])*(tmp[0]-r[0][0])+
			(tmp[1]-r[0][1])*(tmp[1]-r[0][1])+
			(tmp[2]-r[0][2])*(tmp[2]-r[0][2]);
		say("%d:@%f,%f,%f,%f(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)\n", j,tmp[0],tmp[1],tmp[2],tmp[3],
		f[0],f[1],f[2], f[6],f[7],f[8], f[12],f[13],f[14]);
		if(out[3] > tmp[3]){
			out[0] = tmp[0];
			out[1] = tmp[1];
			out[2] = tmp[2];
			out[3] = tmp[3];
			act->data3 = 4*6*3*j;
		}
	}
}




//[-6,-5]: wnd -> cam
//[-4,-3]: cam -> world
//[-2,-1]: world -> stl3d
static void stl3d_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wrd;struct style* camg;
	struct entity* wnd;struct style* area;

	if(stack&&('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wrd = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		stl3d_draw_gl41(ent,slot, scn,geom, wrd,camg, wnd,area);
	}
}
static void stl3d_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_int_ == foot)stl3d_modify_matter(ent, buf,len);
	else stl3d_modify_ray(ent, buf);
}
static void stl3d_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void stl3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act = self->pchip;
	if(0 == act)return;
	struct style* pin = self->pfoot;
	if(0 == pin)return;
	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	own->localgeom = self->pfoot;
	own->worldgeom = peer->pfoot;

	struct glsrc* src = &own->gl41.src;
	parsevertfromstl(src, &pin->fs, src->vbuf, src->vbuf_len);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_enq = 42;
}




static void stl3d_search(struct entity* act)
{
}
static void stl3d_modify(struct entity* act)
{
}
static void stl3d_delete(struct entity* act)
{
	if(0 == act)return;
}
static void stl3d_create(struct entity* act, void* str, int argc, u8** argv)
{
	int j;
	u8 vspath[128];
	u8 fspath[128];
	char* vs = 0;
	char* fs = 0;
	char* stl = 0;
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
	}
	if(0 == vs)vs = "datafile/shader/model/fv.glsl";
	if(0 == fs)fs = "datafile/shader/model/ff.glsl";
	if(0 == str)str = "datafile/stl/bunny-lowpoly.stl";

	act->CTXBUF = memorycreate(0x1000, 0);
	if(0 == act->CTXBUF)return;

	stl3d_ctxforwnd(act->CTXBUF, str, vs, fs);
}




void stl3d_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','t','l','3','d', 0, 0, 0);

	p->oncreate = (void*)stl3d_create;
	p->ondelete = (void*)stl3d_delete;
	p->onsearch = (void*)stl3d_search;
	p->onmodify = (void*)stl3d_modify;

	p->onlinkup = (void*)stl3d_linkup;
	p->ondiscon = (void*)stl3d_discon;
	p->onread  = (void*)stl3d_read;
	p->onwrite = (void*)stl3d_write;
}
