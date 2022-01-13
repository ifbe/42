#include "libuser.h"
#define _mind_ hex32('m','i','n','d')
#define OWNBUF listptr.buf4
//
#define EVTOTYPE listu64.data0
#define IOTO 1
#define EVTO 2
#define MIND 4
//
#define DRAWTYPE listu64.data1
#define RASTER 0
#define RAYTRACE 1
//myown, directx, metal
void world2clip_projz0z1_transpose(mat4 mat, struct fstyle* frus);
void clip2world_projz0z1_transpose(mat4 mat, struct fstyle* frus);
//opengl
void world2clip_projznzp_transpose(mat4 mat, struct fstyle* frus);
void clip2world_projznzp_transpose(mat4 mat, struct fstyle* frus);
//
void frustum2viewandclip_transpose(struct fstyle* frus, mat4 v_, mat4 vp);
//
int gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);
//cpurender
void pixel_clearcolor(void*);
void pixel_cleardepth(void*);
//
int copypath(void*, void*);




struct unidata{
	mat4 mat;
	vec4 vec;
};
struct privdata{
	struct halfrel* self;
	struct halfrel* peer;
	mat4 world2view;	//world to view
	mat4 world2clip;	//world to view to clip
	mat4 clip2world;	//clip to view to world
	struct unidata uni;

	struct dx11data dx11;

	struct gl41data gl41cam;
	struct gl41data gl41gbuf;

	struct mt20data mt20;
};




void printvec4(float* s)
{
	say("%f, %f, %f, %f\n", s[0], s[1], s[2], s[3]);
}
void printstyle(struct fstyle* sty)
{
	printvec4(sty->vl);
	printvec4(sty->vr);
	printvec4(sty->vb);
	printvec4(sty->vt);
	printvec4(sty->vn);
	printvec4(sty->vf);
	printvec4(sty->vq);
	printvec4(sty->vc);
}
void printmat4(float* f)
{
	printvec4(&f[0]);
	printvec4(&f[4]);
	printvec4(&f[8]);
	printvec4(&f[12]);
}




