#include "libuser.h"
#define _int_ hex32('i','n','t', 0)
#define CTXBUF listptr.buf0
//
int copypath(u8* path, u8* data);
void parsevertfromstl(struct fstyle* sty, int* vbuf_h, u8* buf, int len);
//
void dx11data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);
//
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);
void gl41solid_spheretest(_obj* win, u32 rgb, vec3 vc);
//
void world2local(mat4 mat, struct fstyle* src, struct fstyle* dst);
void local2world(mat4 mat, struct fstyle* src, struct fstyle* dst);
void world2clip_projznzp(mat4 mat, struct fstyle* frus);
void clip2world_projznzp(mat4 mat, struct fstyle* frus);
//
void mat4_transposefrom(mat4, mat4);
void mat4_multiplyfrom(mat4, mat4, mat4);
void vec3_normalizefrom(vec3 to, vec3 v);
//
int ray_trigon(vec3 out, vec3 ro, vec3 rd, vec3 t0, vec3 t1, vec3 t2);
int rastersolid_triangle(void*,void*, void*,void*, float*,int,int,int, mat4,void*);




struct privdata{
	struct fstyle* localgeom;
	struct fstyle* worldgeom;

	vec4 matter;
	mat4 objmat;

	int vbuf_w;
	int vbuf_h;
	int vbuf_len;
	void* vbuf;
	int chosen;

	struct gl41data gl41;
	struct dx11data dx11;
};
static u32 stl3d_normal2rgb(vec3 n)
{
	u32 r,g,b;
	float w = 0.5 / vec3_getlen(n);
	b = (u32)(256*(n[0]*w+0.5));
	g = (u32)(256*(n[1]*w+0.5));
	r = (u32)(256*(n[2]*w+0.5));
	return (r<<16) + (g<<8) + (b);
}
static u32 stl3d_mixcolor(u32 x, u32 y)
{
	u32 o = 0;
	u8* p = (void*)&x;
	u8* q = (void*)&y;
	int b = (p[0]+q[0])/2;
	int g = (p[1]+q[1])/2;
	int r = (p[2]+q[2])/2;
	return (r<<16) + (g<<8) + (b);
}
static u8 stl3d_24to4(u32 c)
{
	u8 o = 0;
	u8* p = (void*)&c;
	if(p[0] > 0x80)o |= 1;
	if(p[1] > 0x80)o |= 2;
	if(p[2] > 0x80)o |= 4;
	if(28*p[0]+151*p[1]+30*p[2] > 0x5600)o |= 8;
	return o;
}




static void stl3d_mat4vec3(vec3 o, mat4 m, vec3 v)
{
	float f;
	f = 1.00 / (m[3][0]*v[0] + m[3][1]*v[1] + m[3][2]*v[2] + m[3][3]);
	o[0] = f * (m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2] + m[0][3]);
	o[1] = f * (m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2] + m[1][3]);
	o[2] = f * (m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2] + m[2][3]);
}
static int stl3d_position(vec4 olist[], vec3 ilist[], mat4 m)
{
	float* oo = olist[0];
	float* ov = olist[1];
	float* on = olist[2];
	float* iv = ilist[0];
	float* in = ilist[1];

	ov[0] = iv[0];ov[1] = iv[1];ov[2] = iv[2];
	on[0] = in[0];on[1] = in[1];on[2] = in[2];

	//gl_Position = m*v
	oo[0] = m[0][0]*iv[0] + m[0][1]*iv[1] + m[0][2]*iv[2] + m[0][3];
	oo[1] = m[1][0]*iv[0] + m[1][1]*iv[1] + m[1][2]*iv[2] + m[1][3];
	oo[2] = m[2][0]*iv[0] + m[2][1]*iv[1] + m[2][2]*iv[2] + m[2][3];
	oo[3] = m[3][0]*iv[0] + m[3][1]*iv[1] + m[3][2]*iv[2] + m[3][3];
	return 0;
}
static u32 stl3d_fragment(vec4 out[], vec4 in[], vec4 uni[])
{
	float* n = in[1];
	float w = 0.5 / vec3_getlen(n);
	u32 b = (u32)(256*(n[0]*w+0.5));
	u32 g = (u32)(256*(n[1]*w+0.5));
	u32 r = (u32)(256*(n[2]*w+0.5));
	return (r<<16) + (g<<8) + (b);
}
static void stl3d_readdata(struct privdata* own, char* str)
{
	own->vbuf_len = 0x1000000;
	own->vbuf = memoryalloc(own->vbuf_len, 0);
	openreadclose(str, 0, own->vbuf, own->vbuf_len);
}
static void stl3d_modify_matter(_obj* act, int* src, int len)
{
	int j;
	struct privdata* own = act->CTXBUF;
	float* f = own->matter;
	f[0] = src[0]*0.01;
	f[1] = src[1]*0.01;
	f[2] = src[2]*0.01;
	logtoall("%f,%f,%f\n",f[0],f[1],f[2]);
}




