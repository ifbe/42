#include "libuser.h"
#define _in_ hex32('i','n', 0, 0)
#define MATBUF buf0
#define CAMBUF buf1
void fixmatrix_transpose(float* m, struct fstyle* sty);
int gl41data_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len);




static void vrglass_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
}
static void vrglass_modify(struct entity* act)
{
}
static void vrglass_delete(struct entity* act)
{
}
static void vrglass_create(struct entity* act, void* str)
{
	act->MATBUF = memorycreate(64*2, 0);
	act->CAMBUF = memorycreate(0x200, 0);
}




static int vrglass_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float y;
	vec3 tc;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	for(y=0.0;y<1.01;y+=0.1)
	{
		tc[0] = vc[0] + vf[0]*y;
		tc[1] = vc[1] + vf[1]*y;
		tc[2] = vc[2] + vf[2]*y;
		gl41line_rect(wnd, 0xff0000, tc, vr, vt);
	}
	return 0;
}/*
static int vrglass_event111(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
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
static int vrglass_event(struct entity* act, struct fstyle* pin, struct event* ev, int len)
{
	short* t;
	struct fstyle* obb;
	struct halfrel* self;
	struct halfrel* peer;
	//say("vrglass_event@%llx:%x,%x\n", act, ev->why, ev->what);

	relationsearch(act, _in_, &self, &peer);
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




void vrglass_frustum(struct fstyle* frus, struct fstyle* plane, vec3 eye)
{
	float x,y,z,n;
	//say("@vrglass_frus:%llx,%llx,%llx,%f,%f,%f\n",frus,plane,eye,eye[0],eye[1],eye[2]);

	frus->vc[0] = eye[0];
	frus->vc[1] = eye[1];
	frus->vc[2] = eye[2];


	//l,r
	x = plane->vr[0];
	y = plane->vr[1];
	z = plane->vr[2];
	n = squareroot(x*x + y*y + z*z);
	x /= n;
	y /= n;
	z /= n;

	frus->vl[0] =-x;
	frus->vl[1] =-y;
	frus->vl[2] =-z;
	frus->vl[3] = 
	  (plane->vc[0] - plane->vr[0] - eye[0])*x
	+ (plane->vc[1] - plane->vr[1] - eye[1])*y
	+ (plane->vc[2] - plane->vr[2] - eye[2])*z;

	frus->vr[0] = x;
	frus->vr[1] = y;
	frus->vr[2] = z;
	frus->vr[3] = 
	  (plane->vc[0] + plane->vr[0] - eye[0])*x
	+ (plane->vc[1] + plane->vr[1] - eye[1])*y
	+ (plane->vc[2] + plane->vr[2] - eye[2])*z;


	//b,t
	x = plane->vt[0];
	y = plane->vt[1];
	z = plane->vt[2];
	n = squareroot(x*x + y*y + z*z);
	x /= n;
	y /= n;
	z /= n;

	frus->vb[0] =-x;
	frus->vb[1] =-y;
	frus->vb[2] =-z;
	frus->vb[3] = 
	  (plane->vc[0] - plane->vt[0] - eye[0])*x
	+ (plane->vc[1] - plane->vt[1] - eye[1])*y
	+ (plane->vc[2] - plane->vt[2] - eye[2])*z;

	frus->vt[0] = x;
	frus->vt[1] = y;
	frus->vt[2] = z;
	frus->vt[3] = 
	  (plane->vc[0] + plane->vt[0] - eye[0])*x
	+ (plane->vc[1] + plane->vt[1] - eye[1])*y
	+ (plane->vc[2] + plane->vt[2] - eye[2])*z;


	//n,f
	x = plane->vf[0];
	y = plane->vf[1];
	z = plane->vf[2];
	n = squareroot(x*x + y*y + z*z);
	x /= n;
	y /= n;
	z /= n;

	frus->vn[0] = x;
	frus->vn[1] = y;
	frus->vn[2] = z;
	frus->vn[3] = 
	  (plane->vc[0] - eye[0])*x
	+ (plane->vc[1] - eye[1])*y
	+ (plane->vc[2] - eye[2])*z;

	frus->vf[0] = x;
	frus->vf[1] = y;
	frus->vf[2] = z;
	frus->vf[3] = 1e10;
/*	say("vrglass_frustum: (%f,%f), (%f,%f), (%f,%f), (%f,%f,%f)\n",
		frus->vn[3], frus->vf[3], frus->vl[3], frus->vr[3], frus->vb[3], frus->vt[3],
		frus->vc[0], frus->vc[1], frus->vc[2]);*/
}
static void vrglass_matrix(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom)
{
	struct fstyle* frus = &geom->frus;

	float* mat = act->MATBUF;
	fixmatrix_transpose((void*)mat, frus);
	//printmat4(mat);
}
static void vrglass_camera(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	struct glsrc* src = act->CAMBUF;
	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = act->MATBUF;
	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = frus->vc;
	wnd->gl_camera[0] = act->CAMBUF;
}




static void vrglass_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> texball
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack && ('v' == len)){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;slot = self->pfoot;
		vrglass_draw_gl41(act,slot, wrd,geom, wnd,area);
	}
}
static void vrglass_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd.area -> cam.gl41
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;
	relationsearch(cam, _in_, &stack[rsp+0], &stack[rsp+1]);

//cam.slot -> world.geom
	struct entity* act;struct style* slot;
	struct entity* wrd;struct style* geom;
	act = stack[rsp+0]->pchip;slot = stack[rsp+0]->pfoot;
	wrd = stack[rsp+1]->pchip;geom = stack[rsp+1]->pfoot;

	if('v' == len){
		vrglass_frustum(&geom->frustum, &geom->fshape, geom->fshape.vq);
		vrglass_matrix(act,slot, wrd,geom);

		gl41data_read(self, peer, stack, rsp+2, buf, len);
		vrglass_draw_gl41(act,slot, wrd,geom, wnd,area);
	}
	if('?' == len){
		gl41data_read(self, peer, stack, rsp+2, buf, len);
		vrglass_camera(act,slot, wrd,geom, wnd,area);
	}
}




static void vrglass_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* ent = peer->pchip;
	switch(ent->fmt){
	case _gl41fbog_:
	case _gl41wnd0_:
	case _full_:
	case _wnd_:vrglass_read_bywnd(self, peer, stack, rsp, buf, len);break;
	default:vrglass_read_bycam(self, peer, stack, rsp, buf, len);break;
	}
}
static int vrglass_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	vrglass_event(self->pchip, 0, buf, 0);
	return 0;
}
static void vrglass_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void vrglass_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void vrglass_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('v', 'r', 'g', 'l', 'a', 's', 's', 0);

	p->oncreate = (void*)vrglass_create;
	p->ondelete = (void*)vrglass_delete;
	p->onsearch = (void*)vrglass_search;
	p->onmodify = (void*)vrglass_modify;

	p->onlinkup = (void*)vrglass_linkup;
	p->ondiscon = (void*)vrglass_discon;
	p->onread  = (void*)vrglass_read;
	p->onwrite = (void*)vrglass_write;
}