void freecam_move(vec3 dst, vec3 src, float t)
{
	dst[0] += src[0] * t;
	dst[1] += src[1] * t;
	dst[2] += src[2] * t;
}
void freecam_rotate(vec3 a, vec3 b, vec3 axis, float angle)
{
	quaternion_operation(a, axis, angle);
	quaternion_operation(b, axis, angle);
}
static int freecam_event_obb(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	struct event* ev, int len)
{
	short* t;
	float nx,ny,nz;
	vec3 up = {0.0, 0.0, 1.0};
	//say("freecam_event@%llx:%x,%x\n", act, ev->why, ev->what);

	struct fstyle* obb = &geom->fshape;
	if('p' == (ev->what&0xff))
	{
		if(0x2d70 == ev->what){
			act->whdf.iw0 = 0;
			return 0;
		}

		t = (void*)ev;
		if(0x2b70 == ev->what){
			act->whdf.ix0 = act->whdf.ixn = t[0];
			act->whdf.iy0 = act->whdf.iyn = t[1];
			act->whdf.iw0 = 1;
			return 0;
		}
		if(0x4070 == ev->what){
			if(0 == act->whdf.iw0)return 0;
			quaternion_operation(obb->vf, obb->vr, (t[1] - act->whdf.iyn)/100.0);
			quaternion_operation(obb->vt, obb->vr, (t[1] - act->whdf.iyn)/100.0);

			quaternion_operation(obb->vr, up, (t[0] - act->whdf.ixn)/100.0);
			quaternion_operation(obb->vf, up, (t[0] - act->whdf.ixn)/100.0);
			quaternion_operation(obb->vt, up, (t[0] - act->whdf.ixn)/100.0);

			//freecam_rotate(obb->vr, obb->vf, up, (t[0] - act->whdf.ixn)/100.0);
			//freecam_rotate(obb->vf, obb->vt, obb->vr, (t[1] - act->whdf.iyn)/100.0);
			act->whdf.ixn = t[0];
			act->whdf.iyn = t[1];
		}
		return 0;
	}
	else if('j' == (ev->what&0xff))
	{
		t = (void*)ev;
		if(joy_left == (ev->what & joy_mask))
		{
			if((t[0]<-4096)|(t[0]>4096)){
				obb->vc[0] += t[0]/256;
			}
			if((t[1]<-4096)|(t[1]>4096)){
				obb->vc[1] += t[1]/256;
			}
			if(t[3] & joyl_left)		//x-
			{
				obb->vc[0] -= 10;
			}
			if(t[3] & joyl_right)		//x+
			{
				obb->vc[0] += 10;
			}
			if(t[3] & joyl_down)		//y-
			{
				obb->vc[1] -= 10;
			}
			if(t[3] & joyl_up)			//y+
			{
				obb->vc[1] += 10;
			}
			if(t[3] & joyl_trigger)		//z-
			{
				obb->vc[2] -= 10;
			}
			if(t[3] & joyl_bumper)		//z+
			{
				obb->vc[2] += 10;
			}
		}
		else if(joy_right == (ev->what & joy_mask))
		{
			if((t[0]<-4096)|(t[0]>4096)){
				freecam_rotate(obb->vr, obb->vf, obb->vt,-t[0]/1048576.0);
			}
			if((t[1]<-4096)|(t[1]>4096)){
				freecam_rotate(obb->vf, obb->vt, obb->vr, t[1]/1048576.0);
			}
			if(t[3] & joyr_left)		//x-
			{
				freecam_rotate(obb->vr, obb->vf, obb->vt, 0.05);
			}
			if(t[3] & joyr_right)		//x+
			{
				freecam_rotate(obb->vr, obb->vf, obb->vt,-0.05);
			}
			if(t[3] & joyr_down)		//y-
			{
				freecam_rotate(obb->vf, obb->vt, obb->vr,-0.05);
			}
			if(t[3] & joyr_up)			//y+
			{
				freecam_rotate(obb->vf, obb->vt, obb->vr, 0.05);
			}
			if(t[3] & joyr_trigger)		//z-
			{
				freecam_rotate(obb->vr, obb->vt, obb->vf,-0.05);
			}
			if(t[3] & joyr_bumper)		//z+
			{
				freecam_rotate(obb->vr, obb->vt, obb->vf, 0.05);
			}
		}
	}
	else if(_char_ == ev->what){
		switch(ev->why){
			case 'a':freecam_move(obb->vc, obb->vr,-1.0);break;
			case 'd':freecam_move(obb->vc, obb->vr, 1.0);break;
			case 's':freecam_move(obb->vc, obb->vf,-1.0);break;
			case 'w':freecam_move(obb->vc, obb->vf, 1.0);break;
			case 'q':freecam_move(obb->vc, obb->vt,-1.0);break;
			case 'e':freecam_move(obb->vc, obb->vt, 1.0);break;

			case 'j':freecam_rotate(obb->vr, obb->vf, obb->vt, 0.05);break;
			case 'l':freecam_rotate(obb->vr, obb->vf, obb->vt,-0.05);break;
			case 'i':freecam_rotate(obb->vf, obb->vt, obb->vr, 0.05);break;
			case 'k':freecam_rotate(obb->vf, obb->vt, obb->vr,-0.05);break;
			case 'u':freecam_rotate(obb->vr, obb->vt, obb->vf,-0.05);break;
			case 'o':freecam_rotate(obb->vr, obb->vt, obb->vf, 0.05);break;
		}
	}

	return 1;
}
static int freecam_event_frus(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	struct event* ev, int len)
{
	short* t;
	struct fstyle* frus;
	float nx,ny,nz;

	frus = &geom->frustum;
	if(_char_ == ev->what){
		switch(ev->why){
			case 'a':frus->vl[3]+=0.01;frus->vr[3]-=0.01;break;
			case 'd':frus->vl[3]-=0.01;frus->vr[3]+=0.01;break;
			case 's':frus->vn[3]-=0.01;break;
			case 'w':frus->vn[3]+=0.01;break;
		}
	}
	else if('j' == (ev->what&0xff))
	{
		t = (void*)ev;
		if(joy_left == (ev->what & joy_mask))
		{
			if(t[3] & joyl_left)		//x-
			{
				frus->vl[3]+=0.01;frus->vr[3]-=0.01;
			}
			if(t[3] & joyl_right)		//x+
			{
				frus->vl[3]-=0.01;frus->vr[3]+=0.01;
			}
			if(t[3] & joyl_down)		//y-
			{
				frus->vn[3]-=0.01;
			}
			if(t[3] & joyl_up)			//y+
			{
				frus->vn[3]+=0.01;
			}
		}
		else if(joy_right == (ev->what & joy_mask))
		{
		}
	}
	return 0;
}