static int stl3d_intersect_local(float* out, float* vbuf, int cnt, vec3 ro, vec3 rd)
{
	int j,k,ret;
	float* f;
	vec4 tmp;

	k = -1;
	out[3] = 1000000000.0;
	for(j=0;j<cnt;j++){
		f = &vbuf[6*3*j];
		ret = ray_trigon(tmp, ro,rd, f, &f[6], &f[12]);
		if(ret <= 0)continue;

		tmp[3] = (tmp[0]-ro[0])*(tmp[0]-ro[0]) + (tmp[1]-ro[1])*(tmp[1]-ro[1]) + (tmp[2]-ro[2])*(tmp[2]-ro[2]);
		//logtoall("%d=(%f,%f,%f),(%f,%f,%f),(%f,%f,%f)\n", j,f[0],f[1],f[2], f[6],f[7],f[8], f[12],f[13],f[14]);
		//logtoall("%d@(%f,%f,%f),%f\n", j,tmp[0],tmp[1],tmp[2],tmp[3]);
		if(out[3] > tmp[3]){
			out[0] = tmp[0];
			out[1] = tmp[1];
			out[2] = tmp[2];
			out[3] = tmp[3];
			k = j;
		}
	}
	return k;
}
static int stl3d_intersect_world(float* out, float* vbuf, int cnt, vec3 w_ro, vec3 w_rd, mat4 mat)
{
	//logtoall("world ray: %f,%f,%f -> %f,%f,%f\n",w_ro[0],w_ro[1],w_ro[2], w_rd[0],w_rd[1],w_rd[2]);

	int j;
	vec3 l_ro,l_rd;
	for(j=0;j<3;j++)l_ro[j] = w_ro[j]+w_rd[j];
	stl3d_mat4vec3(l_rd, mat, l_ro);
	stl3d_mat4vec3(l_ro, mat, w_ro);
	for(j=0;j<3;j++)l_rd[j]-= l_ro[j];
	//logtoall("local ray: %f,%f,%f -> %f,%f,%f\n",l_ro[0],l_ro[1],l_ro[2], l_rd[0],l_rd[1],l_rd[2]);

	//todo: return worldxyz, not localxyz
	return stl3d_intersect_local(out, vbuf, cnt, l_ro,l_rd);
}
static void stl3d_modify_ray(_obj* act, vec3 ray[])
{
	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	mat4 mat;
	world2local(mat, own->worldgeom, own->localgeom);

	int ret = stl3d_intersect_world(&act->whdf.fx0, own->vbuf, own->vbuf_h/3, ray[0],ray[1], mat);
	if(ret < 0)return;

	//save id
	logtoall("hit:%d\n",ret);
	own->chosen = 4*6*3*ret;

	//on hit
	vec3 to;
	float* at = own->vbuf + 4*6*3*ret;
	vec3_normalizefrom(to, &at[3]);
	logtoall("%f,%f,%f\n",to[0],to[1],to[2]);
	at[ 0] += to[0];at[ 1] += to[1];at[ 2] += to[2];
	at[ 6] += to[0];at[ 7] += to[1];at[ 8] += to[2];
	at[12] += to[0];at[13] += to[1];at[14] += to[2];
}




