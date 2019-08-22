#include "libuser.h"
void fixmatrix(float* m, struct fstyle* sty);
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



/*
void freecam_fixcam(struct actor* win)
{
	float w,h,t;
	float x,y,z,norm;

	//near
	x = win->target.vc[0] - win->camera.vc[0];
	y = win->target.vc[1] - win->camera.vc[1];
	z = win->target.vc[2] - win->camera.vc[2];
	norm = squareroot(x*x + y*y + z*z);
	x /= norm;
	y /= norm;
	z /= norm;
	win->camera.vn[0] = x;
	win->camera.vn[1] = y;
	win->camera.vn[2] = z;

	//right = cross(near, (0,0,1))
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	x = win->camera.vn[1]*1 - win->camera.vn[2]*0;
	y = win->camera.vn[2]*0 - win->camera.vn[0]*1;
	z = win->camera.vn[0]*0 - win->camera.vn[1]*0;
	norm = squareroot(x*x + y*y + z*z);
	x /= norm;
	y /= norm;
	z /= norm;
	win->camera.vr[0] = x;
	win->camera.vr[1] = y;
	win->camera.vr[2] = z;
	win->camera.vl[0] = -x;
	win->camera.vl[1] = -y;
	win->camera.vl[2] = -z;

	//
	w = win->width;
	h = win->height;
	t = h / w;

	//upper = cross(right, near)
	x = win->camera.vr[1]*win->camera.vn[2] - win->camera.vr[2]*win->camera.vn[1];
	y = win->camera.vr[2]*win->camera.vn[0] - win->camera.vr[0]*win->camera.vn[2];
	z = win->camera.vr[0]*win->camera.vn[1] - win->camera.vr[1]*win->camera.vn[0];
	norm = squareroot(x*x + y*y + z*z);
	x /= norm;
	y /= norm;
	z /= norm;
	win->camera.vt[0] = x * t;
	win->camera.vt[1] = y * t;
	win->camera.vt[2] = z * t;
	win->camera.vb[0] = -x * t;
	win->camera.vb[1] = -y * t;
	win->camera.vb[2] = -z * t;
}
void freecam_rotatey(struct actor* win, float delta)
{
	float q[4];
	float va[4];
	float vb[4];

	//vector = -front
	va[0] = win->camera.vc[0] - win->target.vc[0];
	va[1] = win->camera.vc[1] - win->target.vc[1];
	va[2] = win->camera.vc[2] - win->target.vc[2];
	vb[0] = va[0];
	vb[1] = va[1];
	vb[2] = va[2];

	//right = (-y, x)
	q[0] =-va[1];
	q[1] = va[0];
	q[2] = 0.0;
	quaternion_operation(vb, q, delta);

	//if(dot(cross(va,n), cross(vb,n)) < 0)return;
	if(va[1]*vb[1] + va[0]*vb[0] < 0)return;

	//surround = target+vector
	win->camera.vc[0] = win->target.vc[0] + vb[0];
	win->camera.vc[1] = win->target.vc[1] + vb[1];
	win->camera.vc[2] = win->target.vc[2] + vb[2];
}
void freecam_rotatex(struct actor* win, float delta)
{
	float c = cosine(delta);
	float s = sine(delta);

	//vector = target -> camera
	float vx = win->camera.vc[0] - win->target.vc[0];
	float vy = win->camera.vc[1] - win->target.vc[1];

	//camera = target + vector * r
	win->camera.vc[0] = win->target.vc[0] + vx*c + vy*s;
	win->camera.vc[1] = win->target.vc[1] - vx*s + vy*c;
}
void freecam_rotatexy(struct actor* win, int dx, int dy)
{
	if(0 != dy)freecam_rotatey(win, dy / 100.0);
	if(0 != dx)freecam_rotatex(win, dx / 100.0);
}
void target_deltaxyz(struct actor* win, float x, float y, float z)
{
	float norm;
	float tx, ty;
	float dx, dy, dz;

	tx = win->camera.vn[0];
	ty = win->camera.vn[1];
	norm = squareroot(tx*tx+ty*ty);
	tx /= norm;
	ty /= norm;

	dx = x*ty + y*tx;
	dy = y*ty - x*tx;
	dz = 10*z;

	win->target.vc[0] += dx;
	win->target.vc[1] += dy;
	win->target.vc[2] += dz;

	win->camera.vc[0] += dx;
	win->camera.vc[1] += dy;
	win->camera.vc[2] += dz;
}
void freecam_zoom(struct actor* win, float delta)
{
	float va[4];
	float vb[4];

	//normalize
	float nx = win->camera.vn[0];
	float ny = win->camera.vn[1];
	float nz = win->camera.vn[2];
	float norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//checker
	va[0] = win->camera.vc[0] - win->target.vc[0];
	va[1] = win->camera.vc[1] - win->target.vc[1];
	va[2] = win->camera.vc[2] - win->target.vc[2];
	vb[0] = va[0] + delta * nx;
	vb[1] = va[1] + delta * ny;
	vb[2] = va[2] + delta * nz;
	if(va[0]*vb[0] + va[1]*vb[1] + va[2]*vb[2] < 0)return;

	win->camera.vc[0] += delta * nx;
	win->camera.vc[1] += delta * ny;
	win->camera.vc[2] += delta * nz;
}*/




