#include "libuser.h"
void fixmatrix(float* m, struct fstyle* sty);




static int vrglass_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{/*
	float y;
	vec3 tc,tr,tf,tu;
	tr[0] = win->width/2;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tu[0] = 0.0;
	tu[1] = 0.0;
	tu[2] = win->height/2;

	tc[0] = 0.0;
	tc[1] = 0.0;
	tc[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1000.0;
	tf[2] = 0.0;
	carveline_prism4(win, 0xff0000, tc, tr, tf, tu);
	carvefrustum(win, &win->camera);

	for(y=-1000.0;y<1001.0;y+=500.0)
	{
		tc[0] = 0.0;
		tc[1] = y;
		tc[2] = 0.0;
		carveline_rect(win, 0xff0000, tc, tr, tu);
	}
*/
	//float time = (timeread() % 10000000) / 10000000.0;
	//act->camera.vq[0] = 1000 * cosine(tau * time);
	//act->camera.vq[1] = 1000 * sine(tau * time);
	return 0;
}/*
static int vrglass_event111(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	int id;
	int x0,y0,x1,y1;
	short* t;
	float* vc = act->camera.vc;

	if(_kbd_ == ev->what)
	{
		if(0x4b == ev->why)vc[0] -= 10;
		else if(0x4d == ev->why)vc[0] += 10;
		else if(0x50 == ev->why)vc[2] -= 10;
		else if(0x48 == ev->why)vc[2] += 10;
	}
	else if(0x2b70 == ev->what)
	{
		id = (ev->why)>>48;
		if('f' == id)vc[1] += 10;
		if('b' == id)vc[1] -= 10;
	}
	else if(0x4070 == ev->what)
	{
		id = (ev->why)>>48;
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;
		if(0 == win->input[id].z0)return 0;

		x0 = win->input[id].xn;
		y0 = win->input[id].yn;
		x1 = (ev->why)&0xffff;
		y1 = ((ev->why)>>16)&0xffff;

		vc[0] += x1-x0;
		vc[2] -= y1-y0;
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		//printmemory(t, 16);
		//say("%d,%d\n", t[0], t[1]);
		if(t[3] & joyl_left)		//x-
		{
			vc[0] -= 10;
		}
		if(t[3] & joyl_right)		//x+
		{
			vc[0] += 10;
		}
		if(t[3] & joyl_down)		//y-
		{
			vc[2] -= 10;
		}
		if(t[3] & joyl_up)			//y+
		{
			vc[2] += 10;
		}
		if(t[3] & joyl_trigger)		//z-
		{
			vc[1] -= 10.0;
		}
		if(t[3] & joyl_bumper)		//z+
		{
			vc[1] += 10.0;
		}
		if(t[3] & joyl_thumb)		//w-
		{
			vc[0] = 0.0;
			vc[1] = -2000.0;
			vc[2] = 0.0;
		}
		if(t[3] & joyl_select)		//w+
		{
		}

		x0 = t[0];
		y0 = t[1];
		if((x0 < -4096) | (x0 > 4096) | (y0 < -4096) | (y0 > 4096))
		{
			vc[0] += x0/1000.0;
			vc[2] += y0/1000.0;
		}
	}
	//say("%f,%f,%f\n",vc[0], vc[1], vc[2]);

	win->camera.vc[0] = vc[0];
	win->camera.vc[1] = vc[1];
	win->camera.vc[2] = vc[2];

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = -1000.0 - win->camera.vc[1];
	win->camera.vn[2] = 0.0;

	win->camera.vl[0] = -win->width/2 - win->camera.vc[0];
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = win->width/2 - win->camera.vc[0];
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] = 0.0;
	win->camera.vb[2] = -win->height/2 - win->camera.vc[2];

	win->camera.vt[0] = 0.0;
	win->camera.vt[1] = 0.0;
	win->camera.vt[2] = win->height/2 - win->camera.vc[2];
	return 1;
}*/
static int vrglass_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	say("vrglass_event@%llx:%x,%x\n", act, ev->why, ev->what);

	if(joy_left == (ev->what&joy_mask)){
		t = (void*)ev;
		if(t[3] & joyl_left   )act->camera.vq[0] -= 10.0;
		if(t[3] & joyl_right  )act->camera.vq[0] += 10.0;
		if(t[3] & joyl_down   )act->camera.vq[2] += 10.0;
		if(t[3] & joyl_up     )act->camera.vq[2] -= 10.0;
		if(t[3] & joyl_trigger)act->camera.vq[1] -= 10.0;
		if(t[3] & joyl_bumper )act->camera.vq[1] += 10.0;
	}
	if(_char_ == ev->what){
		if('a' == ev->why)act->camera.vq[0] -= 10.0;
		if('d' == ev->why)act->camera.vq[0] += 10.0;
		if('s' == ev->why)act->camera.vq[2] += 10.0;
		if('w' == ev->why)act->camera.vq[2] -= 10.0;
	}
	return 1;
}




