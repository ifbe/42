#include "libuser.h"
#define MATBUF buf0
#define CAMBUF buf1
#define EVTYPE vfmt
#define EVSEND 666666
void style2matrix2_transpose(struct fstyle* frus, mat4 v_, mat4 vp);
//
void gl41data_before(void*);
void gl41data_after(void*);
int gl41data_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




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
	struct relation* rel;
	struct entity* world;
	struct fstyle* obb = 0;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if( (_virtual_ == world->type) | (_scene3d_ == world->type) ){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
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
	act->EVTYPE = 0;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "script:", 7)){
			if('f' == argv[j][7]){
				act->EVTYPE = 'f';	//frus
			}
		}
	}

	act->fx0 = 0.0;
	act->fy0 = 0.0;
	act->fz0 = 0.0;

	//matrix
	act->MATBUF = memorycreate(64*2, 0);
	act->CAMBUF = memorycreate(0x200, 0);
}




static int freecam_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* none)
{
	//frustum
	carvefrustum(ctx, &geom->frus);

	//ray from eye to far
	gl41line(ctx, 0, geom->frus.vc, &act->fx0);
	return 0;
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
			freecam_rotate(obb->vr, obb->vf, obb->vt, (t[0] - act->ixn)/100.0);
			freecam_rotate(obb->vf, obb->vt, obb->vr, (t[1] - act->iyn)/100.0);
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
		nx = 100.0/vec3_getlen(obb->vr);
		ny = 100.0/vec3_getlen(obb->vf);
		nz = 100.0/vec3_getlen(obb->vt);
		switch(ev->why){
			case 'a':freecam_move(obb->vc, obb->vr,-nx);break;
			case 'd':freecam_move(obb->vc, obb->vr, nx);break;
			case 's':freecam_move(obb->vc, obb->vf,-ny);break;
			case 'w':freecam_move(obb->vc, obb->vf, ny);break;
			case 'q':freecam_move(obb->vc, obb->vt,-nz);break;
			case 'e':freecam_move(obb->vc, obb->vt, nz);break;

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




void freecam_ratio(
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct fstyle* rect = &area->fshape;
	struct fstyle* frus = &geom->frus;
	float dx = rect->vq[0] * wnd->fbwidth;
	float dy = rect->vq[1] * wnd->fbheight;
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
static void freecam_matrix(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom)
{
	struct fstyle* frus = &geom->frus;
	void* vp = act->MATBUF;
	void* v_ = vp + 4*4*4;
	style2matrix2_transpose(frus, v_, vp);

	//matproj_transpose(cammvp, frus);
	//fixview_transpose(cammv_, frus);
	//printmat4(m);
}
static void freecam_camera(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	struct glsrc* src = act->CAMBUF;
	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = act->MATBUF;
	src->arg[1].fmt = 'm';
	src->arg[1].name = "cammv_";
	src->arg[1].data = act->MATBUF + 4*4*4;
	src->arg[2].fmt = 'v';
	src->arg[2].name = "camxyz";
	src->arg[2].data = frus->vc;
	wnd->gl_camera[0] = act->CAMBUF;
}




static int freecam_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam,gl41 -> wor,geom		//the camera taking photo
//[-2,-1]: wor,geom -> ent,gl41		//the entity being taken
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		freecam_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	return 0;
}
static int freecam_read_bywnd(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//find world from camera
	struct halfrel* tmp[2];
	freecam_search(ent, 0, &tmp[0], &tmp[1]);
	stack[sp+0].pchip = tmp[0]->pchip;
	stack[sp+0].pfoot = tmp[0]->pfoot;
	stack[sp+0].flag = tmp[0]->flag;
	stack[sp+1].pchip = tmp[1]->pchip;
	stack[sp+1].pfoot = tmp[1]->pfoot;
	stack[sp+1].flag = tmp[1]->flag;

//[-2,-1]: wnd,area -> cam,togl
//[+0,+1]: cam,towr -> wor,geom
	struct style* slot;
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* geom;
	slot = stack[sp-1].pfoot;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	wor = stack[sp+1].pchip;geom = stack[sp+1].pfoot;
	if('v' == key){
		gl41data_before(wnd);

		freecam_ratio(wor, geom, wnd, area);
		freecam_shape2frustum(&geom->fshape, &geom->frustum);
		freecam_matrix(ent,slot, wor,geom);
		freecam_camera(ent,slot, wor,geom, wnd,area);

		gl41data_read(wor,0, stack,sp+2, 0,'v', buf,len);

		gl41data_after(wnd);
		return -1;
	}
	if('?' == key){
		gl41data_read(wor,0, stack,sp+2, 0,'?', buf,len);
		return 0;
	}
	return 0;
//say("@freecam_read_bywnd.end\n");
}
static int freecam_write_bywnd(_ent* ent,struct event* ev)
{
//find world from camera
	struct halfrel* tmp[2];
	freecam_search(ent, 0, &tmp[0], &tmp[1]);

	struct entity* wor = tmp[1]->pchip;
	struct style* geom = tmp[1]->pfoot;
	switch(ent->EVTYPE){
	case 'f':return freecam_event_frus(ent,0, wor,geom, ev,0);break;
	case 0:return freecam_event_obb(ent,0, wor,geom, ev,0);break;
	}
	return 0;
}




static int freecam_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(sp < 2)return 0;
	struct entity* sup = stack[sp-2].pchip;
	if(0 == sup)return 0;

	switch(sup->fmt){
	case _gl41fbog_:
	case _gl41wnd0_:
	case _full_:
	case _wnd_:return freecam_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);
	default:return freecam_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
	}
	return 0;
}
static int freecam_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(EVSEND != ent->EVTYPE)freecam_write_bywnd(ent, buf);
	else relationwrite(ent,_evto_, stack,sp, arg,key, buf,len);
	return 0;
}
static void freecam_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void freecam_linkup(struct halfrel* self, struct halfrel* peer)
{
    say("@freecam_linkup\n");
	if(_evto_ == self->flag){
		struct entity* cam = self->pchip;
		cam->EVTYPE = EVSEND;
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
	p->onread  = (void*)freecam_read;
	p->onwrite = (void*)freecam_write;
}