static void stl3d_dx11prep(struct privdata* own, char* vs, char* fs)
{
	float* tmp;
	struct mysrc* src = &own->dx11.src;

	//shader
	src->vs = memoryalloc(0x10000, 0);
	loadhlslfromfile(vs, 0, src->vs, 0x10000);
	src->fs = memoryalloc(0x10000, 0);
	loadhlslfromfile(fs, 0, src->fs, 0x10000);
	src->shader_enq = 42;

	//vertex
	struct vertex* vtx = &src->vtx[0];
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_len = own->vbuf_len;
	vtx->vbuf = own->vbuf;
}
static void stl3d_dx11draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* wrd, struct style* camg,
	_obj* wnd, struct style* area)
{
	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	struct dx11data* data = &own->dx11;
	data->src.uni[0].buf = own->objmat;
	data->src.uni[0].len = sizeof(mat4);

	float (*mat)[4] = own->objmat;
	local2world(mat, &part->fs, &geom->fs);

	dx11data_insert(wnd, 's', &own->dx11.src, 1);
}




static void stl3d_gl41prep(struct privdata* own, char* vs, char* fs)
{
	float* tmp;
	struct gl41data* data = &own->gl41;
	struct mysrc* src = &data->src;

	//shader
	src->vs = memoryalloc(0x10000, 0);
	loadglslfromfile(vs, 0, src->vs, 0x10000);
	src->fs = memoryalloc(0x10000, 0);
	loadglslfromfile(fs, 0, src->fs, 0x10000);
	src->shader_enq = 42;

	//argument
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "objmat";
	data->dst.arg[0].data = own->objmat;

	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "matter";
	tmp = data->dst.arg[1].data = own->matter;
	tmp[0] = 0.1;
	tmp[1] = 1.0;
	tmp[2] = 1.0;

	//vertex
	struct vertex* vtx = &src->vtx[0];
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_len = own->vbuf_len;
	vtx->vbuf = own->vbuf;
}
static void stl3d_gl41draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* wrd, struct style* camg,
	_obj* wnd, struct style* area)
{/*
	gl41line_prism4(wnd, 0xff00ff, geom->fs.vc, geom->fs.vr, geom->fs.vf, geom->fs.vt);
	gl41line_rect(wnd, 0xff00ff, geom->fs.vc, geom->fs.vr, geom->fs.vf);
*/
	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	mat4 mat;
	local2world(mat, &part->fs, &geom->fs);
	mat4_transposefrom(own->objmat, mat);

	vec3 tc,td;
	float* f = own->vbuf + own->chosen;
	tc[0] = act->whdf.fx0 + f[3]*100.0;
	tc[1] = act->whdf.fy0 + f[4]*100.0;
	tc[2] = act->whdf.fz0 + f[5]*100.0;
	stl3d_mat4vec3(td, mat, tc);
	stl3d_mat4vec3(tc, mat, &act->whdf.fx0);
	//gl41solid_spheretest(wnd, 0xff00ff, tc);
	gl41line(wnd, 0xffffff, tc, td);

	vec3 t0,t1,t2;
	stl3d_mat4vec3(t0, mat, f);
	stl3d_mat4vec3(t1, mat, &f[6]);
	stl3d_mat4vec3(t2, mat, &f[12]);
	gl41line_triangle(wnd, 0x00ffff, t0,t1,t2);
	gl41solid_triangle(wnd, 0xffff00, t0,t1,t2);

	//gl41data
	gl41data_insert(wnd, 's', &own->gl41.src, 1);
}




