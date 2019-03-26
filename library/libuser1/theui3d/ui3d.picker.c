#include "libuser.h"
int relation_choose(void*, void*);
int invmvp(vec3 v, struct arena* win);
int obb_ray(struct style* obb, vec3 ray[], vec3 out[]);




int joystick2style(struct arena* win, struct style* sty, int aaa, short* tmp)
{
	float c,s;
	float tx,ty,tz;
	int x0,y0,sign = -1;
	if(_vbo_ == win->fmt)sign = 1;

	x0 = tmp[0];
	if(x0 < -8192)x0 = -1;
	else if(x0 > 8192)x0 = 1;
	else x0 = 0;

	y0 = tmp[1];
	if(y0 < -8192)y0 = -1;
	else if(y0 > 8192)y0 = 1;
	else y0 = 0;

	if('l' == aaa)
	{
		if(tmp[3] & joyl_left)
		{
			//dpad left
			tx = (sty->vr[0])/16;
			ty = (sty->vr[1])/16;
			tz = (sty->vr[2])/16;

			sty->vr[0] += tx;
			sty->vr[1] += ty;
			sty->vr[2] += tz;

			sty->vc[0] -= tx;
			sty->vc[1] -= ty;
			sty->vc[2] -= tz;
			return 0;
		}
		if(tmp[3] & joyl_right)
		{
			//dpad right
			tx = (sty->vr[0])/16;
			ty = (sty->vr[1])/16;
			tz = (sty->vr[2])/16;

			sty->vr[0] += tx;
			sty->vr[1] += ty;
			sty->vr[2] += tz;

			sty->vc[0] += tx;
			sty->vc[1] += ty;
			sty->vc[2] += tz;
			return 0;
		}
		if(tmp[3] & joyl_down)
		{
			//dpad down
			tx = (sty->vf[0])/16;
			ty = (sty->vf[1])/16;
			tz = (sty->vf[2])/16;

			sty->vf[0] += tx;
			sty->vf[1] += ty;
			sty->vf[2] += tz;

			sty->vc[0] -= sign*tx;
			sty->vc[1] -= sign*ty;
			sty->vc[2] -= sign*tz;
			return 0;
		}
		if(tmp[3] & joyl_up)
		{
			//dpad up
			tx = (sty->vf[0])/16;
			ty = (sty->vf[1])/16;
			tz = (sty->vf[2])/16;

			sty->vf[0] += tx;
			sty->vf[1] += ty;
			sty->vf[2] += tz;

			sty->vc[0] += sign*tx;
			sty->vc[1] += sign*ty;
			sty->vc[2] += sign*tz;
			return 0;
		}
		if(tmp[3] & joyl_trigger)
		{
			//right trigger
			tx = (sty->vu[0])/16;
			ty = (sty->vu[1])/16;
			tz = (sty->vu[2])/16;

			sty->vu[0] += tx;
			sty->vu[1] += ty;
			sty->vu[2] += tz;

			sty->vc[0] -= tx*2;
			sty->vc[1] -= ty*2;
			sty->vc[2] -= tz*2;
			return 0;
		}
		if(tmp[3] & joyl_bumper)
		{
			//right bumper
			tx = (sty->vu[0])/16;
			ty = (sty->vu[1])/16;
			tz = (sty->vu[2])/16;

			sty->vu[0] += tx;
			sty->vu[1] += ty;
			sty->vu[2] += tz;
			return 0;
		}

		//other
		tx = sign*(sty->vf[0])*y0/16;
		ty = sign*(sty->vf[1])*y0/16;
		tz = sign*(sty->vf[2])*y0/16;
		sty->vc[0] += (sty->vr[0])*x0/16 + tx;
		sty->vc[1] += (sty->vr[1])*x0/16 + ty;
		sty->vc[2] += (sty->vr[2])*x0/16 + tz;
	}

	if('r' == aaa)
	{
		if(tmp[3] & joyr_left)
		{
			//key x
			tx = (sty->vr[0])/16;
			ty = (sty->vr[1])/16;
			tz = (sty->vr[2])/16;

			sty->vr[0] -= tx;
			sty->vr[1] -= ty;
			sty->vr[2] -= tz;

			sty->vc[0] += tx;
			sty->vc[1] += ty;
			sty->vc[2] += tz;
			return 0;
		}
		if(tmp[3] & joyr_right)
		{
			//key b
			tx = (sty->vr[0])/16;
			ty = (sty->vr[1])/16;
			tz = (sty->vr[2])/16;

			sty->vr[0] -= tx;
			sty->vr[1] -= ty;
			sty->vr[2] -= tz;

			sty->vc[0] -= tx;
			sty->vc[1] -= ty;
			sty->vc[2] -= tz;
			return 0;
		}
		if(tmp[3] & joyr_down)
		{
			//key a
			tx = (sty->vf[0])/16;
			ty = (sty->vf[1])/16;
			tz = (sty->vf[2])/16;

			sty->vf[0] -= tx;
			sty->vf[1] -= ty;
			sty->vf[2] -= tz;

			sty->vc[0] += sign*tx;
			sty->vc[1] += sign*ty;
			sty->vc[2] += sign*tz;
			return 0;
		}
		if(tmp[3] & joyr_up)
		{
			//key y
			tx = (sty->vf[0])/16;
			ty = (sty->vf[1])/16;
			tz = (sty->vf[2])/16;

			sty->vf[0] -= tx;
			sty->vf[1] -= ty;
			sty->vf[2] -= tz;

			sty->vc[0] -= sign*tx;
			sty->vc[1] -= sign*ty;
			sty->vc[2] -= sign*tz;
			return 0;
		}
		if(tmp[3] & joyr_trigger)
		{
			//left trigger
			tx = (sty->vu[0])/16;
			ty = (sty->vu[1])/16;
			tz = (sty->vu[2])/16;

			sty->vu[0] -= tx;
			sty->vu[1] -= ty;
			sty->vu[2] -= tz;

			sty->vc[0] += tx*2;
			sty->vc[1] += ty*2;
			sty->vc[2] += tz*2;
			return 0;
		}
		if(tmp[3] & joyr_bumper)
		{
			//left bumper
			tx = (sty->vu[0])/16;
			ty = (sty->vu[1])/16;
			tz = (sty->vu[2])/16;

			sty->vu[0] -= tx;
			sty->vu[1] -= ty;
			sty->vu[2] -= tz;
			return 0;
		}

		//other
		if(x0 < 0)
		{
			c = cosine(0.05);
			s = sine(0.05);
		}
		else if(x0 > 0)
		{
			c = cosine(-0.05);
			s = sine(-0.05);
		}
		else return 0;

		tx = sty->vr[0];
		ty = sty->vr[1];
		sty->vr[0] = tx*c - ty*s;
		sty->vr[1] = tx*s + ty*c;

		tx = sty->vf[0];
		ty = sty->vf[1];
		sty->vf[0] = tx*c - ty*s;
		sty->vf[1] = tx*s + ty*c;
	}

	return 0;
}
int keyboard2style(struct arena* win, struct style* sty, short* tmp)
{
	int sign = -1;
	if(_vbo_ == win->fmt)sign = 1;
	if(0x4b == tmp[0])
	{
		//left
		sty->vc[0] -= (sty->vr[0])/16;
		sty->vc[1] -= (sty->vr[1])/16;
		sty->vc[2] -= (sty->vr[2])/16;
		return 0;
	}
	else if(0x4d == tmp[0])
	{
		//right
		sty->vc[0] += (sty->vr[0])/16;
		sty->vc[1] += (sty->vr[1])/16;
		sty->vc[2] += (sty->vr[2])/16;
		return 0;
	}
	else if(0x50 == tmp[0])
	{
		//down
		sty->vc[0] -= sign*(sty->vf[0])/16;
		sty->vc[1] -= sign*(sty->vf[1])/16;
		sty->vc[2] -= sign*(sty->vf[2])/16;
		return 0;
	}
	else if(0x48 == tmp[0])
	{
		//up
		sty->vc[0] += sign*(sty->vf[0])/16;
		sty->vc[1] += sign*(sty->vf[1])/16;
		sty->vc[2] += sign*(sty->vf[2])/16;
		return 0;
	}
	return 0;
}
int actorinput_editor_target(struct arena* win, struct event* ev)
{
	float c,s,tx,ty,norm;
	struct relation* orel;
	struct style* sty;
	int ax, ay, aaa, bbb, sign;
	int x = (ev->why)&0xffff;
	int y = ((ev->why)>>16)&0xffff;
	int z = ((ev->why)>>32)&0xffff;
	int id = ((ev->why)>>48)&0xffff;

	if(_vbo_ == win->fmt)sign = 1;
	else sign = -1;

	orel = win->oreln;
	if(0 == orel)return 1;

	sty = (void*)(orel->srcfoot);
	if(0 == sty)return 1;

	if(_char_ == ev->what)
	{
		if(8 == ev->why)relationdelete(orel);
		return 0;
	}
	if(_kbd_ == ev->what)
	{
		keyboard2style(win, sty, (void*)ev);
		return 0;
	}
	if(joy_left == (ev->what & joy_mask))
	{
		joystick2style(win, sty, 'l', (void*)ev);
		return 0;
	}
	if(joy_right == (ev->what & joy_mask))
	{
		joystick2style(win, sty, 'r', (void*)ev);
		return 0;
	}

	if('f' == id)
	{
		sty->vr[0] = (sty->vr[0])*17/16;
		sty->vr[1] = (sty->vr[1])*17/16;
		sty->vr[2] = (sty->vr[2])*17/16;

		sty->vf[0] = (sty->vf[0])*17/16;
		sty->vf[1] = (sty->vf[1])*17/16;
		sty->vf[2] = (sty->vf[2])*17/16;

		sty->vu[0] = (sty->vu[0])*17/16;
		sty->vu[1] = (sty->vu[1])*17/16;
		sty->vu[2] = (sty->vu[2])*17/16;
		return 0;
	}
	if('b' == id)
	{
		sty->vr[0] = (sty->vr[0])*15/16;
		sty->vr[1] = (sty->vr[1])*15/16;
		sty->vr[2] = (sty->vr[2])*15/16;

		sty->vf[0] = (sty->vf[0])*15/16;
		sty->vf[1] = (sty->vf[1])*15/16;
		sty->vf[2] = (sty->vf[2])*15/16;

		sty->vu[0] = (sty->vu[0])*15/16;
		sty->vu[1] = (sty->vu[1])*15/16;
		sty->vu[2] = (sty->vu[2])*15/16;
		return 0;
	}
	if(hex32('p','+',0,0) == ev->what)
	{
		//playwith3d_pick(win, x, y);
		return 0;
	}
	if(hex32('p','@',0,0) == ev->what)
	{
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;
		if(0 == win->input[id].z0)return 0;

		//two finger
		if(	(0 != win->input[0].z0)&&
			(0 != win->input[1].z0) )
		{
			if(0 == id)
			{
				x -= (win->input[1].xn);
				y -= (win->input[1].yn);
			}
			if(1 == id)
			{
				x -= (win->input[0].xn);
				y -= (win->input[0].yn);
			}

			ax = (win->input[0].xn) - (win->input[1].xn);
			ay = (win->input[0].yn) - (win->input[1].yn);

			aaa = x*x+y*y;
			bbb = ax*ax + ay*ay;

			sty->vr[0] = (sty->vr[0]) * aaa / bbb;
			sty->vr[1] = (sty->vr[1]) * aaa / bbb;
			sty->vr[2] = (sty->vr[2]) * aaa / bbb;

			sty->vf[0] = (sty->vf[0]) * aaa / bbb;
			sty->vf[1] = (sty->vf[1]) * aaa / bbb;
			sty->vf[2] = (sty->vf[2]) * aaa / bbb;

			sty->vu[0] = (sty->vu[0]) * aaa / bbb;
			sty->vu[1] = (sty->vu[1]) * aaa / bbb;
			sty->vu[2] = (sty->vu[2]) * aaa / bbb;
		}
		else if((0 == id)|(10 == id))
		{
			tx = win->camera.vf[0];
			ty = win->camera.vf[1];
			norm = squareroot(tx*tx+ty*ty);
			tx *= 2.0 / norm;
			ty *= 2.0 / norm;

			ax = x - (win->input[id].xn);
			ay = y - (win->input[id].yn);

			sty->vc[0] += ty*ax - tx*ay;
			sty->vc[1] -= sign*(tx*ax + ty*ay);
			//say("%x,%x\n", sty->vc[0], sty->vc[1]);
		}
		else if(11 == id)
		{
			if(x > win->input[id].x0)
			{
				c = cosine(0.05);
				s = sine(0.05);
			}
			else if(x < win->input[id].x0)
			{
				c = cosine(-0.05);
				s = sine(-0.05);
			}
			else return 0;

			tx = sty->vr[0];
			ty = sty->vr[1];
			sty->vr[0] = tx*c - ty*s;
			sty->vr[1] = tx*s + ty*c;

			tx = sty->vf[0];
			ty = sty->vf[1];
			sty->vf[0] = tx*c - ty*s;
			sty->vf[1] = tx*s + ty*c;
		}
	}
	return 1;
}




