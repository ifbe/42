#include "libuser.h"
#define _mind_ hex32('m','i','n','d')
#define OWNBUF buf0
//
#define EVTOTYPE data2
#define IOTO 1
#define EVTO 2
#define MIND 4
//
#define DRAWTYPE data3
#define RASTER 0
#define RAYTRACE 1
void matproj(mat4 mat, struct fstyle* sty);
void mat4_transposefrom(float* m, float* u);
void frustum2viewandclip_transpose(struct fstyle* frus, mat4 v_, mat4 vp);
int gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);
//cpurender
void pixel_clearcolor(void*);
void pixel_cleardepth(void*);
//directx
void dx11data_before(void*);
void dx11data_after(void*);
int dx11data_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//opengl
void gl41data_before(void*);
void gl41data_after(void*);
int gl41data_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//metal
void mt20data_before(void*);
void mt20data_after(void*);
int mt20data_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);




struct unidata{
	mat4 mat;
	vec4 vec;
};
struct privdata{
	struct halfrel* self;
	struct halfrel* peer;
	mat4 world2view;	//world to view
	mat4 world2clip;	//world to view to clip
	struct unidata uni;
	struct dx11data dx11;
	struct gl41data gl41;
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




static void freecam_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
}
static void freecam_modify(struct entity* act)
{
}
static void freecam_delete(struct entity* act)
{
}
static void freecam_create(struct entity* act, void* arg, int argc, u8** argv)
{
	int j;
    say("@freecam_create\n");

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
	}

	act->fx0 = 0.0;
	act->fy0 = 0.0;
	act->fz0 = 0.0;

	//matrix
	act->OWNBUF = memorycreate(0x1000, 0);
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
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
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
			act->iw0 = 0;
			return 0;
		}

		t = (void*)ev;
		if(0x2b70 == ev->what){
			act->ix0 = act->ixn = t[0];
			act->iy0 = act->iyn = t[1];
			act->iw0 = 1;
			return 0;
		}
		if(0x4070 == ev->what){
			if(0 == act->iw0)return 0;
			quaternion_operation(obb->vf, obb->vr, (t[1] - act->iyn)/100.0);
			quaternion_operation(obb->vt, obb->vr, (t[1] - act->iyn)/100.0);

			quaternion_operation(obb->vr, up, (t[0] - act->ixn)/100.0);
			quaternion_operation(obb->vf, up, (t[0] - act->ixn)/100.0);
			quaternion_operation(obb->vt, up, (t[0] - act->ixn)/100.0);

			//freecam_rotate(obb->vr, obb->vf, up, (t[0] - act->ixn)/100.0);
			//freecam_rotate(obb->vf, obb->vt, obb->vr, (t[1] - act->iyn)/100.0);
			act->ixn = t[0];
			act->iyn = t[1];
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
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
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
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	float dx = wnd->width;
	float dy = wnd->height * 2;
	frus->vb[3] = frus->vl[3] * dy / dx;
	frus->vt[3] = frus->vr[3] * dy / dx;
}
void freecam_ratio(
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct fstyle* rect = &area->fshape;
	struct fstyle* frus = &geom->frus;
	float dx = rect->vq[0] * wnd->width;
	float dy = rect->vq[1] * wnd->height;
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
static void freecam_frustum2matrix(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom)
{
	struct fstyle* frus = &geom->frus;
	struct privdata* own = act->OWNBUF;
	frustum2viewandclip_transpose(frus, own->world2view, own->world2clip);
	//printmat4(own->world2clip);
}
static void freecam_gl41cam(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	struct privdata* own = act->OWNBUF;

	struct gl41data* data = &own->gl41;
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = own->world2clip;
	data->dst.arg[1].fmt = 'm';
	data->dst.arg[1].name = "cammv_";
	data->dst.arg[1].data = own->world2view;
	data->dst.arg[2].fmt = 'v';
	data->dst.arg[2].name = "camxyz";
	data->dst.arg[2].data = frus->vc;
	wnd->glfull_camera[0] = data;
}
static void freecam_dx11cam(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	struct fstyle* frus = &geom->frus;
	struct privdata* own = act->OWNBUF;
	struct dxsrc* src = &own->dx11.src;

	mat4_transposefrom((void*)src->arg.mat, (void*)own->world2clip);
	for(j=0;j<3;j++)src->arg.vec[j] = frus->vc[j];

	wnd->glfull_camera[0] = (void*)src;
}
static void freecam_mt20cam(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y;
	struct privdata* own = act->OWNBUF;
	struct mt20data* data = &own->mt20;
	data->src.uni[0].buf = &own->uni;
	data->src.uni[0].len = sizeof(struct unidata);

	float* c = geom->frus.vc;
	float* vec = own->uni.vec;
	float (*mat)[4] = own->uni.mat;
	for(x=0;x<3;x++)vec[x] = c[x];
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)mat[y][x] = own->world2clip[y][x];
	}

	wnd->mtfull_camera[0] = data;
}