static void stl3d_rgba_test(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
void stl3d_rgba_theone(_obj* wnd, struct style* area, float* point, float* primi, mat4 mat)
{
	int dx = wnd->whdf.width * area->fs.vq[0] / 2;
	int dy = wnd->whdf.height* area->fs.vq[1] / 2;
	int cx = wnd->whdf.width * area->fs.vc[0] + dx;
	int cy = wnd->whdf.height* area->fs.vc[1] + dy;
	//logtoall("%d,%d,%d,%d\n", cx,cy, dx,dy);

	int x0,y0, x1,y1, x2,y2;
	vec3 t0,t1,t2;
	t0[0] = point[0] + primi[3]*100.0;
	t0[1] = point[1] + primi[4]*100.0;
	t0[2] = point[2] + primi[5]*100.0;
	stl3d_mat4vec3(t1, mat, t0);
	x1 = cx + dx*t1[0];
	y1 = cy - dy*t1[1];
	stl3d_mat4vec3(t0, mat, point);
	x0 = cx + dx*t0[0];
	y0 = cy - dy*t0[1];
	drawline(wnd, 0xffffff, x0,y0, x1,y1);


	stl3d_mat4vec3(t0, mat, primi);
	x0 = cx + dx*t0[0];
	y0 = cy - dy*t0[1];
	stl3d_mat4vec3(t1, mat, &primi[6]);
	x1 = cx + dx*t1[0];
	y1 = cy - dy*t1[1];
	stl3d_mat4vec3(t2, mat, &primi[12]);
	x2 = cx + dx*t2[0];
	y2 = cy - dy*t2[1];
	drawline_triangle(wnd, 0x00ffff, x0,y0, x1,y1, x2,y2);
	drawsolid_triangle(wnd, 0xffff00, x0,y0, x1,y1, x2,y2);
}
static void stl3d_rgba_raster(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* wrd, struct style* camg,
	_obj* wnd, struct style* area,
	mat4 clip_from_world)
{
	//logtoall("@stl3d: raster\n");
	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	mat4 m,world_from_local;
	local2world(world_from_local, &part->fs, &geom->fs);
	mat4_multiplyfrom(m, clip_from_world, world_from_local);
	//printmat4(world_from_local);

	rastersolid_triangle(
		wnd, area, stl3d_position, stl3d_fragment,
		own->vbuf, 6, 6*3, own->vbuf_h/3,
		m, own);
	stl3d_rgba_theone(wnd,area, &act->whdf.fx0,own->vbuf + own->chosen, m);
}
static void stl3d_rgba_raytrace(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wrd, struct style* camg,
	_obj* wnd, struct style* area,
	mat4 clip_from_world)
{
	int w = wnd->whdf.width;
	int h = wnd->whdf.height;
	int dx = w * area->fs.vq[0];
	int dy = h * area->fs.vq[1];
	int x0 = w * area->fs.vc[0];
	int y0 = h * area->fs.vc[1];
	int xn = x0+dx;
	int yn = y0+dy;
	//logtoall("%d,%d,%d,%d\n", x0,y0, xn,yn);

	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	mat4 mat,inv;
	world2local(mat, own->worldgeom, own->localgeom);
	clip2world_projznzp(inv, &camg->frus);

	void* rgba = wnd->rgbanode.buf;
	int stride = wnd->whdf.fbwidth;

	int x,y,j,ret;
	vec4 ro,rd, vv,oo;
	for(j=0;j<3;j++)ro[j] = camg->frus.vc[j];
	for(y=y0;y<yn;y++){
		for(x=y0;x<xn;x++){
			vv[0] = 2.0*(x     - x0) / dx - 1.0;
			vv[1] = 2.0*(h-1-y - y0) / dy - 1.0;
			vv[2] = 0.5;
			//logtoall("%f,%f,%f\n",v[0],v[1],v[2]);

			stl3d_mat4vec3(rd, inv, vv);
			for(j=0;j<3;j++)rd[j] -= ro[j];

			ret = stl3d_intersect_world(oo, own->vbuf, own->vbuf_h/3, ro,rd, mat);
			if(ret < 0)continue;

			*(u32*)(rgba + y*stride + x*4) = stl3d_normal2rgb(own->vbuf + 4*(18*ret+3));
		}
	}

	mat4 m,world_from_local;
	local2world(world_from_local, &part->fs, &geom->fs);
	mat4_multiplyfrom(m, clip_from_world, world_from_local);

	stl3d_rgba_theone(wnd,area, &act->whdf.fx0,own->vbuf + own->chosen, m);
}




static void stl3d_tui_test(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
	int x,y;
	int width = wnd->whdf.width;
	int height = wnd->whdf.height;
	u8* buf = (u8*)(wnd->tuitext.buf);

	for(y=0;y<height;y++){
		for(x=0;x<width;x++){
			buf[4*(width*y+x)+3] = 4;
		}
	}
}
static void stl3d_tui_raytrace(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wrd, struct style* camg,
	_obj* wnd, struct style* area,
	mat4 clip_from_world)
{
	int x,y,j;
	int top,bot,rgb,val;
	int www = wnd->whdf.width;
	int hhh = wnd->whdf.height;
	u8* buf = (u8*)(wnd->tuitext.buf);
	//logtoall("%d,%d,%d,%d\n", x0,y0, xn,yn);

	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	mat4 mat,inv;
	world2local(mat, own->worldgeom, own->localgeom);
	clip2world_projznzp(inv, &camg->frus);

	vec4 ro,rd, vv,oo;
	for(j=0;j<3;j++)ro[j] = camg->frus.vc[j];
	for(y=0;y<hhh;y++){
		for(x=0;x<www;x++){
			//top half
			vv[0] = 2.0*x/www - 1.0;
			vv[1] = 1.0 - 2.0*(y+y+0)/(hhh*2);
			vv[2] = 0.5;
			stl3d_mat4vec3(rd, inv, vv);
			for(j=0;j<3;j++)rd[j] -= ro[j];
			top = stl3d_intersect_world(oo, own->vbuf, own->vbuf_h/3, ro,rd, mat);

			//bot half
			vv[0] = 2.0*x/www - 1.0;
			vv[1] = 1.0 - 2.0*(y+y+1)/(hhh*2);
			vv[2] = 0.5;
			stl3d_mat4vec3(rd, inv, vv);
			for(j=0;j<3;j++)rd[j] -= ro[j];
			bot = stl3d_intersect_world(oo, own->vbuf, own->vbuf_h/3, ro,rd, mat);

			if((top < 0)&&(bot < 0))continue;
			else if(top < 0){
				val = 'b';
				rgb = stl3d_normal2rgb(own->vbuf + 4*(18*bot+3));
			}
			else if(bot < 0){
				val = 'p';
				rgb = stl3d_normal2rgb(own->vbuf + 4*(18*top+3));
			}
			else{
				val = '8';
				rgb = stl3d_mixcolor(
					stl3d_normal2rgb(own->vbuf + 4*(18*bot+3)),
					stl3d_normal2rgb(own->vbuf + 4*(18*top+3)));
			}

			j = (www*y+x)*4;
			buf[j + 0] = val;
			buf[j + 2] = stl3d_24to4(rgb);
		}//forx
	}//fory
}




static void stl3d_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stl3d_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stl3d_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("stl3d(%x,%x,%x)\n",win,act,sty);
}