int playwith3d_pick(struct arena* root, struct arena* twig, int x, int y)
{
	int ret;
	vec3 ray[2];
	vec3 out[2];
	struct style* sty;
	struct relation* rel;

	float w = root->width;
	float h = root->height;
	float fx = (float)x * 2.0;
	float fy = (float)y * 2.0;

	ray[0][0] = root->camera.vc[0];
	ray[0][1] = root->camera.vc[1];
	ray[0][2] = root->camera.vc[2];
	ray[1][0] = fx / w - 1.0;
	ray[1][1] = 1.0 - fy / h;
	ray[1][2] = 0.0;

	invmvp(ray[1], root);
	ray[1][0] -= ray[0][0];
	ray[1][1] -= ray[0][1];
	ray[1][2] -= ray[0][2];
	say("(%f,%f,%f) -> (%f,%f,%f)\n",
		ray[0][0], ray[0][1], ray[0][2],
		ray[1][0], ray[1][1], ray[1][2]
	);

	rel = twig->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			ret = obb_ray(sty, ray, out);
			say("rel=%llx, ret=%d\n", rel, ret);
			if(ret > 0)break;
		}

		rel = samesrcprevdst(rel);
	}
	if(rel)relation_choose(twig, rel);
	return 0;
}
int playwith3d_move(struct arena* root, struct arena* twig, int x0, int y0, int xn, int yn)
{
	int ret;
	float dx, dy;
	vec3 ray0[2];
	vec3 rayn[2];
	vec3 out[2];
	struct style* sty;
	struct relation* rel;

	float w = root->width;
	float h = root->height;
	float fx0 = (float)x0 * 2.0;
	float fy0 = (float)y0 * 2.0;
	float fxn = (float)xn * 2.0;
	float fyn = (float)yn * 2.0;

	ray0[0][0] = root->camera.vc[0];
	ray0[0][1] = root->camera.vc[1];
	ray0[0][2] = root->camera.vc[2];
	ray0[1][0] = fx0 / w - 1.0;
	ray0[1][1] = 1.0 - fy0 / h;
	ray0[1][2] = 0.0;
	invmvp(ray0[1], root);
	ray0[1][0] -= ray0[0][0];
	ray0[1][1] -= ray0[0][1];
	ray0[1][2] -= ray0[0][2];

	rayn[0][0] = root->camera.vc[0];
	rayn[0][1] = root->camera.vc[1];
	rayn[0][2] = root->camera.vc[2];
	rayn[1][0] = fxn / w - 1.0;
	rayn[1][1] = 1.0 - fyn / h;
	rayn[1][2] = 0.0;
	invmvp(rayn[1], root);
	rayn[1][0] -= rayn[0][0];
	rayn[1][1] -= rayn[0][1];
	rayn[1][2] -= rayn[0][2];

	//x = x0 - x1*z0/z1
	dx = (ray0[0][0] - ray0[1][0]*ray0[0][2]/ray0[1][2])
	   - (rayn[0][0] - rayn[1][0]*rayn[0][2]/rayn[1][2]);
	dy = (ray0[0][1] - ray0[1][1]*ray0[0][2]/ray0[1][2])
	   - (rayn[0][1] - rayn[1][1]*rayn[0][2]/rayn[1][2]);
	say("%f,%f\n", dx, dy);

	rel = twig->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			goto found;
		}

		rel = samesrcprevdst(rel);
	}
	return 0;

