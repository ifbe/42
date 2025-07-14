#include "libuser.h"
void world2view_righthand_transpose(mat4 m, struct fstyle* s);
//
int gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);
//cpurender
void pixel_clearcolor(void*);
void pixel_cleardepth(void*);
//
int copypath(void*, void*);



#define FEATURE_LOG 1
#define FEATURE_AUXLINE 2
struct unidata{
	mat4 mat;
	vec4 vec;
};
struct privdata{
	struct halfrel* self;
	struct halfrel* peer;

	u64 drawtype;
	u64 evtype;
	u32 feature;
	u32 linecolor;

	//common
	mat4 world2view;	//world to view
	struct unidata uni;

	//gl: forward, deferred, perpixellinklist
	struct gl41data gl41cam;

	u8 gbuf_vs[128];
	u8 gbuf_fs[128];
	struct gl41data gl41gbuf;

	u8 ppll_vs[128];
	u8 ppll_fs[128];
	struct gl41data gl41ppll;
};




void fisheyecam_move(vec3 dst, vec3 src, float t)
{
	dst[0] += src[0] * t;
	dst[1] += src[1] * t;
	dst[2] += src[2] * t;
}
void fisheyecam_rotate(vec3 a, vec3 b, vec3 axis, float angle)
{
	quaternion_operation(a, axis, angle);
	quaternion_operation(b, axis, angle);
}
static int fisheyecam_event_obb_char(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	struct event* ev, int len)
{
	struct fstyle* obb = &geom->fshape;
	switch(ev->why){
		case 'a':fisheyecam_move(obb->vc, obb->vr,-1.0);break;
		case 'd':fisheyecam_move(obb->vc, obb->vr, 1.0);break;
		case 's':fisheyecam_move(obb->vc, obb->vf,-1.0);break;
		case 'w':fisheyecam_move(obb->vc, obb->vf, 1.0);break;
		case 'f':fisheyecam_move(obb->vc, obb->vt,-1.0);break;
		case 'r':fisheyecam_move(obb->vc, obb->vt, 1.0);break;

		case 'j':fisheyecam_rotate(obb->vr, obb->vf, obb->vt, 0.05);break;
		case 'l':fisheyecam_rotate(obb->vr, obb->vf, obb->vt,-0.05);break;
		case 'i':fisheyecam_rotate(obb->vf, obb->vt, obb->vr, 0.05);break;
		case 'k':fisheyecam_rotate(obb->vf, obb->vt, obb->vr,-0.05);break;
		case 'u':fisheyecam_rotate(obb->vr, obb->vt, obb->vf,-0.05);break;
		case 'o':fisheyecam_rotate(obb->vr, obb->vt, obb->vf, 0.05);break;
	}
	return 0;
}
static int fisheyecam_event_obb_point(
	_obj* act, struct style* part,
	_obj* wor, struct style* geom,
	_obj* wnd, struct style* area,
	struct event* ev, int len)
{
	int type = (ev->what>>8)&0xff;
	if(0x2d == type){
		act->whdf.iw0 = 0;
		return 0;
	}

	short* t = (void*)ev;
	vec3 up = {0.0, 0.0, 1.0};
	struct fstyle* obb = &geom->fshape;
	if(0x2b == type){
		act->whdf.ix0 = act->whdf.ixn = t[0];
		act->whdf.iy0 = act->whdf.iyn = t[1];
		act->whdf.iw0 = 1;
		return 0;
	}
	if(0x40 == type){
		if(0 == act->whdf.iw0)return 0;
		quaternion_operation(obb->vf, obb->vr, (t[1] - act->whdf.iyn)/100.0);
		quaternion_operation(obb->vt, obb->vr, (t[1] - act->whdf.iyn)/100.0);

		quaternion_operation(obb->vr, up, (t[0] - act->whdf.ixn)/100.0);
		quaternion_operation(obb->vf, up, (t[0] - act->whdf.ixn)/100.0);
		quaternion_operation(obb->vt, up, (t[0] - act->whdf.ixn)/100.0);

		//fisheyecam_rotate(obb->vr, obb->vf, up, (t[0] - act->whdf.ixn)/100.0);
		//fisheyecam_rotate(obb->vf, obb->vt, obb->vr, (t[1] - act->whdf.iyn)/100.0);
		act->whdf.ixn = t[0];
		act->whdf.iyn = t[1];
	}
	return 0;
}
static int fisheyecam_event_obb(
	_obj* act, struct style* part,
	_obj* wor, struct style* geom,
	_obj* wnd, struct style* area,
	struct event* ev, int len)
{
	int t = (ev->what&0xff);
	if(_char_ == ev->what)return fisheyecam_event_obb_char(act,part, wor,geom, ev,len);
	else if('p' == t)return fisheyecam_event_obb_point(act,part, wor,geom, wnd,area, ev,len);
	else if('t' == t)return fisheyecam_event_obb_point(act,part, wor,geom, wnd,area, ev,len);