static int freecam_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* none)
{
	//frustum
	gl41frustum(ctx, &geom->frus);

	//ray from eye to far
	gl41line(ctx, 0, geom->frus.vc, &act->fx0);
	return 0;
}
static int freecam_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam,gl41 -> wor,geom		//the camera taking photo
//[-2,-1]: wor,geom -> ent,gl41		//the entity being taken
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	if(_tui_ == wnd->fmt)return 0;
	if(_rgba_ == wnd->fmt)return 0;
	if(_gl41full_ == wnd->fmt){
		if(stack&&('v' == key))freecam_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	return 0;
}
static int freecam_read_bywnd(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct privdata* own = ent->OWNBUF;
	struct halfrel* self = own->self;
	struct halfrel* peer = own->peer;
	stack[sp+0].pchip = self->pchip;
	stack[sp+0].pfoot = self->pfoot;
	stack[sp+0].flag = self->flag;
	stack[sp+1].pchip = peer->pchip;
	stack[sp+1].pfoot = peer->pfoot;
	stack[sp+1].flag = peer->flag;

//[+0,+1]: cam,towr -> wor,geom
//[-2,-1]: wnd,area -> cam,togl
	struct entity* wor;struct style* geom;
	struct style* slot;
	struct entity* wnd;struct style* area;
	wor = stack[sp+1].pchip;geom = stack[sp+1].pfoot;
	slot = stack[sp-1].pfoot;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	switch(wnd->fmt){

	case _cli_:
		say("\r%s/%s/%s:%f,%f,%f", &wnd->fmt, &ent->fmt, &wor->fmt, geom->fs.vc[0], geom->fs.vc[1], geom->fs.vc[2]);
		break;

	case _tui_:
	case _rgba_:
		//say("@freecam: raster\n");
		if(_tui_ == wnd->fmt)freecam_tui_ratio(wor, geom, wnd, area);
		else freecam_ratio(wor, geom, wnd, area);

		freecam_shape2frustum(&geom->fshape, &geom->frustum);
		//printstyle(&geom->frus);

		mat4 m;
		matproj(m, &geom->frus);
		//printmat4(m);

		pixel_cleardepth(wnd);
		entityread(stack[sp+1].pchip, 0, stack, sp+2, m, ent->DRAWTYPE, 0, 0);
		break;

	case _dx11full_:
		//clear all
		dx11data_before(wnd);
		//camera matrix
		freecam_ratio(wor, geom, wnd, area);
		freecam_shape2frustum(&geom->fshape, &geom->frustum);
		freecam_frustum2matrix(ent,slot, wor,geom);
		freecam_dx11cam(ent,slot, wor,geom, wnd,area);
		//render data
		dx11data_taking(wor,0, stack,sp+2, 0,'v', buf,len);
		//enq++
		dx11data_after(wnd);
		break;

	case _gl41full_:
		//clear all
		gl41data_before(wnd);
		//camera matrix
		freecam_ratio(wor, geom, wnd, area);
		freecam_shape2frustum(&geom->fshape, &geom->frustum);
		freecam_frustum2matrix(ent,slot, wor,geom);
		freecam_gl41cam(ent,slot, wor,geom, wnd,area);
		//render data
		gl41data_taking(wor,0, stack,sp+2, 0,'v', buf,len);
		//enq++
		gl41data_after(wnd);

		//let fbos draw, before window draw
		gl41data_taking(wor,0, stack,sp+2, 0,'?', buf,len);
		break;

	case _mt20full_:
		mt20data_before(wnd);
		//camera
		freecam_ratio(wor, geom, wnd, area);
		freecam_shape2frustum(&geom->fshape, &geom->frustum);
		freecam_frustum2matrix(ent,slot, wor,geom);
		freecam_mt20cam(ent,slot, wor,geom, wnd,area);
		//data
		mt20data_taking(wor,0, stack,sp+2, 0,'v', buf,len);
		//enq++
		mt20data_after(wnd);
		break;
	}
	return 0;
//say("@freecam_read_bywnd.end\n");
}
static int freecam_write_bywnd(_ent* ent,struct event* ev)
{
//find world from camera
	struct privdata* own = ent->OWNBUF;
	struct halfrel* rel = own->peer;

	struct entity* wor = rel->pchip;
	struct style* geom = rel->pfoot;
	switch(ent->EVTOTYPE){
	case 'f':return freecam_event_frus(ent,0, wor,geom, ev,0);break;
	default:return freecam_event_obb(ent,0, wor,geom, ev,0);break;
	}
	return 0;
}




static int freecam_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@freecam_read\n");
	if(sp < 2)return 0;
	take_data_from_peer(ent,_mind_, stack,sp, 0,0, 0,0);

	struct entity* sup = stack[sp-2].pchip;
	if(0 == sup)return 0;

	switch(sup->type){
	case _wnd_:
	case _fbo_:return freecam_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);
	default:return freecam_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
	}
	return 0;
}
static int freecam_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
	freecam_write_bywnd(ent, buf);
	return 0;
}
static void freecam_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void freecam_linkup(struct halfrel* self, struct halfrel* peer)
{
    say("@freecam_linkup\n");

	struct entity* this = self->pchip;
	if(_mind_ == self->flag){
		this->EVTOTYPE |= MIND;
		return;
	}
	if(_ioto_ == self->flag){
		this->EVTOTYPE |= IOTO;
		return;
	}
	if(_evto_ == self->flag){
		this->EVTOTYPE |= EVTO;
		return;
	}

	struct entity* that = peer->pchip;
	if( (_virtual_ == that->type) | (_scene3d_ == that->type) ){
		struct privdata* own = this->OWNBUF;
		own->self = self;
		own->peer = peer;
		return;
	}
}




void freecam_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('f', 'r', 'e', 'e', 'c', 'a', 'm', 0);

	p->oncreate = (void*)freecam_create;
	p->ondelete = (void*)freecam_delete;
	p->onsearch = (void*)freecam_search;
	p->onmodify = (void*)freecam_modify;

	p->onlinkup = (void*)freecam_linkup;
	p->ondiscon = (void*)freecam_discon;
	p->ontaking = (void*)freecam_taking;
	p->ongiving = (void*)freecam_giving;
}
