#include "libuser.h"




void surround_fixcam(struct arena* win)
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
	win->camera.vu[0] = x * t;
	win->camera.vu[1] = y * t;
	win->camera.vu[2] = z * t;
	win->camera.vb[0] = -x * t;
	win->camera.vb[1] = -y * t;
	win->camera.vb[2] = -z * t;
}
void surround_rotatey(struct arena* win, float delta)
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
void surround_rotatex(struct arena* win, float delta)
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
void surround_rotatexy(struct arena* win, int dx, int dy)
{
	if(0 != dy)surround_rotatey(win, dy / 100.0);
	if(0 != dx)surround_rotatex(win, dx / 100.0);
}
void target_deltaxyz(struct arena* win, float x, float y, float z)
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
void surround_zoom(struct arena* win, float delta)
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
}




static int surround_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	sty->vn[0] = sty->vf[0];
	sty->vn[1] = sty->vf[1];
	sty->vn[2] = sty->vf[2];
	sty->vl[0] = -sty->vr[0];
	sty->vl[1] = -sty->vr[1];
	sty->vl[2] = -sty->vr[2];
	sty->vb[0] = -sty->vu[0];
	sty->vb[1] = -sty->vu[1];
	sty->vb[2] = -sty->vu[2];
	carvefrustum(win, sty);
	return 0;
}
static int surround_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
			case 'f':surround_zoom(win, 100.0);break;
			case 'b':surround_zoom(win,-100.0);break;
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

			if((x0*x0+y0*y0) < (x1*x1+y1*y1))surround_zoom(win, 10.0);
			else surround_zoom(win, -10.0);
		}
		else
		{
			x0 = win->input[id].xn;
			y0 = win->input[id].yn;
			x1 = (ev->why)&0xffff;
			y1 = ((ev->why)>>16)&0xffff;

			surround_rotatexy(win, x1-x0, y0-y1);
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
			surround_rotatexy(win, -1.0, 0.0);
		}
		if(t[3] & joyr_right)		//x+
		{
			surround_rotatexy(win, 1.0, 0.0);
		}
		if(t[3] & joyr_down)		//y-
		{
			surround_rotatexy(win, 0.0, -1.0);
		}
		if(t[3] & joyr_up)			//y+
		{
			surround_rotatexy(win, 0.0, 1.0);
		}
		if(t[3] & joyr_trigger)		//z-
		{
			surround_zoom(win, 100.0);
		}
		if(t[3] & joyr_bumper)		//z+
		{
			surround_zoom(win, -100.0);
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

		if((0 != x1) | (0 != y1))surround_rotatexy(win, x1/4096.0, y1/4096.0);
	}

	//fix it!
	surround_fixcam(win);
	return 1;
}
static void surround_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void surround_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void surround_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void surround_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
    say("@surround_start\n");
}
static void surround_delete()
{
}
static void surround_create(void* addr)
{
    say("@surround_create\n");
}




void surround_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'u', 'r', 'r', 'o', 'u', 'n', 'd');

	p->oncreate = (void*)surround_create;
	p->ondelete = (void*)surround_delete;
	p->onstart  = (void*)surround_start;
	p->onstop   = (void*)surround_stop;
	p->oncread  = (void*)surround_cread;
	p->oncwrite = (void*)surround_cwrite;
	p->onsread  = (void*)surround_sread;
	p->onswrite = (void*)surround_swrite;
}