	return 0;
}
static int fisheyecam_event_frus(
	_obj* act, struct style* part,
	_obj* wor, struct style* geom,
	_obj* wnd, struct style* area,
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




void fisheyecam_ratio(
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
void fisheyecam_shape2frustum(struct fstyle* s, struct fstyle* d)
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
static void printvec4(float* s)
{
	logtoall("%f, %f, %f, %f\n", s[0], s[1], s[2], s[3]);
}
static void printstyle(struct fstyle* sty)
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
static void printmat4(float* f)
{
	printvec4(&f[0]);
	printvec4(&f[4]);
	printvec4(&f[8]);
	printvec4(&f[12]);
}
static void fisheyecam_frus2wvp(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom)
{
	struct fstyle* frus = &geom->frus;
	struct privdata* own = act->priv_ptr;
    world2view_righthand_transpose(own->world2view, frus);
}




static void fisheyecam_gl41_cam(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	struct privdata* own = act->priv_ptr;

	struct gl41data* data = &own->gl41cam;
    data->dst.camtype = _fish_eye_proj_;

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammv_";
	data->dst.arg[0].data = own->world2view;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = frus->vc;
	wnd->gl41list.world[0].camera[0] = data;
}
static int fisheyecam_gl41_mesh(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* none)
{
	struct privdata* own = act->priv_ptr;

	if(own->feature & FEATURE_AUXLINE){
		//logtoall("FEATURE_AUXLINE\n");
		fisheyecam_shape2frustum(&geom->fshape, &geom->frustum);

		//frustum
		gl41frustum_color(ctx, &geom->frus, own->linecolor);

		//ray from eye to far
		gl41line(ctx, own->linecolor, geom->frus.vc, &act->whdf.fx0);
	}
	return 0;
}




static int fisheyecam_byworld_bycam_bywnd_read(_obj* ent,void* slot, _syn* stack,int sp)
{
//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam,gl41 -> wor,geom		//the camera taking photo
//[-2,-1]: wor,geom -> ent,gl41		//the entity being taken
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	switch(wnd->vfmt){
	case _tui_:
	case _rgba_:
		return 0;
	case _gl41list_:
		//fisheyecam_gl41_mesh(ent,slot, wor,geom, wnd,area);
        break;
	}
	return 0;
}




//world - camera - window
static int fisheyecam_visitworld(
_obj* wor,struct style* geom,
_obj* ent,void* slot,
_obj* wnd,struct style* area,
_syn* stack,int sp)
{
	//logtoall("%s\n", __func__);
	struct privdata* own = ent->priv_ptr;
	switch(wnd->vfmt){
	case _gl41list_:
		//logtoall("%s case gl41list\n", __func__);
		//clear all
		gl41data_before(wnd);
		//render data
		gl41data_taking(wor,0, stack,sp+2, 0,'v', 0,0);
		//enq++
		gl41data_after(wnd);

		//camera matrix
		fisheyecam_ratio(wor, geom, wnd, area);
		fisheyecam_shape2frustum(&geom->fshape, &geom->frustum);
		fisheyecam_frus2wvp(ent,slot, wor,geom);
		fisheyecam_gl41_cam(ent,slot, wor,geom, wnd,area);
		break;
	}
	return 0;
}




static int fisheyecam_bywnd_read(_obj* ent,void* slot, _obj* wnd,struct style* area, _syn* stack,int sp)
{
	//logtoall("@%s: stack=%p,sp=%d\n",__func__,stack,sp);
	struct privdata* own = ent->priv_ptr;
	struct halfrel* self = own->self;
	struct halfrel* peer = own->peer;
	//logtoall("ent=%p,own=%p,self=%p,peer=%p\n",ent,own,self,peer);
	stack[sp+0].pchip = self->pchip;
	stack[sp+0].pfoot = self->pfoot;
	stack[sp+0].foottype = self->foottype;
	stack[sp+1].pchip = peer->pchip;
	stack[sp+1].pfoot = peer->pfoot;
	stack[sp+1].foottype = peer->foottype;

//[+0,+1]: cam,towr -> wor,geom
	_obj* wor = stack[sp+1].pchip;
	struct style* geom = stack[sp+1].pfoot;
	fisheyecam_visitworld(wor,geom, ent,slot, wnd,area, stack,sp);
	return 0;
//logtoall("@fisheyecam_bywnd_read.end\n");
}
static int fisheyecam_bywnd_write(_obj* ent,void* ef, _obj* wnd,void* wf, struct event* ev)
{
//find world from camera
	struct privdata* own = ent->priv_ptr;
	struct halfrel* rel = own->peer;

	_obj* wor = rel->pchip;
	struct style* geom = rel->pfoot;
	switch(own->evtype){
	case 'f':return fisheyecam_event_frus(ent,0, wor,geom, wnd,wf, ev,0);break;
	default:return fisheyecam_event_obb(ent,0, wor,geom, wnd,wf, ev,0);break;
	}
	return 0;
}




static int fisheyecam_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@fisheyecam_read begin\n");
	if(0 == stack)return 0;

	_obj* caller = stack[sp-2].pchip;
	if(0 == caller)return 0;
	struct style* area = stack[sp-2].pfoot;

	//logtoall("callertype=%.4s\n", &caller->type);
	switch(caller->type){
	case _wnd_:
	case _render_:
		fisheyecam_bywnd_read(ent,foot, caller, area, stack,sp);
		break;
	default:
		fisheyecam_byworld_bycam_bywnd_read(ent,foot, stack,sp);
		break;
	}
	//logtoall("@fisheyecam_read end\n");
	return 0;
}
static int fisheyecam_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	struct privdata* own = ent->priv_ptr;
	struct event* ev = buf;

	_obj* selfnode = ent; //stack[sp-1].pchip;
	void* selffoot = stack[sp-1].pfoot;
	_obj* callnode = stack[sp-2].pchip;
	void* callfoot = stack[sp-2].pfoot;
	return fisheyecam_bywnd_write(selfnode,selffoot, callnode,callfoot, buf);
}
static void fisheyecam_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void fisheyecam_attach(struct halfrel* self, struct halfrel* peer)
{
    logtoall("@fisheyecam_attach\n");

	_obj* ent = self->pchip;
	struct privdata* own = ent->priv_ptr;

	_obj* that = peer->pchip;
	logtoall("thattype=%.4s\n", &that->type);
	switch(that->type){
	case _virtual_:
	case _scene3d_:
		own->self = self;
		own->peer = peer;
		return;
	}
}




static void fisheyecam_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
}
static void fisheyecam_modify(_obj* act)
{
}
static void fisheyecam_delete(_obj* act)
{
}
static void fisheyecam_create(_obj* act, void* arg, int argc, u8** argv)
{
    logtoall("@fisheyecam_create\n");
	struct privdata* own = act->priv_ptr = memoryalloc(0x2000, 0);
}




void fisheyecam_register(_obj* p)
{
	p->type = hex64('f', 'i', 's', 'h', 'e', 'y', 'e', 0);
	p->vfmt = _orig_;

	p->oncreate = (void*)fisheyecam_create;
	p->ondelete = (void*)fisheyecam_delete;
	p->onreader = (void*)fisheyecam_search;
	p->onwriter = (void*)fisheyecam_modify;

	p->onattach = (void*)fisheyecam_attach;
	p->ondetach = (void*)fisheyecam_detach;
	p->ontaking = (void*)fisheyecam_taking;
	p->ongiving = (void*)fisheyecam_giving;
}