//[-6,-5]: wnd -> cam
//[-4,-3]: cam -> world
//[-2,-1]: world -> stl3d
static void stl3d_world_camera_window(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key)
{
	_obj* scn;struct style* geom;
	_obj* wrd;struct style* camg;
	_obj* wnd;struct style* area;

	scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wrd = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	if(_rgba_ == wnd->vfmt){
		if(0==key)stl3d_rgba_raster(ent,slot, scn,geom, wrd,camg, wnd,area, (void*)arg);
		else stl3d_rgba_raytrace(ent,slot, scn,geom, wrd,camg, wnd,area, (void*)arg);
		return;
	}
	if(_tui_ == wnd->vfmt){
		//stl3d_tui_test(ent,slot, wnd,area);
		stl3d_tui_raytrace(ent,slot, scn,geom, wrd,camg, wnd,area, (void*)arg);
		return;
	}

	switch(wnd->vfmt){
	case _dx11list_:stl3d_dx11draw(ent,slot, scn,geom, wrd,camg, wnd,area);break;
	case _gl41list_:stl3d_gl41draw(ent,slot, scn,geom, wrd,camg, wnd,area);break;
	}
}
static void stl3d_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		stl3d_world_camera_window(ent,slot, stack,sp, arg,key);
	}
}
static void stl3d_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(_int_ == stack[sp-1].foottype)stl3d_modify_matter(ent, buf,len);
	else stl3d_modify_ray(ent, buf);
}
static void stl3d_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void stl3d_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* act = self->pchip;
	if(0 == act)return;
	struct style* pin = self->pfoot;
	if(0 == pin)return;
	struct privdata* own = act->CTXBUF;
	if(0 == own)return;

	//my own
	own->localgeom = self->pfoot;
	own->worldgeom = peer->pfoot;
	own->vbuf_w = 4*6;
	parsevertfromstl(&pin->fs, &own->vbuf_h, own->vbuf, own->vbuf_len);

	//for dx11
	struct mysrc* dxsrc = &own->dx11.src;
	struct vertex* dxvtx = &dxsrc->vtx[0];
	dxvtx->vbuf_w = own->vbuf_w;
	dxvtx->vbuf_h = own->vbuf_h;
	dxsrc->vbuf_enq = 42;

	//for gl41
	struct mysrc* mysrc = &own->gl41.src;
	struct vertex* glvtx = &mysrc->vtx[0];
	glvtx->vbuf_w = own->vbuf_w;
	glvtx->vbuf_h = own->vbuf_h;
	mysrc->vbuf_enq = 42;
}




