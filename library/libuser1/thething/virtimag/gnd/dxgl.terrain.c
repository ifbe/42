#include "libuser.h"
#define _int_ hex32('i','n','t', 0)
#define RGBTEX 0
#define DEPTEX 1
#define OWNBUF buf0
int copypath(u8* path, u8* data);
void dx11data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);
void gl41data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);


struct privdata{
	u8 vs[128];
	u8 fs[128];
	u8 rgb[128];
	u8 dep[128];

	mat4 mato2w;
	vec4 matter;

	struct vertex vtx;

	struct texture color;
	struct texture depth;

	struct dx11data dx11;
	struct gl41data gl41;
};
static int loadhlslfromfile(char* buf, char* url)
{
	int ret = openreadclose(url, 0, buf, 0xf000);
	buf[ret] = 0;
	return ret+1;
}
static int loadglslfromfile(char* buf, char* url)
{
	int ret = mysnprintf(buf, 99, "%s%s", GLSL_VERSION, GLSL_PRECISION);
	return openreadclose(url, 0, buf+ret, 0x10000-ret);
}




static void terrain_vtxprep(struct privdata* own)
{
	struct vertex* vtx = &own->vtx;

	vtx->vbuf_fmt = vbuffmt_333;
	vtx->vbuf_w = 4*9;
	vtx->vbuf_h = 256*255;
	vtx->vbuf_len = (vtx->vbuf_w) * 256*256;
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = 254*254*2;
	vtx->ibuf_len = (vtx->ibuf_w) * 256*256*2;
	vtx->ibuf = memorycreate(vtx->ibuf_len, 0);
}
static void terrain_texprep(struct privdata* own, char* rgb, char* dep)
{
	own->color.data = memorycreate(2048*2048*4, 0);
	if(0 == own->color.data)return;
	loadtexfromfile(&own->color, rgb);

	own->depth.data = memorycreate(2048*2048*4, 0);
	if(0 == own->depth.data)return;
	loadtexfromfile(&own->depth, dep);

	if(own->depth.w)return;
	if(own->depth.h)return;
	own->depth.w = 2048;
	own->depth.h = 2048;

	int x,y;
	u8* rgba = own->depth.data;
	for(y=0;y<2048;y++){
		for(x=0;x<2048;x++){
			rgba[4*(y*2048 + x)+0] = getrandom()%256;
			rgba[4*(y*2048 + x)+1] = getrandom()%256;
			rgba[4*(y*2048 + x)+2] = getrandom()%256;
		}
	}
}
static void terrain_generate(float (*vbuf)[9], u16* ibuf, struct entity* act, struct privdata* own)
{
	int x,y,j;
	int cx,cy,px,py;
	int w = own->depth.w;
	int h = own->depth.h;
	u8* rgba = own->depth.data;

	//cx,cy is integer
	cx = w * act->fx0;
	cy = h * act->fy0;
	for(y=0;y<255;y++){
		for(x=0;x<255;x++){
			//xyz[-1,1]
			px = cx+x-127;
			py = cy+y-127;
			vbuf[y*256+x][0] = (float)px/w*2 - 1.0;
			vbuf[y*256+x][1] = (float)py/h*2 - 1.0;

			//jpg inverts y?
			py = h-1-py;
			if((px < 0) | (px >= w) | (py < 0) | (py >= h))vbuf[y*256+x][2] = 0.0;
			vbuf[y*256+x][2] = rgba[(w*py+px)*4]/255.0;

			//uv[0,1]
			vbuf[y*256+x][6] = (float)px/w;
			vbuf[y*256+x][7] = (float)py/h;
			vbuf[y*256+x][8] = 0.0;
		}
	}

	float hl,hr,hn,hf;
	for(y=1;y<254;y++){
		for(x=1;x<254;x++){
			hl = vbuf[(y+0)*256+x-1][2];
			hr = vbuf[(y+0)*256+x+1][2];
			hn = vbuf[(y-1)*256+x+0][2];
			hf = vbuf[(y+1)*256+x+0][2];
			vbuf[y*256+x][3] = hl - hr;
			vbuf[y*256+x][4] = hn - hf;
			vbuf[y*256+x][5] = 2.0;
		}
	}

	j = 0;
	for(y=0;y<254;y++)
	{
		for(x=0;x<254;x++)
		{
			ibuf[j+0] = y*256+x;
			ibuf[j+1] = y*256+x+1;
			ibuf[j+2] = y*256+x+256;

			ibuf[j+3] = y*256+x+256;
			ibuf[j+4] = y*256+x+1;
			ibuf[j+5] = y*256+x+257;

			j += 6;
		}
	}
}/*
void terrain_locate(vec4 v, struct entity* act)
{
	//geometry
	int w = act->width;
	int h = act->height;

	//center
	float sx = (w-1) / 2.0;
	float sy = (h-1) / 2.0;

	//position
	float fx = sx + v[0] / 1000.0;
	float fy = sy - v[1] / 1000.0;

	//
	int x0;
	int x1;
	if(fx > 0){x0 = (int)fx;x1 = x0 + 1;}
	else{x1 = -(int)(-fx);x0 = x1 - 1;}

	//
	int y0;
	int y1;
	if(fx > 0){y0 = (int)fy;y1 = y0 + 1;}
	else{y1 = -(int)(-fy);y0 = y1 - 1;}

	//
	//say("%d,%d,%d,%d\n",x0,x1,y0,y1);
	if(x0 < 0)goto edge;
	if(x1 >= w)goto edge;
	if(y0 < 0)goto edge;
	if(y1 >= h)goto edge;

	//
	float val;
	float max;
	u8* rgba = act->DEPBUF;
	max = rgba[(w*y0 + x0)*4];
	val = rgba[(w*y0 + x1)*4];
	if(val > max)max = val;
	val = rgba[(w*y1 + x0)*4];
	if(val > max)max = val;
	val = rgba[(w*y1 + x1)*4];
	if(val > max)max = val;
	v[2] = max * 10000.0 / 255;
	return;

edge:
	v[2] = 0.0;
}
static void terrain_ask(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	float x,y;
	struct entity* act = (void*)(self->chip);
	int w = act->width;
	int h = act->height;
	float* v = (void*)buf;

	terrain_locate(v, act);
	say("%f,%f,%f\n", v[0], v[1], v[2]);

	x = v[0] / 1000.0;
	act->fxn = (int)x;
	while(act->fxn <= act->fx0-64){
		act->fx0 -= 64;
		act->iw0 = 0;
	}
	while(act->fxn >= act->fx0+64){
		act->fx0 += 64;
		act->iw0 = 0;
	}

	y = v[1] / 1000.0;
	act->fyn = (int)y;
	while(act->fyn <= act->fy0-64){
		act->fy0 -= 64;
		act->iw0 = 0;
	}
	while(act->fyn >= act->fy0+64){
		act->fy0 += 64;
		act->iw0 = 0;
	}

	say("%f,%f,%f,%f\n", act->fx0, act->fy0, act->fxn, act->fyn);
}*/