void vrglass_sty2cam(struct fstyle* d, struct fstyle* s)
{
	float x,y,z,n;
	d->vc[0] = s->vc[0];
	d->vc[1] = s->vc[1];
	d->vc[2] = s->vc[2];


	x = s->vr[0];	y = s->vr[1];	z = s->vr[2];
	n = 1.0 / squareroot(x*x + y*y + z*z);
	x *= n;		y*= n;		z*= n;
	d->vr[0] = x;	d->vr[1] = y;	d->vr[2] = z;	d->vr[3] = 1000 - d->vq[0];
	d->vl[0] = -x;	d->vl[1] = -y;	d->vl[2] = -z;	d->vl[3] = -1000 - d->vq[0];
	d->vc[0] += x * d->vq[0];
	d->vc[1] += y * d->vq[0];
	d->vc[2] += z * d->vq[0];


	x = s->vt[0];	y = s->vt[1];	z = s->vt[2];
	n = 1.0 / squareroot(x*x + y*y + z*z);
	x *= n;		y*= n;		z*= n;
	d->vt[0] = x;	d->vt[1] = y;	d->vt[2] = z;	d->vt[3] = 1000 - d->vq[1];
	d->vb[0] = -x;	d->vb[1] = -y;	d->vb[2] = -z;	d->vb[3] = -1000 - d->vq[1];
	d->vc[0] += x * d->vq[1];
	d->vc[1] += y * d->vq[1];
	d->vc[2] += z * d->vq[1];


	x = s->vf[0];	y = s->vf[1];	z = s->vf[2];
	n = 1.0 / squareroot(x*x + y*y + z*z);
	x *= n;		y*= n;		z*= n;
	d->vn[0] = x;	d->vn[1] = y;	d->vn[2] = z;	d->vn[3] = d->vq[2];
	d->vf[0] = x;	d->vf[1] = y;	d->vf[2] = z;	d->vf[3] = 1e20;
	d->vc[0] -= x * d->vq[2];
	d->vc[1] -= y * d->vq[2];
	d->vc[2] -= z * d->vq[2];
}
static void vrglass_matrix(
	struct actor* act, struct style* pin,
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
	vrglass_sty2cam(&act->camera, s);
	fixmatrix(m, &act->camera);
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




static void vrglass_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//if(_cam_ == self->flag)vrglass_matrix(act, pin, buf, len);
	//else vrglass_draw(act, pin, win, sty);
}
static int vrglass_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return 0;//vrglass_event(act, pin, win, sty, ev, 0);
}
static void vrglass_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void vrglass_start(struct halfrel* self, struct halfrel* peer)
{
}




static void vrglass_search(struct actor* act)
{
}
static void vrglass_modify(struct actor* act)
{
}
static void vrglass_delete(struct actor* act)
{
}
static void vrglass_create(struct actor* act, void* str)
{/*
	float* vc = act->camera.vc;
	float* vn = act->camera.vn;

	vc[0] = 0.0;
	vc[1] = -2000.0;
	vc[2] = 200.0;

	vn[0] = 0.0;
	vn[1] = -1000.0 - vc[1];
	vn[2] = 0.0;*/

	act->camera.vq[0] = 100.0;
	act->camera.vq[1] = 100.0;
	act->camera.vq[2] = 1000.0;
	act->buf = memorycreate(64, 0);
}




void vrglass_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('v', 'r', 'g', 'l', 'a', 's', 's', 0);

	p->oncreate = (void*)vrglass_create;
	p->ondelete = (void*)vrglass_delete;
	p->onsearch = (void*)vrglass_search;
	p->onmodify = (void*)vrglass_modify;

	p->onstart = (void*)vrglass_start;
	p->onstop  = (void*)vrglass_stop;
	p->onread  = (void*)vrglass_read;
	p->onwrite = (void*)vrglass_write;
}