static void stl3d_search(_obj* act)
{
}
static void stl3d_modify(_obj* act)
{
}
static void stl3d_delete(_obj* act)
{
	if(0 == act)return;
}
static void stl3d_create(_obj* act, void* str, int argc, u8** argv)
{
	int j;
	char* stl = 0;
	char* glvs = 0;
	char* glfs = 0;
	char* dxvs = 0;
	char* dxps = 0;
	u8 glvspath[128];
	u8 glfspath[128];
	u8 dxvspath[128];
	u8 dxpspath[128];
	if(0 == act)return;

	struct privdata* own = act->CTXBUF = memoryalloc(0x1000, 0);
	if(0 == own)return;

	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "dxvs:", 3)){
			copypath(dxvspath, argv[j]+5);
			dxvs = (void*)dxvspath;
		}
		if(0 == ncmp(argv[j], "dxps:", 3)){
			copypath(dxpspath, argv[j]+3);
			dxps = (void*)dxpspath;
		}
		if(0 == ncmp(argv[j], "vs:", 3)){
			copypath(glvspath, argv[j]+3);
			glvs = (void*)glvspath;
		}
		if(0 == ncmp(argv[j], "fs:", 3)){
			copypath(glfspath, argv[j]+3);
			glfs = (void*)glfspath;
		}
	}

	if(0 == str)str = "datafile/stl/bunny-lowpoly.stl";
	stl3d_readdata(own, str);

	if(0 == dxvs)dxvs = "datafile/shader/model/dxvs.hlsl";
	if(0 == dxps)dxps = "datafile/shader/model/dxps.hlsl";
	stl3d_dx11prep(own, dxvs, dxps);

	if(0 == glvs)glvs = "datafile/shader/model/fv.glsl";
	if(0 == glfs)glfs = "datafile/shader/model/ff.glsl";
	stl3d_gl41prep(own, glvs, glfs);
}




void stl3d_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('s','t','l','3','d', 0, 0, 0);

	p->oncreate = (void*)stl3d_create;
	p->ondelete = (void*)stl3d_delete;
	p->onreader = (void*)stl3d_search;
	p->onwriter = (void*)stl3d_modify;

	p->onattach = (void*)stl3d_attach;
	p->ondetach = (void*)stl3d_detach;
	p->ontaking = (void*)stl3d_taking;
	p->ongiving = (void*)stl3d_giving;
}