static void terrain_dx11prep(struct privdata* own, char* vs, char* ps)
{
	int ret;
	struct mysrc* src = &own->dx11.src;

	//shader
	src->vs = memorycreate(0x10000, 0);
	loadhlslfromfile(src->vs, vs);
	src->fs = memorycreate(0x10000, 0);
	loadhlslfromfile(src->fs, ps);
	src->shader_enq = 42;

	//texture
	struct texture* tex = &src->tex[RGBTEX];
	tex->data = own->color.data;
	tex->w    = own->color.w;
	tex->h    = own->color.h;
	tex->fmt = hex32('r','g','b','a');
	src->tex_enq[RGBTEX] = 42;

	//uniform
	src->uni[0].buf = own->mato2w;
	src->uni[0].len = sizeof(mat4);

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->opaque = 0;
	vtx->geometry = 3;

	vtx->vbuf_fmt = own->vtx.vbuf_fmt;
	vtx->vbuf_w   = own->vtx.vbuf_w;
	vtx->vbuf_h   = own->vtx.vbuf_h;
	vtx->vbuf_len = own->vtx.vbuf_len;
	vtx->vbuf     = own->vtx.vbuf;

	vtx->ibuf_fmt = own->vtx.ibuf_fmt;
	vtx->ibuf_w   = own->vtx.ibuf_w;
	vtx->ibuf_h   = own->vtx.ibuf_h;
	vtx->ibuf_len = own->vtx.ibuf_len;
	vtx->ibuf     = own->vtx.ibuf;

	src->vbuf_enq = 42;
	src->ibuf_enq = 42;
}
static void terrain_dx11draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* area)
{
	float w = vec3_getlen(geom->fs.vr);
	float h = vec3_getlen(geom->fs.vf);
	float x = camg->frus.vc[0]/w/2 + 0.5;
	float y = camg->frus.vc[1]/h/2 + 0.5;
	float dx = x - act->fx0;
	float dy = y - act->fy0;
	if(dx<0)dx = -dx;
	if(dy<0)dy = -dy;
	//say("x=%f,y=%f,dx=%f,dy=%f\n",x,y,dx,dy);

	struct privdata* own = act->OWNBUF;
	struct mysrc* src = &own->dx11.src;
	float* mat = (void*)src->uni[0].buf;
	void* vbuf;
	void* ibuf;
	if((dx > 1.0/16)|(dy > 1.0/16)){
		act->fx0 = x;
		act->fy0 = y;

		//x0,y0,z0,dx,dy,dz -> ndc
		vbuf = own->vtx.vbuf;
		if(0 == vbuf)return;
		ibuf = own->vtx.ibuf;
		if(0 == ibuf)return;
		terrain_generate(vbuf, ibuf, act, own);
		src->vbuf_enq += 1;

		//ndc -> geom
		mat[ 0] = geom->fs.vr[0];
		mat[ 1] = geom->fs.vf[0];
		mat[ 2] = geom->fs.vt[0];
		mat[ 3] = geom->fs.vc[0];
		mat[ 4] = geom->fs.vr[1];
		mat[ 5] = geom->fs.vf[1];
		mat[ 6] = geom->fs.vt[1];
		mat[ 7] = geom->fs.vc[1];
		mat[ 8] = geom->fs.vr[2];
		mat[ 9] = geom->fs.vf[2];
		mat[10] = geom->fs.vt[2];
		mat[11] = geom->fs.vc[2];
		mat[12] = 0.0;
		mat[13] = 0.0;
		mat[14] = 0.0;
		mat[15] = 1.0;
	}
	dx11data_insert(ctx, 's', src, 1);
}