found:
	sty->vc[0] += dx;
	sty->vc[1] += dy;
	return 0;
}




static int picker_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct relation* rel;
	struct arena* www;
	struct style* sss;

	carveline_prism4(win, 0xff00ff, win->target.vc, win->target.vr, win->target.vf, win->target.vu);

	www = 0;
	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype)
		{
			www = (void*)(rel->dstchip);
			if(_fg3d_ == www->fmt)goto found;
		}

		rel = samesrcnextdst(rel);
	}
	return 0;

found:
	rel = www->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sss = (void*)(rel->srcfoot);
			carveline_prism4(win, 0xffffff, sss->vc, sss->vr, sss->vf, sss->vu);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
static int picker_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	float w = win->width;
	float h = win->height;
	win->nearl = win->nearb * w/h;
	win->nearr = win->neart * w/h;

	int x,y,z;
	struct arena* www = 0;
	struct relation* rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype)
		{
			www = (void*)(rel->dstchip);
			if(_fg3d_ == www->fmt)goto found;
		}

		rel = samesrcnextdst(rel);
	}
	return 0;

found:
	if(hex32('p','+',0,0) == ev->what)
	{
		x = ev->why & 0xffff;
		y = (ev->why >> 16) & 0xffff;
		playwith3d_pick(win, www, x, y);
	}
	if(hex32('p','@',0,0) == ev->what)
	{
		z = win->input[10].z0;
		if(z)
		{
			x = ev->why & 0xffff;
			y = (ev->why >> 16) & 0xffff;
			playwith3d_move(win, www, x, y, win->input[10].xn, win->input[10].yn);
		}
	}
	return 0;
}
static void picker_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void picker_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void picker_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void picker_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
    say("@picker_start\n");
}
static void picker_delete()
{
}
static void picker_create(void* addr)
{
    say("@picker_create\n");
}




void picker_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'i', 'c', 'k', 'e', 'r', 0, 0);

	p->oncreate = (void*)picker_create;
	p->ondelete = (void*)picker_delete;
	p->onstart  = (void*)picker_start;
	p->onstop   = (void*)picker_stop;
	p->onget    = (void*)picker_cread;
	p->onpost   = (void*)picker_cwrite;
	p->onread   = (void*)picker_sread;
	p->onwrite  = (void*)picker_swrite;
}