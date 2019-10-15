#include "libuser.h"
void fixmatrix(float* m, struct fstyle* sty);




/*
static int rtscam_event11111(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	//say("%llx,%llx\n", ev->why, ev->what);
	int id;
	int x0,y0,x1,y1;
	short* t;
	if(_kbd_ == ev->what)
	{
		if(0x4b == ev->why)win->camera.vc[0] -= 10;
		else if(0x4d == ev->why)win->camera.vc[0] += 10;
		else if(0x50 == ev->why)win->camera.vc[1] -= 10;
		else if(0x48 == ev->why)win->camera.vc[1] += 10;
	}
	else if(0x4070 == ev->what)
	{
		id = (ev->why)>>48;
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;
		//if(0 == win->input[id].z0)return;

		//x0 = win->input[id].xn;
		//y0 = win->input[id].yn;
		x1 = (ev->why)&0xffff;
		y1 = ((ev->why)>>16)&0xffff;
		//win->camera.vc[0] += x1-x0;
		//win->camera.vc[1] -= y1-y0;

		if(x1 < 16)win->camera.vc[0] -= 10;
		if(x1 > win->width-16)win->camera.vc[0] += 10;
		if(y1 < 16)win->camera.vc[1] += 10;
		if(y1 > win->width-16)win->camera.vc[1] -= 10;
	}

	return 1;
}*/
static int rtscam_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	struct fstyle* s;
	struct relation* rel;
	say("rtscam_event@%llx:%x,%x\n", act, ev->why, ev->what);

	//
	rel = act->irel0;
	while(1){
		if(0 == rel)return 0;
		if(hex32('g','e','o','m') == rel->dstflag){
			s = (void*)(rel->srcfoot);
			break;
		}
		rel = samedstnextsrc(rel);
	}
	if(0 == s)return 0;

	//
	if(joy_left == (ev->what&joy_mask)){
		t = (void*)ev;
		if(t[3] & joyl_left   )s->vc[0] -= 10.0;
		if(t[3] & joyl_right  )s->vc[0] += 10.0;
		if(t[3] & joyl_down   )s->vc[1] -= 10.0;
		if(t[3] & joyl_up     )s->vc[1] += 10.0;
		if(t[3] & joyl_trigger)s->vc[2] -= 10.0;
		if(t[3] & joyl_bumper )s->vc[2] += 10.0;
	}

	if(_char_ == ev->what){
		if('a' == ev->why)s->vc[0] -= 10.0;
		if('d' == ev->why)s->vc[0] += 10.0;
		if('s' == ev->why)s->vc[1] -= 10.0;
		if('w' == ev->why)s->vc[1] += 10.0;
	}
	return 1;
}




void rtscam_frustum(struct fstyle* d, struct fstyle* s)
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
	//d->vr[3] = 0.7;
	d->vl[0] = -x / n;
	d->vl[1] = -y / n;
	d->vl[2] = -z / n;
	//d->vl[3] = -0.7;


	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	//d->vt[3] = 0.7;
	d->vb[0] = -x / n;
	d->vb[1] = -y / n;
	d->vb[2] = -z / n;
	//d->vb[3] = -0.7;


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
static void rtscam_matrix(
	struct actor* act, struct style* frustum,
	struct actor* win, struct style* wingeom,
	u8* buf, int len)
{/*
	struct relation* rel;
	struct actor* r;
	struct fstyle* s;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		if(hex32('g','e','o','m') == rel->dstflag){
			s = (void*)(rel->srcfoot);
			r = (void*)(rel->srcchip);
			break;
		}
		rel = samedstnextsrc(rel);
	}
	if(0 == s)return;


	float* m = act->buf;
	rtscam_frustum(&frustum->f, s);
	fixmatrix(m, &frustum->f);
	mat4_transpose((void*)m);
	//printmat4(m);


	u64* p = (void*)buf;
	struct glsrc* src = (void*)(buf+0x20);

	p[0] = (u64)src;
	p[1] = (u64)r->gl_light;
	p[2] = (u64)r->gl_solid;
	p[3] = (u64)r->gl_opaque;


	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "cammvp";
	src->arg_data[0] = m;

	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "camxyz";
	src->arg_data[1] = act->camera.vc;
*/
}




static void rtscam_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
/*
	switch(self->flag){
		case _cam_:rtscam_matrix(act, pin, win, sty, buf, len);break;
	}*/
}
static int rtscam_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return 0;//rtscam_event(act, pin, win, sty, ev, 0);
}
static void rtscam_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void rtscam_start(struct halfrel* self, struct halfrel* peer)
{
    say("@rtscam_start\n");
}




static void rtscam_search(struct actor* act)
{
}
static void rtscam_modify(struct actor* act)
{
}
static void rtscam_delete(struct actor* act)
{
}
static void rtscam_create(struct actor* act, void* addr)
{
	act->buf = memorycreate(64, 0);
}




void rtscam_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 't', 's', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)rtscam_create;
	p->ondelete = (void*)rtscam_delete;
	p->onsearch = (void*)rtscam_search;
	p->onmodify = (void*)rtscam_modify;

	p->onstart = (void*)rtscam_start;
	p->onstop  = (void*)rtscam_stop;
	p->onread  = (void*)rtscam_read;
	p->onwrite = (void*)rtscam_write;
}
