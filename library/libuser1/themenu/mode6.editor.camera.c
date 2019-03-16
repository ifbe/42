#include "libuser.h"




void camera_deltax(struct arena* win, float delta)
{
	float c,s;
	float tx,ty,tz;		//target
	float cx,cy,cz;		//camera
	float vx,vy,vz;		//vector(tar to cam)(origin)
	float px,py,pz;		//vector(tar to cam)(rotate)

	//target
	tx = win->target.vc[0];
	ty = win->target.vc[1];
	tz = win->target.vc[2];

	//camera
	cx = win->camera.vc[0];
	cy = win->camera.vc[1];
	cz = win->camera.vc[2];

	//vector = -front
	vx = cx-tx;
	vy = cy-ty;
	vz = cz-tz;

	c = cosine(delta);
	s = sine(delta);

	//rotate
	px = vx*c + vy*s;
	py = -vx*s + vy*c;
	pz = vz;

	//camera = target+vector
	win->camera.vc[0] = tx+px;
	win->camera.vc[1] = ty+py;
	win->camera.vc[2] = tz+pz;

	//front = -vector
	win->camera.vf[0] = -px;
	win->camera.vf[1] = -py;
	win->camera.vf[2] = -pz;
}
void camera_deltay(struct arena* win, float delta)
{
	float v[4];
	float q[4];
	float tx,ty,tz;		//target
	float cx,cy,cz;		//camera

	//target
	tx = win->target.vc[0];
	ty = win->target.vc[1];
	tz = win->target.vc[2];

	//camera
	cx = win->camera.vc[0];
	cy = win->camera.vc[1];
	cz = win->camera.vc[2];

	//vector = -front
	v[0] = cx-tx;
	v[1] = cy-ty;
	v[2] = cz-tz;

	//cos=dot(v1,v2) / (|v1|*|v2|)
	q[0] = cosine(delta)*cosine(delta);
	q[1] = (v[2]*v[2]) / (v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if(q[0] < q[1])
	{
		//(v[2]>0&&delta>0) or (v[2]<0&&delta<0)
		if(v[2]*delta > 0)return;
	}

	//right = cross(front, (0,0,1))
	q[0] = v[1]*1 - v[2]*0;
	q[1] = v[2]*0 - v[0]*1;
	q[2] = v[0]*0 - v[1]*0;
	quaternion_operation(v, q, delta);

	//camera = target+vector
	win->camera.vc[0] = tx+v[0];
	win->camera.vc[1] = ty+v[1];
	win->camera.vc[2] = tz+v[2];

	//front = -vector
	win->camera.vf[0] = -v[0];
	win->camera.vf[1] = -v[1];
	win->camera.vf[2] = -v[2];
}
void camera_deltaxy(struct arena* win, int dx, int dy)
{
	float delta;

	if(0 != dx)
	{
		if(dx < 0)delta = -0.05;
		else if(dx > 0)delta = 0.05;
		camera_deltax(win, delta);
	}
	if(0 != dy)
	{
		if(dy < 0)delta = -0.02;
		else if(dy > 0)delta = 0.02;
		camera_deltay(win, delta);
	}
}
void camera_zoom(struct arena* win, float delta)
{
	float x = delta*(win->camera.vf[0]);
	float y = delta*(win->camera.vf[1]);
	float z = delta*(win->camera.vf[2]);

	win->camera.vc[0] += x;
	win->camera.vc[1] += y;
	win->camera.vc[2] += z;

	win->camera.vf[0] -= x;
	win->camera.vf[1] -= y;
	win->camera.vf[2] -= z;
}
void target_deltaxyz(struct arena* win, int x, int y, int z)
{
	float norm;
	float tx, ty;
	float dx, dy, dz;

	if(x < 0)x = -1;
	else if(x > 0)x = 1;
	if(y < 0)y = -1;
	else if(y > 0)y = 1;

	tx = win->camera.vf[0];
	ty = win->camera.vf[1];
	norm = squareroot(tx*tx+ty*ty);
	tx /= norm;
	ty /= norm;

	dx = 10.0*x*ty;
	dx += 10.0*y*tx;
	dy = -10.0*x*tx;
	dy += 10.0*y*ty;
	dz = 10*z;

	win->target.vc[0] += dx;
	win->target.vc[1] += dy;
	win->target.vc[2] += dz;

	win->camera.vc[0] += dx;
	win->camera.vc[1] += dy;
	win->camera.vc[2] += dz;
}




int actorinput_cameraevent_gamepad(struct arena* win, struct event* ev)
{
	short* t;
	float x,y,z,w;
	int x0,y0,sign;

	sign = -1;
	if(_vbo_ == win->fmt)sign = 1;
	if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		if(t[3] & joyl_left)		//x-
		{
			win->target.vc[0] -= 10;
			win->camera.vc[0] -= 10;
			return 0;
		}
		if(t[3] & joyl_right)		//x+
		{
			win->target.vc[0] += 10;
			win->camera.vc[0] += 10;
			return 0;
		}
		if(t[3] & joyl_down)		//y-
		{
			win->target.vc[1] -= sign*10;
			win->camera.vc[1] -= sign*10;
			return 0;
		}
		if(t[3] & joyl_up)			//y+
		{
			win->target.vc[1] += sign*10;
			win->camera.vc[1] += sign*10;
			return 0;
		}
		if(t[3] & joyl_trigger)		//z-
		{
			target_deltaxyz(win, 0, 0, -1);
			return 0;
		}
		if(t[3] & joyl_bumper)		//z+
		{
			target_deltaxyz(win, 0, 0, 1);
			return 0;
		}
		if(t[3] & joyl_stick)		//w-
		{
			win->camera.vc[2] -= win->target.vc[2];
			win->target.vc[2] = 0;
			return 0;
		}
		if(t[3] & joyl_select)		//w+
		{
			return 0;
		}

		x0 = t[0];
		if(x0 < -8192)x0 = -1;
		else if(x0 > 8192)x0 = 1;
		else x0 = 0;

		y0 = t[1];
		if(y0 < -8192)y0 = -1;
		else if(y0 > 8192)y0 = 1;
		else y0 = 0;

		target_deltaxyz(win, x0, sign*y0, 0);
		return 0;
	}
	if(joy_right == (ev->what & joy_mask))
	{
		t = (void*)ev;
/*
		if(t[3] & joyr_left)		//x-
		{
			if(win->nearstride > 1)win->nearstride--;
			x = win->nearstride;
			z = win->neardepth;
			say("%f,%f,fov=%f\n", x, z, arctan2(x, z)*180/PI);
			return 0;
		}
		if(t[3] & joyr_right)		//x+
		{
			if(win->nearstride < 9999)win->nearstride++;
			x = win->nearstride;
			z = win->neardepth;
			say("%f,%f,fov=%f\n", x, z, arctan2(x, z)*180/PI);
			return 0;
		}
		if(t[3] & joyr_down)		//y-
		{
			if(win->neardepth > 1)win->neardepth--;
			x = win->nearstride;
			z = win->neardepth;
			say("%f,%f,fov=%f\n", x, z, arctan2(x, z)*180/PI);
			return 0;
		}
		if(t[3] & joyr_up)			//y+
		{
			if(win->neardepth < 9999)win->neardepth++;
			x = win->nearstride;
			z = win->neardepth;
			say("%f,%f,fov=%f\n", x, z, arctan2(x, z)*180/PI);
			return 0;
		}
*/
		if(t[3] & joyr_trigger)		//z-
		{
			camera_zoom(win, 0.1);
			return 0;
		}
		if(t[3] & joyr_bumper)		//z+
		{
			camera_zoom(win, -0.1);
			return 0;
		}
		if(t[3] & joyr_stick)		//w-
		{
			x = win->camera.vc[0] - win->target.vc[0];
			y = win->camera.vc[1] - win->target.vc[1];
			z = win->camera.vc[2] - win->target.vc[2];
			w = squareroot(x*x + y*y + z*z);

			win->camera.vf[0] = 0.0;
			win->camera.vf[1] = w*0.7071067811865476;
			win->camera.vf[2] = -w*0.7071067811865476;

			win->camera.vc[0] = win->target.vc[0];
			win->camera.vc[1] = win->target.vc[1] - win->camera.vf[1];
			win->camera.vc[2] = win->target.vc[2] - win->camera.vf[2];
			return 0;
		}
		if(t[3] & joyr_start)		//w+
		{
			return 0;
		}

		x0 = t[0];
		if(x0 < -8192)x0 = -1;
		else if(x0 > 8192)x0 = 1;
		else x0 = 0;

		y0 = t[1];
		if(y0 < -8192)y0 = -1;
		else if(y0 > 8192)y0 = 1;
		else y0 = 0;

		camera_deltaxy(win, x0, -y0);
	}
	return 0;
}
int actorinput_cameraevent(struct arena* win, struct event* ev)
{
	int x0,y0,x1,y1,id,sign;

	if(_kbd_ == ev->what)
	{
		sign = -1;
		if(_vbo_ == win->fmt)sign = 1;
		if(0x4b == ev->why)
		{
			win->target.vc[0] -= 10;
			win->camera.vc[0] -= 10;
			return 0;
		}
		else if(0x4d == ev->why)
		{
			win->target.vc[0] += 10;
			win->camera.vc[0] += 10;
			return 0;
		}
		else if(0x50 == ev->why)
		{
			win->target.vc[1] -= sign*10;
			win->camera.vc[1] -= sign*10;
			return 0;
		}
		else if(0x48 == ev->why)
		{
			win->target.vc[1] += sign*10;
			win->camera.vc[1] += sign*10;
			return 0;
		}
	}
	else if(joy_event == (ev->what & 0xff))
	{
		actorinput_cameraevent_gamepad(win, ev);
	}
	else if(0x4070 == ev->what)
	{
		id = (ev->why)>>48;
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
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

			if((x0*x0+y0*y0) < (x1*x1+y1*y1))camera_zoom(win, 0.05);
			else camera_zoom(win, -0.05);
		}
		else
		{
			x0 = win->input[id].xn;
			y0 = win->input[id].yn;
			x1 = (ev->why)&0xffff;
			y1 = ((ev->why)>>16)&0xffff;

			camera_deltaxy(win, x1-x0, y1-y0);
		}
	}
	else if(0x2b70 == ev->what)
	{
		id = (ev->why)>>48;
		if('f' == id)camera_zoom(win, 0.1);
		if('b' == id)camera_zoom(win, -0.1);
	}
	return 0;
}




static void camera_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = win->target.vc;
	carvepoint(win, 0xff0000, vc);
}
static void camera_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	//say("%llx,%llx\n", ev->why, ev->what);
	actorinput_cameraevent(win, ev);
}
static void camera_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void camera_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void camera_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void camera_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
    say("@camera_start\n");
}
static void camera_delete()
{
}
static void camera_create(void* addr)
{
    say("@camera_create\n");
}




void camera_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('c', 'a', 'm', 'e', 'r', 'a', 0, 0);

	p->oncreate = (void*)camera_create;
	p->ondelete = (void*)camera_delete;
	p->onstart  = (void*)camera_start;
	p->onstop   = (void*)camera_stop;
	p->onget    = (void*)camera_cread;
	p->onpost   = (void*)camera_cwrite;
	p->onread   = (void*)camera_sread;
	p->onwrite  = (void*)camera_swrite;
}