static void terrain_gl41prep(struct privdata* own, char* vs, char* fs)
{
	float* tmp;
	struct gl41data* data = &own->gl41;
	struct mysrc* src = &data->src;
	struct gldst* dst = &data->dst;

	//shader
	src->vs = memorycreate(0x10000, 0);
	loadglslfromfile(src->vs, vs);
	src->fs = memorycreate(0x10000, 0);
	loadglslfromfile(src->fs, fs);
	src->shader_enq = 42;

	//argument
	dst->arg[0].name = "objmat";
	dst->arg[0].data = own->mato2w;
	dst->arg[0].fmt = 'm';

	dst->arg[1].fmt = 'v';
	dst->arg[1].name = "matter";
	tmp = dst->arg[1].data = own->matter;
	tmp[0] = 0.1;
	tmp[1] = 0.8;
	tmp[2] = 1.0;

	//texture
	struct texture* tex = &src->tex[RGBTEX];
	tex->data = own->color.data;
	tex->fmt = hex32('r','g','b','a');
	tex->w = own->color.w;
	tex->h = own->color.h;

	dst->texname[RGBTEX] = "rgbtex";
	src->tex_enq[RGBTEX] = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->opaque = 0;
	vtx->geometry = 3;

	vtx->vbuf_fmt = own->vtx.vbuf_fmt;
	vtx->vbuf_w   = own->vtx.vbuf_w;
	vtx->vbuf_h   = own->vtx.vbuf_h;
	vtx->vbuf_len = own->vtx.vbuf_len;
	vtx->vbuf     = own->vtx.vbuf;

	vtx->ibuf_fmt = own->vtx.ibuf_fmt;
	vtx->ibuf_w   = own->vtx.ibuf_w;
	vtx->ibuf_h   = own->vtx.ibuf_h;
	vtx->ibuf_len = own->vtx.ibuf_len;
	vtx->ibuf     = own->vtx.ibuf;

	src->vbuf_enq = 42;
	src->ibuf_enq = 42;
}
static void terrain_gl41draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* area)
{
	float w = vec3_getlen(geom->fs.vr);
	float h = vec3_getlen(geom->fs.vf);
	float x = camg->frus.vc[0]/w/2 + 0.5;
	float y = camg->frus.vc[1]/h/2 + 0.5;
	float dx = x - act->fx0;
	float dy = y - act->fy0;
	if(dx<0)dx = -dx;
	if(dy<0)dy = -dy;
	//say("x=%f,y=%f,dx=%f,dy=%f\n",x,y,dx,dy);

	struct privdata* own = act->OWNBUF;
	struct mysrc* src = &own->gl41.src;
	struct gldst* dst = &own->gl41.dst;
	float* mat;
	void* vbuf;
	void* ibuf;
	if((dx > 1.0/16)|(dy > 1.0/16)){
		act->fx0 = x;
		act->fy0 = y;

		//x0,y0,z0,dx,dy,dz -> ndc
		vbuf = own->vtx.vbuf;
		if(0 == vbuf)return;
		ibuf = own->vtx.ibuf;
		if(0 == ibuf)return;
		terrain_generate(vbuf, ibuf, act, own);
		src->vbuf_enq += 1;

		//ndc -> geom
		mat = dst->arg[0].data;
		if(0 == mat)return;
		mat[ 0] = geom->fs.vr[0];
		mat[ 1] = geom->fs.vr[1];
		mat[ 2] = geom->fs.vr[2];
		mat[ 3] = 0.0;
		mat[ 4] = geom->fs.vf[0];
		mat[ 5] = geom->fs.vf[1];
		mat[ 6] = geom->fs.vf[2];
		mat[ 7] = 0.0;
		mat[ 8] = geom->fs.vt[0];
		mat[ 9] = geom->fs.vt[1];
		mat[10] = geom->fs.vt[2];
		mat[11] = 0.0;
		mat[12] = geom->fs.vc[0];
		mat[13] = geom->fs.vc[1];
		mat[14] = geom->fs.vc[2];
		mat[15] = 1.0;
	}
	gl41data_insert(ctx, 's', src, 1);
}




