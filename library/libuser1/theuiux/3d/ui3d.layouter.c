#include "libuser.h"
int relation_choose(void*, void*);
int invmvp(vec3 v, struct fstyle* sty);
int obb_ray(struct fstyle* obb, vec3 ray[], vec3 out[]);




int joystick2style(struct arena* win, struct fstyle* sty, int aaa, short* tmp)
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
			tx = (sty->vt[0])/16;
			ty = (sty->vt[1])/16;
			tz = (sty->vt[2])/16;

			sty->vt[0] += tx;
			sty->vt[1] += ty;
			sty->vt[2] += tz;

			sty->vc[0] -= tx*2;
			sty->vc[1] -= ty*2;
			sty->vc[2] -= tz*2;
			return 0;
		}
		if(tmp[3] & joyl_bumper)
		{
			//right bumper
			tx = (sty->vt[0])/16;
			ty = (sty->vt[1])/16;
			tz = (sty->vt[2])/16;

			sty->vt[0] += tx;
			sty->vt[1] += ty;
			sty->vt[2] += tz;
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
			tx = (sty->vt[0])/16;
			ty = (sty->vt[1])/16;
			tz = (sty->vt[2])/16;

			sty->vt[0] -= tx;
			sty->vt[1] -= ty;
			sty->vt[2] -= tz;

			sty->vc[0] += tx*2;
			sty->vc[1] += ty*2;
			sty->vc[2] += tz*2;
			return 0;
		}
		if(tmp[3] & joyr_bumper)
		{
			//left bumper
			tx = (sty->vt[0])/16;
			ty = (sty->vt[1])/16;
			tz = (sty->vt[2])/16;

			sty->vt[0] -= tx;
			sty->vt[1] -= ty;
			sty->vt[2] -= tz;
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
int keyboard2style(struct arena* win, struct fstyle* sty, short* tmp)
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
	struct fstyle* sty;
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

		sty->vt[0] = (sty->vt[0])*17/16;
		sty->vt[1] = (sty->vt[1])*17/16;
		sty->vt[2] = (sty->vt[2])*17/16;
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

		sty->vt[0] = (sty->vt[0])*15/16;
		sty->vt[1] = (sty->vt[1])*15/16;
		sty->vt[2] = (sty->vt[2])*15/16;
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

			sty->vt[0] = (sty->vt[0]) * aaa / bbb;
			sty->vt[1] = (sty->vt[1]) * aaa / bbb;
			sty->vt[2] = (sty->vt[2]) * aaa / bbb;
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




int playwith3d_pick(struct arena* root, struct arena* twig, struct actor* act, int x, int y)
{
	int ret;
	vec3 ray[2];
	vec3 out[2];
	struct fstyle* sty;
	struct relation* rel;

	float w = root->width;
	float h = root->height;
	float fx = (float)x / w;
	float fy = (float)y / h;

	ray[0][0] = root->camera.vc[0];
	ray[0][1] = root->camera.vc[1];
	ray[0][2] = root->camera.vc[2];
	ray[1][0] = 2.0 * fx - 1.0;
	ray[1][1] = 1.0 - 2.0 * fy;
	ray[1][2] = 0.0;

	invmvp(ray[1], &root->camera);
	say("(%f,%f,%f) -> (%f,%f,%f)\n",
		ray[0][0], ray[0][1], ray[0][2],
		ray[1][0], ray[1][1], ray[1][2]
	);

	ray[1][0] -= ray[0][0];
	ray[1][1] -= ray[0][1];
	ray[1][2] -= ray[0][2];
	act->target.vc[0] = ray[0][0] - ray[1][0]*ray[0][2]/ray[1][2];
	act->target.vc[1] = ray[0][1] - ray[1][1]*ray[0][2]/ray[1][2];
	act->target.vc[2] = 0.0;

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
	struct fstyle* sty;
	struct relation* rel;

	float w = root->width;
	float h = root->height;
	float fx0 = (float)x0 / w;
	float fy0 = (float)y0 / h;
	float fxn = (float)xn / w;
	float fyn = (float)yn / h;

	ray0[0][0] = root->camera.vc[0];
	ray0[0][1] = root->camera.vc[1];
	ray0[0][2] = root->camera.vc[2];
	ray0[1][0] = 2.0*fx0 - 1.0;
	ray0[1][1] = 1.0 - 2.0*fy0;
	ray0[1][2] = 0.0;
	invmvp(ray0[1], &root->camera);
	ray0[1][0] -= ray0[0][0];
	ray0[1][1] -= ray0[0][1];
	ray0[1][2] -= ray0[0][2];

	rayn[0][0] = root->camera.vc[0];
	rayn[0][1] = root->camera.vc[1];
	rayn[0][2] = root->camera.vc[2];
	rayn[1][0] = 2.0*fxn - 1.0;
	rayn[1][1] = 1.0 - 2.0*fyn;
	rayn[1][2] = 0.0;
	invmvp(rayn[1], &root->camera);
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




static int picker_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int flag;
	vec3 tc;
	struct relation* rel;
	struct arena* www;
	struct fstyle* sss;
	//carvefrustum(win, &win->camera);
	//carveline_prism4(win, 0xff00ff, win->target.vc, win->target.vr, win->target.vf, win->target.vu);

	tc[0] = act->target.vc[0];
	tc[1] = act->target.vc[1];
	tc[2] = act->target.vc[2] + 1000.0*1000.0;
	carveline(win, 0xff00ff, act->target.vc, tc);
//say(">>>>>%f,%f,%f\n", tc[0], tc[1], tc[2]);

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
	flag = 0;
	rel = www->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sss = (void*)(rel->srcfoot);

			if(0 == flag)carveline_prism4(win, 0xffffff, sss->vc, sss->vr, sss->vf, sss->vt);
			else carveopaque_prism4(win, 0xffffff, sss->vc, sss->vr, sss->vf, sss->vt);

			flag ++;
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
static int picker_event(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	struct arena* www = 0;
	struct relation* rel = win->orel0;
	//say("@picker_swrite:%llx,%llx\n", ev->what, ev->why);

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
	t = (void*)ev;
	if('l' != t[3])return 0;

	if(hex32('p','+',0,0) == ev->what)
	{
		playwith3d_pick(win, www, act, t[0], t[1]);
		return 1;
	}
	if(hex32('p','@',0,0) == ev->what)
	{
		if(win->input[10].z0)playwith3d_move(win, www, t[0], t[1], win->input[10].xn, win->input[10].yn);
		if(win->input[ 0].z0)playwith3d_move(win, www, t[0], t[1], win->input[ 0].xn, win->input[ 0].yn);
		return 1;
	}
	return 0;
}




static void picker_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	picker_draw(act, pin, win, sty);
}
static int picker_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return picker_event(act, pin, win, sty, ev, 0);
}
static void picker_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void picker_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void picker_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void picker_start(struct halfrel* self, struct halfrel* peer)
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
	p->fmt = hex64('p', 'i', 'c', 'k', 'e', 'r', 0, 0);

	p->oncreate = (void*)picker_create;
	p->ondelete = (void*)picker_delete;
	p->onstart  = (void*)picker_start;
	p->onstop   = (void*)picker_stop;
	p->oncread  = (void*)picker_cread;
	p->oncwrite = (void*)picker_cwrite;
	p->onsread  = (void*)picker_sread;
	p->onswrite = (void*)picker_swrite;
}