void freecam_tui_ratio(
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	float dx = wnd->whdf.width;
	float dy = wnd->whdf.height * 2;
	frus->vb[3] = frus->vl[3] * dy / dx;
	frus->vt[3] = frus->vr[3] * dy / dx;
}
void freecam_ratio(
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct fstyle* rect = &area->fshape;
	struct fstyle* frus = &geom->frus;
	float dx = rect->vq[0] * wnd->whdf.width;
	float dy = rect->vq[1] * wnd->whdf.height;
	frus->vb[3] = frus->vl[3] * dy / dx;
	frus->vt[3] = frus->vr[3] * dy / dx;
}
void freecam_shape2frustum(struct fstyle* s, struct fstyle* d)
{
	float x,y,z,n;
	d->vc[0] = s->vc[0];
	d->vc[1] = s->vc[1];
	d->vc[2] = s->vc[2];


	x = s->vr[0];
	y = s->vr[1];
	z = s->vr[2];
	n = squareroot(x*x + y*y + z*z);
	d->vr[0] = x / n;
	d->vr[1] = y / n;
	d->vr[2] = z / n;
	//d->vr[3] = 1.0;
	d->vl[0] = -x / n;
	d->vl[1] = -y / n;
	d->vl[2] = -z / n;
	//d->vl[3] = -1.0;


	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	//d->vt[3] = 1.0;
	d->vb[0] = -x / n;
	d->vb[1] = -y / n;
	d->vb[2] = -z / n;
	//d->vb[3] = -1.0;


	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vn[0] = x / n;
	d->vn[1] = y / n;
	d->vn[2] = z / n;
	//d->vn[3] = 1.0;
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	//d->vf[3] = 1e20;
}
static void freecam_frus2wvp(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom)
{
	struct fstyle* frus = &geom->frus;
	struct privdata* own = act->OWNBUF;
	frustum2viewandclip_transpose(frus, own->world2view, own->world2clip);

	//say("wvp:\n");
	//printmat4(own->world2clip);
}
static void freecam_frus2pvw(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom)
{
	struct fstyle* frus = &geom->frus;
	struct privdata* own = act->OWNBUF;
	clip2world_projz0z1_transpose(own->clip2world, frus);

	//say("pvw:\n");
	//printmat4(own->clip2world);
}




static void freecam_gl41_cam(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	struct privdata* own = act->OWNBUF;

	struct gl41data* data = &own->gl41cam;
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = own->world2clip;
	data->dst.arg[1].fmt = 'm';
	data->dst.arg[1].name = "cammv_";
	data->dst.arg[1].data = own->world2view;
	data->dst.arg[2].fmt = 'v';
	data->dst.arg[2].name = "camxyz";
	data->dst.arg[2].data = frus->vc;
	wnd->gl41list.world[0].camera[0] = data;
}
static int freecam_gl41_mesh(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* none)
{
	//frustum
	gl41frustum(ctx, &geom->frus);

	//ray from eye to far
	gl41line(ctx, 0, geom->frus.vc, &act->whdf.fx0);
	return 0;
}




