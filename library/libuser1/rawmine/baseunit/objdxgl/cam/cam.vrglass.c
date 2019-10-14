#include "libuser.h"
void fixmatrix(float* m, struct fstyle* sty);




static void vrglass_search(struct actor* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct actor* world;
	struct fstyle* obb = 0;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
static void vrglass_modify(struct actor* act)
{
}
static void vrglass_delete(struct actor* act)
{
}
static void vrglass_create(struct actor* act, void* str)
{
	act->buf = memorycreate(0x1000, 0);
}




static int vrglass_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float y;
	vec3 tc;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vt = sty->f.vt;

	for(y=0.0;y<1.01;y+=0.1)
	{
		tc[0] = vc[0] + vf[0]*y;
		tc[1] = vc[1] + vf[1]*y;
		tc[2] = vc[2] + vf[2]*y;
		carveline_rect(win, 0xff0000, tc, vr, vt);
	}
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
	struct actor* act, struct fstyle* pin,
	struct actor* win, struct fstyle* sty,
	struct event* ev, int len)
{
	short* t;
	struct fstyle* obb;
	struct halfrel* self;
	struct halfrel* peer;
	//say("vrglass_event@%llx:%x,%x\n", act, ev->why, ev->what);

	vrglass_search(act, 0, &self, &peer);
	obb = peer->pfoot;

	if(joy_left == (ev->what&joy_mask)){
		t = (void*)ev;
		if(t[3] & joyl_left   )obb->vq[0] -= 10.0;
		if(t[3] & joyl_right  )obb->vq[0] += 10.0;
		if(t[3] & joyl_down   )obb->vq[2] += 10.0;
		if(t[3] & joyl_up     )obb->vq[2] -= 10.0;
		if(t[3] & joyl_trigger)obb->vq[1] -= 10.0;
		if(t[3] & joyl_bumper )obb->vq[1] += 10.0;
	}
	if(_char_ == ev->what){
		switch(ev->why){
			case 'a':obb->vq[0] -= 10.0;break;
			case 'd':obb->vq[0] += 10.0;break;
			case 's':obb->vq[1] -= 10.0;break;
			case 'w':obb->vq[1] += 10.0;break;
			case 'q':obb->vq[2] -= 10.0;break;
			case 'e':obb->vq[2] += 10.0;break;
		}
		say("%f,%f,%f\n", obb->vq[0], obb->vq[1], obb->vq[2]);
	}
	return 1;
}




void vrglass_frustum(struct fstyle* frus, struct fstyle* obb)
{
	float x,y,z,n;
	//say("@vrglass_frustum\n");

	frus->vc[0] = obb->vq[0];
	frus->vc[1] = obb->vq[1];
	frus->vc[2] = obb->vq[2];


	//l,r
	x = obb->vr[0];
	y = obb->vr[1];
	z = obb->vr[2];
	n = squareroot(x*x + y*y + z*z);
	x /= n;
	y /= n;
	z /= n;

	frus->vl[0] =-x;
	frus->vl[1] =-y;
	frus->vl[2] =-z;
	frus->vl[3] = 
	  (obb->vc[0] - obb->vr[0] - obb->vq[0])*x
	+ (obb->vc[1] - obb->vr[1] - obb->vq[1])*y
	+ (obb->vc[2] - obb->vr[2] - obb->vq[2])*z;

	frus->vr[0] = x;
	frus->vr[1] = y;
	frus->vr[2] = z;
	frus->vr[3] = 
	  (obb->vc[0] + obb->vr[0] - obb->vq[0])*x
	+ (obb->vc[1] + obb->vr[1] - obb->vq[1])*y
	+ (obb->vc[2] + obb->vr[2] - obb->vq[2])*z;


	//b,t
	x = obb->vt[0];
	y = obb->vt[1];
	z = obb->vt[2];
	n = squareroot(x*x + y*y + z*z);
	x /= n;
	y /= n;
	z /= n;

	frus->vb[0] =-x;
	frus->vb[1] =-y;
	frus->vb[2] =-z;
	frus->vb[3] = 
	  (obb->vc[0] - obb->vt[0] - obb->vq[0])*x
	+ (obb->vc[1] - obb->vt[1] - obb->vq[1])*y
	+ (obb->vc[2] - obb->vt[2] - obb->vq[2])*z;

	frus->vt[0] = x;
	frus->vt[1] = y;
	frus->vt[2] = z;
	frus->vt[3] = 
	  (obb->vc[0] + obb->vt[0] - obb->vq[0])*x
	+ (obb->vc[1] + obb->vt[1] - obb->vq[1])*y
	+ (obb->vc[2] + obb->vt[2] - obb->vq[2])*z;


	//n,f
	x = obb->vf[0];
	y = obb->vf[1];
	z = obb->vf[2];
	n = squareroot(x*x + y*y + z*z);
	x /= n;
	y /= n;
	z /= n;

	frus->vn[0] = x;
	frus->vn[1] = y;
	frus->vn[2] = z;
	frus->vn[3] = 
	  (obb->vc[0] - obb->vq[0])*x
	+ (obb->vc[1] - obb->vq[1])*y
	+ (obb->vc[2] - obb->vq[2])*z;

	frus->vf[0] = x;
	frus->vf[1] = y;
	frus->vf[2] = z;
	frus->vf[3] = 1e10;
/*	say("portal_frustum: (%f,%f), (%f,%f), (%f,%f)\n",
		frus->vn[3], frus->vf[3], frus->vl[3], frus->vr[3], frus->vb[3], frus->vt[3]);*/
}
static void vrglass_matrix(
	struct actor* act, struct fstyle* frus,
	struct actor* ctx, struct fstyle* area)
{
	struct halfrel* self;
	struct halfrel* peer;
	struct fstyle* obb;
	float dx,dy;
	//say("@vrglass_matrix\n");

	dx = area->vq[0] * ctx->fbwidth;
	dy = area->vq[1] * ctx->fbheight;
	frus->vb[3] = frus->vl[3] * dy / dx;
	frus->vt[3] = frus->vr[3] * dy / dx;
	vrglass_search(act, 0, &self, &peer);

	obb = peer->pfoot;
	vrglass_frustum(frus, obb);

	float* mat = act->buf;
	fixmatrix((void*)mat, frus);
	mat4_transpose((void*)mat);
	//printmat4(mat);

	struct glsrc* src = ctx->gl_camera;
	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "cammvp";
	src->arg_data[0] = mat;

	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "camxyz";
	src->arg_data[1] = obb->vq;
}




static void vrglass_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	//say("@vrglass_read\n");
	if(ctx){
		switch(ctx->type){
			case _gl41data_:vrglass_draw_vbo(act,pin,ctx,sty);
		}
	}
	else{
		switch(win->type){
			case _gl41view_:
			case _gl41wnd0_:vrglass_matrix(act, &pin->fs, win, &sty->fs);
		}
	}
}
static int vrglass_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	vrglass_event(act, &pin->fs, win, &sty->fs, ev, 0);
	return 0;
}
static void vrglass_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void vrglass_start(struct halfrel* self, struct halfrel* peer)
{
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