static int freecam_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{/*
	vec3 tc,tf;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	tc[0] = vc[0] - vf[0]/2;
	tc[1] = vc[1] - vf[1]/2;
	tc[2] = vc[2] - vf[2]/2;
	tf[0] = vf[0] / 2;
	tf[1] = vf[1] / 2;
	tf[2] = vf[2] / 2;
	carvesolid_prism4(win, 0x800000, tc, vr, vu, tf);

	sty->vn[0] = sty->vf[0];
	sty->vn[1] = sty->vf[1];
	sty->vn[2] = sty->vf[2];
	sty->vl[0] = -sty->vr[0];
	sty->vl[1] = -sty->vr[1];
	sty->vl[2] = -sty->vr[2];
	sty->vb[0] = -sty->vu[0];
	sty->vb[1] = -sty->vu[1];
	sty->vb[2] = -sty->vu[2];
	carvefrustum(win, sty);*/
	return 0;
}/*
static int freecam_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	float x,y,z,w;
	int x0,y0,x1,y1,id,sign;

	sign = -1;
	if(_vbo_ == win->fmt)sign = 1;

	if(_kbd_ == ev->what)
	{
		switch(ev->why)
		{
			case 0x4b:{
				win->target.vc[0] -= 100;
				win->camera.vc[0] -= 100;
				break;
			}
			case 0x4d:{
				win->target.vc[0] += 100;
				win->camera.vc[0] += 100;
				break;
			}
			case 0x50:{
				win->target.vc[1] -= sign*100;
				win->camera.vc[1] -= sign*100;
				break;
			}
			case 0x48:{
				win->target.vc[1] += sign*100;
				win->camera.vc[1] += sign*100;
				break;
			}
		}
	}
	else if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case '-':{
				win->target.vc[2] -= 100;
				win->camera.vc[2] -= 100;
				break;
			}
			case '=':{
				win->target.vc[2] += 100;
				win->camera.vc[2] += 100;
				break;
			}
		}
	}
	else if(0x2b70 == ev->what)
	{
		id = (ev->why)>>48;
		switch(id){
			case 'f':freecam_zoom(win, 100.0);break;
			case 'b':freecam_zoom(win,-100.0);break;
			case 'r':return 1;
			default:return 0;
		}
	}
	else if(0x4070 == ev->what)
	{
		id = (ev->why)>>48;
		if('l' == id)id = 10;
		if('r' == id)id = 11;
		else if(id > 10)return 0;
		if(0 == win->input[id].z0)return 0;

		if(	(0 != win->input[0].z0)&&
			(0 != win->input[1].z0))
		{
			x1 = (ev->why)&0xffff;
			y1 = ((ev->why)>>16)&0xffff;
			if(0 == id)
			{
				x1 -= (win->input[1].xn);
				y1 -= (win->input[1].yn);
			}
			if(1 == id)
			{
				x1 -= (win->input[0].xn);
				y1 -= (win->input[0].yn);
			}

			x0 = (win->input[0].xn) - (win->input[1].xn);
			y0 = (win->input[0].yn) - (win->input[1].yn);

			if((x0*x0+y0*y0) < (x1*x1+y1*y1))freecam_zoom(win, 10.0);
			else freecam_zoom(win, -10.0);
		}
		else
		{
			x0 = win->input[id].xn;
			y0 = win->input[id].yn;
			x1 = (ev->why)&0xffff;
			y1 = ((ev->why)>>16)&0xffff;

			freecam_rotatexy(win, x1-x0, y0-y1);
		}
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		//printmemory(t, 16);
		//say("%d,%d\n", t[0], t[1]);
		if(t[3] & joyl_left)		//x-
		{
			win->target.vc[0] -= 10;
			win->camera.vc[0] -= 10;
		}
		if(t[3] & joyl_right)		//x+
		{
			win->target.vc[0] += 10;
			win->camera.vc[0] += 10;
		}
		if(t[3] & joyl_down)		//y-
		{
			win->target.vc[1] -= sign*10;
			win->camera.vc[1] -= sign*10;
		}
		if(t[3] & joyl_up)			//y+
		{
			win->target.vc[1] += sign*10;
			win->camera.vc[1] += sign*10;
		}
		if(t[3] & joyl_trigger)		//z-
		{
			target_deltaxyz(win, 0, 0, -1);
		}
		if(t[3] & joyl_bumper)		//z+
		{
			target_deltaxyz(win, 0, 0, 1);
		}
		if(t[3] & joyl_thumb)		//w-
		{
			win->target.vc[0] = 0.0;
			win->target.vc[1] = 0.0;
			win->target.vc[2] = 0.0;
		}
		if(t[3] & joyl_select)		//w+
		{
			return 0;
		}

		x0 = t[0];
		y0 = t[1];
		if((x0 < -4096) | (x0 > 4096) | (y0 < -4096) | (y0 > 4096))
		{
			target_deltaxyz(win, x0/1000.0, sign*y0/1000.0, 0);
		}
	}
	else if(joy_right == (ev->what & joy_mask))
	{
		t = (void*)ev;

		if(t[3] & joyr_left)		//x-
		{
			freecam_rotatexy(win, -1.0, 0.0);
		}
		if(t[3] & joyr_right)		//x+
		{
			freecam_rotatexy(win, 1.0, 0.0);
		}
		if(t[3] & joyr_down)		//y-
		{
			freecam_rotatexy(win, 0.0, -1.0);
		}
		if(t[3] & joyr_up)			//y+
		{
			freecam_rotatexy(win, 0.0, 1.0);
		}
		if(t[3] & joyr_trigger)		//z-
		{
			freecam_zoom(win, 100.0);
		}
		if(t[3] & joyr_bumper)		//z+
		{
			freecam_zoom(win, -100.0);
		}
		if(t[3] & joyr_thumb)		//w-
		{
			x = win->camera.vc[0] - win->target.vc[0];
			y = win->camera.vc[1] - win->target.vc[1];
			z = win->camera.vc[2] - win->target.vc[2];
			w = squareroot(x*x + y*y + z*z);

			win->camera.vn[0] = 0.0;
			win->camera.vn[1] = w*0.7071067811865476;
			win->camera.vn[2] = -w*0.7071067811865476;

			win->camera.vc[0] = win->target.vc[0] - win->camera.vn[0];
			win->camera.vc[1] = win->target.vc[1] - win->camera.vn[1];
			win->camera.vc[2] = win->target.vc[2] - win->camera.vn[2];
		}
		if(t[3] & joyr_start)		//w+
		{
			return 0;
		}

		x0 = t[0];
		if(x0 <-4096)x1 = x0 + 4096;
		else if(x0 > 4096)x1 = x0 - 4096;
		else x1 = 0;

		y0 = t[1];
		if(y0 <-4096)y1 = y0 + 4096;
		else if(y0 > 4096)y1 = y0 - 4096;
		else y1 = 0;

		if((0 != x1) | (0 != y1))freecam_rotatexy(win, x1/4096.0, y1/4096.0);
	}

	//fix it!
	freecam_fixcam(win);
	return 1;
}*/
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
static int freecam_event1(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	float nx,ny,nz;
	struct relation* rel;
	struct style* s;
	short* t;
	say("freecam_event@%llx:%x,%x\n", act, ev->why, ev->what);

	rel = act->irel0;
	while(1){
		if(0 == rel)return 0;

		if(hex32('g','e','o','m') == rel->dstflag){
			s = (void*)(rel->srcfoot);
			break;
		}
		rel = samedstnextsrc(rel);
	}

	if('p' == (ev->what&0xff))
	{
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		if(t[3] & joyl_left)		//x-
		{
			s->f.vc[0] -= 10;
		}
		if(t[3] & joyl_right)		//x+
		{
			s->f.vc[0] += 10;
		}
		if(t[3] & joyl_down)		//y-
		{
			s->f.vc[1] -= 10;
		}
		if(t[3] & joyl_up)			//y+
		{
			s->f.vc[1] += 10;
		}
		if(t[3] & joyl_trigger)		//z-
		{
			s->f.vc[2] -= 10;
		}
		if(t[3] & joyl_bumper)		//z+
		{
			s->f.vc[2] += 10;
		}
	}
	else if(joy_right == (ev->what & joy_mask))
	{
		t = (void*)ev;
	}
	else if(_char_ == ev->what){
		nx = 10.0/vec3_len(s->f.vr);
		ny = 10.0/vec3_len(s->f.vf);
		nz = 10.0/vec3_len(s->f.vt);
		switch(ev->why){
			case 'a':freecam_move(s->f.vc, s->f.vr,-nx);break;
			case 'd':freecam_move(s->f.vc, s->f.vr, nx);break;
			case 's':freecam_move(s->f.vc, s->f.vf,-ny);break;
			case 'w':freecam_move(s->f.vc, s->f.vf, ny);break;
			case 'q':freecam_move(s->f.vc, s->f.vt,-nz);break;
			case 'e':freecam_move(s->f.vc, s->f.vt, nz);break;

			case 'j':freecam_rotate(s->f.vr, s->f.vf, s->f.vt, 0.05);break;
			case 'l':freecam_rotate(s->f.vr, s->f.vf, s->f.vt,-0.05);break;
			case 'i':freecam_rotate(s->f.vf, s->f.vt, s->f.vr, 0.05);break;
			case 'k':freecam_rotate(s->f.vf, s->f.vt, s->f.vr,-0.05);break;
			case 'u':freecam_rotate(s->f.vr, s->f.vt, s->f.vf,-0.05);break;
			case 'o':freecam_rotate(s->f.vr, s->f.vt, s->f.vf, 0.05);break;
		}
	}

	return 1;
}




