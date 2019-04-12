#include "libuser.h"




void thridperson_fixcam(struct arena* win, float* v)
{
	float w,h;
	float x,y,z,n;

	//near = -v
	n = squareroot(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	win->camera.vn[0] = -v[0] / n;
	win->camera.vn[1] = -v[1] / n;
	win->camera.vn[2] = -v[2] / n;

	//right = cross(near, (0,0,1))
	x = -v[1]*1 + v[2]*0;
	y = -v[2]*0 + v[0]*1;
	z = -v[0]*0 + v[1]*0;
	n = 1.0 / squareroot(x*x + y*y + z*z);
	win->camera.vr[0] = x * n;
	win->camera.vr[1] = y * n;
	win->camera.vr[2] = z * n;
	win->camera.vl[0] =-x * n;
	win->camera.vl[1] =-y * n;
	win->camera.vl[2] =-z * n;

	//w,h
	w = win->width;
	h = win->height;

	//up = cross(right, near)
	x = win->camera.vr[1]*win->camera.vn[2] - win->camera.vr[2]*win->camera.vn[1];
	y = win->camera.vr[2]*win->camera.vn[0] - win->camera.vr[0]*win->camera.vn[2];
	z = win->camera.vr[0]*win->camera.vn[1] - win->camera.vr[1]*win->camera.vn[0];
	n = h / w / squareroot(x*x + y*y + z*z);
	win->camera.vu[0] = x * n;
	win->camera.vu[1] = y * n;
	win->camera.vu[2] = z * n;
	win->camera.vb[0] =-x * n;
	win->camera.vb[1] =-y * n;
	win->camera.vb[2] =-z * n;
}




static int thirdperson_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int j;
	vec3 tc,tr,tf;
	struct relation* rel;
	struct actor* tar;
	struct style* tmpsty;

	//3rdcam's target
	rel = act->irel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_act_ == rel->srctype)
		{
			tar = (void*)(rel->srcchip);
			if(0 == tar)return 0;
			else goto found;
		}

		rel = samedstnextsrc(rel);
	}
	return 0;

found:
	//target's world
	rel = tar->irel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_win_ == rel->srctype)
		{
			tmpsty = (void*)(rel->srcfoot);

			//camera.xyz = target.top + act.vector
			win->camera.vc[0] = tmpsty->vc[0] + tmpsty->vu[0] + act->camera.vc[0];
			win->camera.vc[1] = tmpsty->vc[1] + tmpsty->vu[1] + act->camera.vc[1];
			win->camera.vc[2] = tmpsty->vc[2] + tmpsty->vu[2] + act->camera.vc[2];
			thridperson_fixcam(win, act->camera.vc);
			goto print;
		}

		rel = samedstnextsrc(rel);
	}
	return 0;

print:
	carvefrustum(win, &win->camera);

	tr[0] = 0.025;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 0.025;
	tf[2] = 0.0;

	//camera
	tc[0] = -1.0;
	tc[1] = -0.05 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x0000ff, tc, tr, tf, (void*)"vl: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x0000ff, tc, tr, tf, win->camera.vl);

	tc[0] = -1.0;
	tc[1] = -0.1 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x0000ff, tc, tr, tf, (void*)"vr: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x0000ff, tc, tr, tf, win->camera.vr);

	tc[0] = -1.0;
	tc[1] = -0.15 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x00ff00, tc, tr, tf, (void*)"vb: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x00ff00, tc, tr, tf, win->camera.vb);

	tc[0] = -1.0;
	tc[1] = -0.2 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x00ff00, tc, tr, tf, (void*)"vu: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x00ff00, tc, tr, tf, win->camera.vu);

	tc[0] = -1.0;
	tc[1] = -0.25 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xff0000, tc, tr, tf, (void*)"vn: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xff0000, tc, tr, tf, win->camera.vn);

	tc[0] = -1.0;
	tc[1] = -0.3 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xff0000, tc, tr, tf, (void*)"vf: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xff0000, tc, tr, tf, win->camera.vf);

	tc[0] = -1.0;
	tc[1] = -0.35 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xffffff, tc, tr, tf, (void*)"vq: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xffffff, tc, tr, tf, win->camera.vq);

	tc[0] = -1.0;
	tc[1] = -0.4 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xffffff, tc, tr, tf, (void*)"vc: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xffffff, tc, tr, tf, win->camera.vc);

	return 0;
}
static int thirdperson_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	vec4 v,q;
	float a,c,s;
	int x0,y0,x1,y1,id;
	if(0 == act)return 0;

	//only hook mouse event
	if(0x2b70 == ev->what)
	{
		id = (ev->why)>>48;
		if('f' == id)
		{
			act->camera.vc[0] *= 0.99;
			act->camera.vc[1] *= 0.99;
			act->camera.vc[2] *= 0.99;
			return 1;
		}
		if('b' == id)
		{
			act->camera.vc[0] *= 1.01;
			act->camera.vc[1] *= 1.01;
			act->camera.vc[2] *= 1.01;
			return 1;
		}
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

		//rotate y
		v[0] = act->camera.vc[0];
		v[1] = act->camera.vc[1];
		v[2] = act->camera.vc[2];
		q[0] =-v[1];
		q[1] = v[0];
		q[2] = 0.0;
		quaternion_operation(v, q, (y0-y1)/100.0);

		//dot(cross(va,n), cross(vb,n))
		if(v[0]*act->camera.vc[0] + v[1]*act->camera.vc[1] > 0){
			act->camera.vc[0] = v[0];
			act->camera.vc[1] = v[1];
			act->camera.vc[2] = v[2];
		}

		//rotate x
		a = (x0-x1)/100.0;
		s = sine(a);
		c = cosine(a);
		v[0] = act->camera.vc[0];
		v[1] = act->camera.vc[1];
		act->camera.vc[0] = v[0]*c - v[1]*s;
		act->camera.vc[1] = v[0]*s + v[1]*c;
		return 1;
	}
	return 0;
}
static void thirdperson_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void thirdperson_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void thirdperson_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void thirdperson_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void thirdperson_delete(struct actor* act)
{
}
static void thirdperson_create(struct actor* act, void* str)
{
	if(0 == act)return;
	act->camera.vc[0] = 0.0;
	act->camera.vc[1] =-2000.0;
	act->camera.vc[2] = 500.0;
}




void thirdperson_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('3', 'r', 'd', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)thirdperson_create;
	p->ondelete = (void*)thirdperson_delete;
	p->onstart  = (void*)thirdperson_start;
	p->onstop   = (void*)thirdperson_stop;
	p->oncread  = (void*)thirdperson_cread;
	p->oncwrite = (void*)thirdperson_cwrite;
	p->onsread  = (void*)thirdperson_sread;
	p->onswrite = (void*)thirdperson_swrite;
}
