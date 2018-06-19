#include "libuser.h"
int relation_swap(void*, void*);
int invmvp(vec3 v, struct arena* win);
int ray_obb(vec3 ray[], struct style* sty, vec3 out[]);




int joystick2style(struct style* sty, short* tmp)
{
	int x0,y0;
	float c,s;
	float tx,ty,tz;

	if(_dl_ == tmp[2])
	{
		//dpad left
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
	else if(_dr_ == tmp[2])
	{
		//dpad right
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
	else if(_dn_ == tmp[2])
	{
		//dpad down
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] -= tx;
		sty->vf[1] -= ty;
		sty->vf[2] -= tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if(_df_ == tmp[2])
	{
		//dpad up
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] -= tx;
		sty->vf[1] -= ty;
		sty->vf[2] -= tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if(_lt_ == tmp[2])
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
	else if(_lb_ == tmp[2])
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
	else if(_kx_ == tmp[2])
	{
		//key x
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
	else if(_kb_ == tmp[2])
	{
		//key b
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
	else if(_ka_ == tmp[2])
	{
		//key a
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] += tx;
		sty->vf[1] += ty;
		sty->vf[2] += tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if(_ky_ == tmp[2])
	{
		//key y
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] += tx;
		sty->vf[1] += ty;
		sty->vf[2] += tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if(_rt_ == tmp[2])
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
	else if(_rb_ == tmp[2])
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

	x0 = tmp[0];
	if(x0 < -8192)x0 = -1;
	else if(x0 > 8192)x0 = 1;
	else x0 = 0;

	y0 = tmp[1];
	if(y0 < -8192)y0 = -1;
	else if(y0 > 8192)y0 = 1;
	else y0 = 0;

	if('l' == tmp[2])
	{
		sty->vc[0] += (sty->vr[0])*x0/16 + (sty->vf[0])*y0/16;
		sty->vc[1] += (sty->vr[1])*x0/16 + (sty->vf[1])*y0/16;
		sty->vc[2] += (sty->vr[2])*x0/16 + (sty->vf[2])*y0/16;
	}
	else if('r' == tmp[2])
	{
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
int keyboard2style(struct style* sty, short tmp)
{
	int x0,y0;
	float c,s;
	float tx,ty,tz;

	if('s' == tmp)
	{
		//dpad left
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
	else if('f' == tmp)
	{
		//dpad right
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
	else if('d' == tmp)
	{
		//dpad down
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] -= tx;
		sty->vf[1] -= ty;
		sty->vf[2] -= tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if('e' == tmp)
	{
		//dpad up
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] -= tx;
		sty->vf[1] -= ty;
		sty->vf[2] -= tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if('w' == tmp)
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
	else if('r' == tmp)
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
	else if('j' == tmp)
	{
		//key x
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
	else if('l' == tmp)
	{
		//key b
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
	else if('k' == tmp)
	{
		//key a
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] += tx;
		sty->vf[1] += ty;
		sty->vf[2] += tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if('i' == tmp)
	{
		//key y
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] += tx;
		sty->vf[1] += ty;
		sty->vf[2] += tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if('u' == tmp)
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
	else if('o' == tmp)
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
	return 0;
}
int playwith3d_pick(struct arena* win, struct relation* top, int x, int y)
{
	int ret;
	vec3 ray[2];
	vec3 out[2];
	struct style* sty = 0;
	struct relation* rel = top;

	ray[0][0] = win->camera.vc[0];
	ray[0][1] = win->camera.vc[1];
	ray[0][2] = win->camera.vc[2];
	ray[1][0] = (float)(x*2) / (win->width) - 1.0;
	ray[1][1] = 1.0 - (float)(y*2) / (win->height);
	ray[1][2] = 0.0;

	invmvp(ray[1], win);
	ray[1][0] -= ray[0][0];
	ray[1][1] -= ray[0][1];
	ray[1][2] -= ray[0][2];

	while(1)
	{
		if(rel == 0)break;
		sty = (void*)(rel->dstfoot);

		ret = ray_obb(ray, sty, out);
		say("ret=%d\n", ret);
		if(ret > 0)break;

		rel = samedstprevsrc(rel);
		sty = 0;
	}
	if(sty != 0)relation_swap(top, rel);
	return 0;
}
int playwith3d(struct arena* win, struct event* ev)
{
	float c,s,tx,ty,norm;
	struct relation* rel;
	struct relation* tmp;
	struct style* sty;
	int ax, ay, aaa, bbb;
	int x = (ev->why)&0xffff;
	int y = ((ev->why)>>16)&0xffff;
	int z = ((ev->why)>>32)&0xffff;
	int id = ((ev->why)>>48)&0xffff;

	rel = win->irel0;
	if(rel == 0)return 1;
	while(1)
	{
		tmp = samedstnextsrc(rel);
		if(0 == tmp)break;

		rel = tmp;
	}
	sty = (void*)(rel->dstfoot);

	if(_char_ == ev->what)
	{
		if(8 == ev->why)relationdelete(rel);
		else keyboard2style(sty, (ev->why)&0xff);
		return 0;
	}
	if(_joy_ == ev->what)
	{
		joystick2style(sty, (void*)ev);
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
		playwith3d_pick(win, rel, x, y);
		return 0;
	}
	if(hex32('p','@',0,0) == ev->what)
	{
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;
		if(0 == win->input[id].z0)return 0;

		if(	(0 != win->input[0].z0)&&
			(0 != win->input[1].z0) )
		{
			if(0 == id)
			{
				x -= (win->input[1].x1);
				y -= (win->input[1].y1);
			}
			if(1 == id)
			{
				x -= (win->input[0].x1);
				y -= (win->input[0].y1);
			}

			ax = (win->input[0].x1) - (win->input[1].x1);
			ay = (win->input[0].y1) - (win->input[1].y1);

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

			ax = x - (win->input[id].x1);
			ay = y - (win->input[id].y1);

			sty->vc[0] += ty*ax - tx*ay;
			sty->vc[1] -= tx*ax + ty*ay;
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