void freecam_frustum(struct fstyle* d, struct fstyle* s)
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
	struct actor* act, struct style* pin,
	struct actor* ctx, struct style* sty)
{
	say("@orthcam_matrix:%llx,%llx,%llx,%llx\n", ctx->gl_camera, ctx->gl_light, ctx->gl_solid, ctx->gl_opaque);

	struct relation* rel;
	struct actor* world;
	struct fstyle* obb;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			obb = (void*)(rel->srcfoot);
			break;
		}
		rel = samedstnextsrc(rel);
	}
	if(0 == obb)return;


	float* m = act->buf;
	freecam_frustum(&pin->f, obb);
	fixmatrix((void*)m, &pin->f);
	mat4_transpose((void*)m);
	//printmat4(m);

	struct glsrc* src = ctx->gl_camera;
	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "cammvp";
	src->arg_data[0] = m;

	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "camxyz";
	src->arg_data[1] = obb->vc;
}




static void freecam_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	say("@freecam_read:\n");

	if(ctx){
		switch(ctx->type){
			case _gl41data_:break;//freecam_draw_vbo(act,pin,ctx,sty);
		}
	}
	else{
		switch(win->type){
			case _gl41view_:
			case _gl41wnd0_:freecam_matrix(act, pin, win, sty);
		}
	}
/*
	switch(self->flag){
		case _cam_:freecam_matrix(act, pin, win, sty, buf, len);break;
		//case _obj_:
		default:freecam_draw(act, pin, win, sty);
	}*/
}
static int freecam_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return 0;//freecam_event1(act, pin, win, sty, ev, 0);
}
static void freecam_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void freecam_start(struct halfrel* self, struct halfrel* peer)
{
    say("@freecam_start\n");
}




static void freecam_search(struct actor* act)
{
}
static void freecam_modify(struct actor* act)
{
}
static void freecam_delete(struct actor* act)
{
}
static void freecam_create(struct actor* act, void* arg)
{
    say("@freecam_create\n");
	act->buf = memorycreate(64);
}




void freecam_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('f', 'r', 'e', 'e', 'c', 'a', 'm', 0);

	p->oncreate = (void*)freecam_create;
	p->ondelete = (void*)freecam_delete;
	p->onsearch = (void*)freecam_search;
	p->onmodify = (void*)freecam_modify;

	p->onstart = (void*)freecam_start;
	p->onstop  = (void*)freecam_stop;
	p->onread  = (void*)freecam_read;
	p->onwrite = (void*)freecam_write;
}