static void freecam_gl41gbuf_world1_cam(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	struct privdata* own = act->OWNBUF;

	struct gl41data* data = &own->gl41cam;
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = own->world2clip;
	data->dst.arg[1].fmt = 'm';
	data->dst.arg[1].name = "cammv_";
	data->dst.arg[1].data = own->world2view;
	data->dst.arg[2].fmt = 'v';
	data->dst.arg[2].name = "camxyz";
	data->dst.arg[2].data = frus->vc;

	data->src.tex[0].w = wnd->whdf.width;
	data->src.tex[0].h = wnd->whdf.height;
	data->src.tex[0].fmt = 0;
	data->src.tex[0].glfd = 0;

	data->src.type = 'g';
	data->src.target_enq = 42;

	wnd->gl41list.world[0].camera[0] = data;
}
static void freecam_gl41gbuf_world2_prep(_obj* act, void* vs, void* fs)
{
	struct privdata* own = act->OWNBUF;
	struct gl41data* data = &own->gl41gbuf;

	//shader
	say("vs=%s,fs=%s\n", vs, fs);
	data->src.vs = memorycreate(0x1000, 0);
	openreadclose(vs, 0, data->src.vs, 0x1000);
	data->src.fs = memorycreate(0x1000, 0);
	openreadclose(fs, 0, data->src.fs, 0x1000);
	data->src.shader_enq = 42;

	//vertex
	struct vertex* vtx = data->src.vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);

	//texture
	data->dst.texname[0] = "tex0";
	data->dst.texname[1] = "tex1";
	data->dst.texname[2] = "tex2";
	data->dst.texname[3] = "tex3";
	data->dst.texname[4] = "tex4";

	data->src.vbuf_enq = 42;
}
static void freecam_gl41gbuf_world2_mesh(
	_obj* act, struct style* slot,
	_obj* wrd, struct style* geom,
	_obj* ogl, struct style* area)
{
	struct privdata* own = act->OWNBUF;
	struct gl41data* fbo = &own->gl41cam;
	struct gl41data* data = &own->gl41gbuf;

	//arg
	struct fstyle* frus = &geom->frus;
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "invwvp";
	data->dst.arg[0].data = own->clip2world;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = frus->vc;

	//vertex
	float (*vbuf)[6] = data->src.vtx[0].vbuf;
	if(0 == vbuf)return;

	vbuf[0][0] = -1.0;
	vbuf[0][1] = -1.0;
	vbuf[0][2] = 0.0;
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = 1.0;
	vbuf[1][1] = 1.0;
	vbuf[1][2] = 0.0;
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = -1.0;
	vbuf[2][1] = 1.0;
	vbuf[2][2] = 0.0;
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = 1.0;
	vbuf[3][1] = 1.0;
	vbuf[3][2] = 0.0;
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = -1.0;
	vbuf[4][1] = -1.0;
	vbuf[4][2] = 0.0;
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = 1.0;
	vbuf[5][1] = -1.0;
	vbuf[5][2] = 0.0;
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	data->src.vbuf_enq += 1;

	//texture
	data->src.tex[0].glfd = fbo->dst.tex[0];
	data->src.tex[0].fmt = '!';
	data->src.tex_enq[0] += 1;

	data->src.tex[1].glfd = fbo->dst.tex[1];
	data->src.tex[1].fmt = '!';
	data->src.tex_enq[1] += 1;

	data->src.tex[2].glfd = fbo->dst.tex[2];
	data->src.tex[2].fmt = '!';
	data->src.tex_enq[2] += 1;

	data->src.tex[3].glfd = fbo->dst.tex[3];
	data->src.tex[3].fmt = '!';
	data->src.tex_enq[3] += 1;

	data->src.tex[4].glfd = fbo->dst.rbo;
	data->src.tex[4].fmt = '!';
	data->src.tex_enq[4] += 1;

	ogl->gl41list.world[1].solid[0] = data;
}




static void freecam_dx11_cam(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct privdata* own = act->OWNBUF;
	struct dx11data* data = &own->dx11;
	data->src.uni[0].buf = &own->uni;
	data->src.uni[0].len = sizeof(struct unidata);

	int x,y;
	float* c = geom->frus.vc;
	float* vec = own->uni.vec;
	float (*mat)[4] = own->uni.mat;
	for(x=0;x<3;x++)vec[x] = c[x];
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)mat[y][x] = own->world2clip[x][y];
	}

	wnd->dx11list.camera[0] = data;
}




static void freecam_mt20_cam(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct privdata* own = act->OWNBUF;
	struct mt20data* data = &own->mt20;
	data->src.uni[0].buf = &own->uni;
	data->src.uni[0].len = sizeof(struct unidata);

	int x,y;
	float* c = geom->frus.vc;
	float* vec = own->uni.vec;
	float (*mat)[4] = own->uni.mat;
	for(x=0;x<3;x++)vec[x] = c[x];
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)mat[y][x] = own->world2clip[y][x];
	}

	wnd->mt20list.camera[0] = data;
}




static int freecam_byworld_bycam_bywnd_read(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam,gl41 -> wor,geom		//the camera taking photo
//[-2,-1]: wor,geom -> ent,gl41		//the entity being taken
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	switch(wnd->hfmt){
	case _tui_:
	case _rgba_:
		return 0;
	case _gl41list_:
		 freecam_gl41_mesh(ent,slot, wor,geom, wnd,area);
	}
	return 0;
}