static void terrain_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}


void terrain_modify_matter(struct entity* act, int* src, int len)
{
	int j;
	struct privdata* own = act->OWNBUF;
	float* f = own->matter;
	f[0] = src[0]*0.01;
	f[1] = src[1]*0.01;
	f[2] = src[2]*0.01;
	say("%f,%f,%f\n",f[0],f[1],f[2]);
}




static void terrain_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* dup;struct style* camg;
	struct entity* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11list_:terrain_dx11draw(ent,slot, wor,geom, dup,camg, wnd,area);break;
	case _gl41list_:terrain_gl41draw(ent,slot, wor,geom, dup,camg, wnd,area);break;
	}
}




static void terrain_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		terrain_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void terrain_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_int_ == stack[sp-1].flag)terrain_modify_matter(ent, buf,len);
}
static void terrain_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void terrain_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void terrain_search(struct entity* act)
{
}
static void terrain_modify(struct entity* act)
{
}
static void terrain_delete(struct entity* act)
{
	if(0 == act)return;
}
static void terrain_create(struct entity* act, void* arg, int argc, u8** argv)
{
	int j,k;
	if(0 == act)return;

	act->fx0 = -2.0;
	act->fy0 = -2.0;
	act->fz0 = -2.0;

	struct privdata* own = act->OWNBUF = memorycreate(0x10000, 0);
	if(0 == own)return;

	char* rgb = 0;
	char* dep = 0;
	char* dxvs = 0;
	char* dxps = 0;
	char* glvs = 0;
	char* glfs = 0;
	for(j=0;j<argc;j++){
		//say("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "rgb:", 4)){
			copypath(own->rgb, argv[j]+4);
			rgb = (void*)own->rgb;
		}
		if(0 == ncmp(argv[j], "dep:", 4)){
			copypath(own->dep, argv[j]+4);
			dep = (void*)own->dep;
		}
		if(0 == ncmp(argv[j], "glvs:", 5)){
			copypath(own->vs, argv[j]+5);
			glvs = (void*)own->vs;
		}
		if(0 == ncmp(argv[j], "glfs:", 5)){
			copypath(own->fs, argv[j]+5);
			glfs = (void*)own->fs;
		}
	}

	if(0 == rgb)rgb = "datafile/jpg/cartoon.jpg";
	if(0 == dep)dep = "datafile/jpg/cartoon.jpg";
	terrain_texprep(own, rgb, dep);
	terrain_vtxprep(own);

	if(0 == dxvs)dxvs = "datafile/shader/terrain/dxvs.hlsl";
	if(0 == dxps)dxps = "datafile/shader/terrain/dxps.hlsl";
	terrain_dx11prep(own, dxvs, dxps);

	if(0 == glvs)glvs = "datafile/shader/terrain/glvs.glsl";
	if(0 == glfs)glfs = "datafile/shader/terrain/glfs.glsl";
	terrain_gl41prep(own, glvs, glfs);
}




void terrain_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'r', 'r', 'a', 'i', 'n', 0);

	p->oncreate = (void*)terrain_create;
	p->ondelete = (void*)terrain_delete;
	p->onsearch = (void*)terrain_search;
	p->onmodify = (void*)terrain_modify;

	p->onlinkup = (void*)terrain_linkup;
	p->ondiscon = (void*)terrain_discon;
	p->ontaking = (void*)terrain_taking;
	p->ongiving = (void*)terrain_giving;
}