static int freecam_generate(_obj* ent,void* slot, _syn* stack,int sp, _obj* wor,struct style* geom, _obj* wnd,struct style* area)
{
	switch(wnd->hfmt){

	case _cli_:
		say("\r%s/%s/%s:%f,%f,%f", &wnd->hfmt, &ent->hfmt, &wor->hfmt, geom->fs.vc[0], geom->fs.vc[1], geom->fs.vc[2]);
		break;

	case _tui_:
	case _rgba_:
		//say("@freecam: raster\n");
		if(_tui_ == wnd->hfmt)freecam_tui_ratio(wor, geom, wnd, area);
		else freecam_ratio(wor, geom, wnd, area);

		freecam_shape2frustum(&geom->fshape, &geom->frustum);
		//printstyle(&geom->frus);

		mat4 m;
		world2clip_projz0z1_transpose(m, &geom->frus);
		//printmat4(m);

		pixel_cleardepth(wnd);
		entity_take(stack[sp+1].pchip, 0, stack, sp+2, m, ent->DRAWTYPE, 0, 0);
		break;

	case _dx11list_:
		//clear all
		dx11data_before(wnd);
		//camera matrix
		freecam_ratio(wor, geom, wnd, area);
		freecam_shape2frustum(&geom->fshape, &geom->frustum);
		freecam_frus2wvp(ent,slot, wor,geom);
		freecam_dx11_cam(ent,slot, wor,geom, wnd,area);
		//render data
		dx11data_taking(wor,0, stack,sp+2, 0,'v', 0,0);
		//enq++
		dx11data_after(wnd);
		break;

	case _gl41list_:
		//clear all
		gl41data_before(wnd);
		//render data
		gl41data_taking(wor,0, stack,sp+2, 0,'v', 0,0);
		//enq++
		gl41data_after(wnd);

		//camera matrix
		freecam_ratio(wor, geom, wnd, area);
		freecam_shape2frustum(&geom->fshape, &geom->frustum);
		freecam_frus2wvp(ent,slot, wor,geom);
		if(_gbuf_ == wnd->vfmt){
			freecam_frus2pvw(ent,slot, wor,geom);
			freecam_gl41gbuf_world1_cam(ent,slot, wor,geom, wnd,area);
			freecam_gl41gbuf_world2_mesh(ent,slot, wor,geom, wnd,area);
		}
		else{
			freecam_gl41_cam(ent,slot, wor,geom, wnd,area);
		}
		break;
/*
	case deferred_rendering:
		//stage1: read albedo,normal,matter, render to tex0,1,2,3
		//stage2: use tex0,1,2,3, render to window
		break;
*/
	case _mt20list_:
		mt20data_before(wnd);
		//camera
		freecam_ratio(wor, geom, wnd, area);
		freecam_shape2frustum(&geom->fshape, &geom->frustum);
		freecam_frus2wvp(ent,slot, wor,geom);
		freecam_mt20_cam(ent,slot, wor,geom, wnd,area);
		//data
		mt20data_taking(wor,0, stack,sp+2, 0,'v', 0,0);
		//enq++
		mt20data_after(wnd);
		break;
	}
	return 0;
}




static int freecam_bywnd_read(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct privdata* own = ent->OWNBUF;
	struct halfrel* self = own->self;
	struct halfrel* peer = own->peer;
	stack[sp+0].pchip = self->pchip;
	stack[sp+0].pfoot = self->pfoot;
	stack[sp+0].foottype = self->foottype;
	stack[sp+1].pchip = peer->pchip;
	stack[sp+1].pfoot = peer->pfoot;
	stack[sp+1].foottype = peer->foottype;

//[+0,+1]: cam,towr -> wor,geom
//[-2,-1]: wnd,area -> cam,togl
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp+1].pchip;geom = stack[sp+1].pfoot;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	freecam_generate(ent,slot, stack,sp, wor,geom, wnd,area);
	return 0;
//say("@freecam_bywnd_read.end\n");
}
static int freecam_bywnd_write(_obj* ent,struct event* ev)
{
//find world from camera
	struct privdata* own = ent->OWNBUF;
	struct halfrel* rel = own->peer;

	_obj* wor = rel->pchip;
	struct style* geom = rel->pfoot;
	switch(ent->EVTOTYPE){
	case 'f':return freecam_event_frus(ent,0, wor,geom, ev,0);break;
	default:return freecam_event_obb(ent,0, wor,geom, ev,0);break;
	}
	return 0;
}




static int freecam_byrts_bywnd_read(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@%s\n",__FUNCTION__);
	struct privdata* own = ent->OWNBUF;
	struct halfrel* self = own->self;
	struct halfrel* peer = own->peer;
	stack[sp+0].pchip = self->pchip;
	stack[sp+0].pfoot = self->pfoot;
	stack[sp+0].foottype = self->foottype;
	stack[sp+1].pchip = peer->pchip;
	stack[sp+1].pfoot = peer->pfoot;
	stack[sp+1].foottype = peer->foottype;

//[+0,+1]: cam,towr -> wor,geom
//[-2,-1]: mixer,area -> cam,togl
//[-4,-3]: wnd,0 -> mixer,0
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp+1].pchip;geom = stack[sp+1].pfoot;
	wnd = stack[sp-4].pchip;area = stack[sp-2].pfoot;

	freecam_generate(ent,slot, stack,sp, wor,geom, wnd,area);
	return 0;
}




static int freecam_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@freecam_read\n");
	if(0 == stack)return 0;
	take_data_from_peer(ent,_mind_, stack,sp, 0,0, 0,0);

	_obj* caller = stack[sp-2].pchip;
	if(0 == caller)return 0;

	switch(caller->type){
	case _wnd_:
		return freecam_bywnd_read(ent,foot, stack,sp, arg,key, buf,len);
	case _camrts_:
		return freecam_byrts_bywnd_read(ent,foot, stack,sp, arg,key, buf,len);
	//case _?_:
		//return freecam_byworld_bycam_byrts_bywnd_read(ent,foot, stack,sp, arg,key, buf,len);
	default:
		return freecam_byworld_bycam_bywnd_read(ent,foot, stack,sp, arg,key, buf,len);
	}
	return 0;
}
static int freecam_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct event* ev = buf;
	if(ent->EVTOTYPE & MIND){
		give_data_into_peer(ent,_mind_, stack,sp, arg,key, buf,len);
		return 0;
	}
	if(ent->EVTOTYPE & EVTO){
		if(_char_ != ev->what){
			give_data_into_peer(ent,_evto_, stack,sp, arg,key, buf,len);
			return 0;
		}
	}
	if(ent->EVTOTYPE & IOTO){
		if(_char_ == ev->what){
			give_data_into_peer(ent,_ioto_, stack,sp, arg,key, buf,len);
			return 0;
		}
	}
	freecam_bywnd_write(ent, buf);
	return 0;
}
static void freecam_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void freecam_attach(struct halfrel* self, struct halfrel* peer)
{
    say("@freecam_attach\n");

	_obj* this = self->pchip;
	switch(self->foottype){
	case _mind_:
		this->EVTOTYPE |= MIND;
		return;
	case _ioto_:
		this->EVTOTYPE |= IOTO;
		return;
	case _evto_:
		this->EVTOTYPE |= EVTO;
		return;
	}

	_obj* that = peer->pchip;
	if( (_virtual_ == that->type) | (_scene3d_ == that->type) ){
		struct privdata* own = this->OWNBUF;
		own->self = self;
		own->peer = peer;
		return;
	}
}




static void freecam_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
}
static void freecam_modify(_obj* act)
{
}
static void freecam_delete(_obj* act)
{
}
static void freecam_create(_obj* act, void* arg, int argc, u8** argv)
{
	int j;
    say("@freecam_create\n");

	u8* glvs = (void*)"datafile/shader/deferred/vert.glsl";
	u8* glfs = (void*)"datafile/shader/deferred/debug.glsl";
	u8 glvsbuf[128];
	u8 glfsbuf[128];

	//script
	act->EVTOTYPE = 0;
	act->DRAWTYPE = getrandom()&1;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "render:", 7)){
			if('0' == argv[j][7])act->DRAWTYPE = 0;
			if('1' == argv[j][7])act->DRAWTYPE = 1;
		}
		if(0 == ncmp(argv[j], "script:", 7)){
			if('f' == argv[j][7])act->EVTOTYPE = 'f';
		}
		if(0 == ncmp(argv[j], "glvs:", 5)){
			copypath(glvsbuf, argv[j] + 5);
			glvs = glvsbuf;
		}
		if(0 == ncmp(argv[j], "glfs:", 5)){
			copypath(glfsbuf, argv[j] + 5);
			glfs = glfsbuf;
		}
	}

	act->whdf.fx0 = 0.0;
	act->whdf.fy0 = 0.0;
	act->whdf.fz0 = 0.0;

	//matrix
	act->OWNBUF = memorycreate(0x1000, 0);

	freecam_gl41gbuf_world2_prep(act, glvs, glfs);
}




void freecam_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('f', 'r', 'e', 'e', 'c', 'a', 'm', 0);

	p->oncreate = (void*)freecam_create;
	p->ondelete = (void*)freecam_delete;
	p->onsearch = (void*)freecam_search;
	p->onmodify = (void*)freecam_modify;

	p->onattach = (void*)freecam_attach;
	p->ondetach = (void*)freecam_detach;
	p->ontaking = (void*)freecam_taking;
	p->ongiving = (void*)freecam_giving;
}
