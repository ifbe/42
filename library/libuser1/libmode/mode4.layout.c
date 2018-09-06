#include "libuser.h"
int relation_choose(void*, void*);
int invmvp(vec3 v, struct arena* win);
int ray_obb(vec3 ray[], struct style* sty, vec3 out[]);




void select_2d(struct arena* win, struct style* sty)
{
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];

	drawline_rect(win, 0xff00ff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
}
void select_3d(struct arena* win, struct style* sty)
{
	vec3 tc;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	tc[0] = vc[0] + vu[0];
	tc[1] = vc[1] + vu[1];
	tc[2] = vc[2] + vu[2];
	carveline_prism4(win, 0xff0000, tc, vr, vf, vu);
}




void actoroutput_panel3d(struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf)
{
	float j;
	vec3 ta;
	vec3 tb;
	carvesolid2d_rect(win, rgb, vc, vr, vf);

	for(j=-0.75;j<1.0;j+=0.25)
	{
		ta[0] = vc[0] + j*vr[0] - 0.8*vf[0];
		ta[1] = vc[1] + j*vr[1] - 0.8*vf[1];
		ta[2] = vc[2] + j*vr[2] - 0.8*vf[2];
		tb[0] = vc[0] + j*vr[0] + 0.8*vf[0];
		tb[1] = vc[1] + j*vr[1] + 0.8*vf[1];
		tb[2] = vc[2] + j*vr[2] + 0.8*vf[2];
		carveline2d(win, 0xffffff, ta, tb);
	}
}
void actoroutput_layout_vbo(struct arena* win, struct style* st)
{
	vec3 vc, vr, vf;
	carveaxis(win);

	//right
	vc[0] = 1.0-0.05;
	vc[1] = 0.0;
	vc[2] = -0.8;
	vr[0] = 0.05;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 0.75;
	vf[2] = 0.0;
	actoroutput_panel3d(win, 0x400000, vc, vf, vr);

	//left
	vc[0] = -vc[0];
	actoroutput_panel3d(win, 0x000040, vc, vf, vr);

	//upper
	vc[0] = 0.0;
	vc[1] = 1.0-0.05;
	vr[0] = 0.75;
	vf[1] = 0.05;
	actoroutput_panel3d(win, 0x404000, vc, vr, vf);

	//bottom
	vc[1] = -vc[1];
	actoroutput_panel3d(win, 0x004040, vc, vr, vf);
}
void actoroutput_layout_pixel(struct arena* win, struct style* st)
{
	int w = win->width;
	int h = win->height;
	drawaxis(win);

	//left
	drawsolid_rect(win, 0x000040, 0, h/8, w/20, h*7/8);

	//right
	drawsolid_rect(win, 0x400000, w*19/20, h/8, w, h*7/8);

	//upper
	drawsolid_rect(win, 0x404000, w/8, 0, w*7/8, h/20);

	//bottom
	drawsolid_rect(win, 0x004040, w/8, h*19/20, w*7/8, h);
}
int actoroutput_layout(struct arena* win, struct style* st)
{
	struct relation* orel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
	u64 fmt = win->fmt;

	orel = win->orel0;
	while(1)
	{
		if(0 == orel)break;

		if(_act_ == orel->dsttype)
		{
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			sty = (void*)(orel->srcfoot);

			//say("%x,%x,%x,%x\n", win, act, sty, pin);
			act->onread(win, sty, act, pin);

			if(_vbo_ == fmt)select_3d(win, sty);
			else select_2d(win, sty);
		}

		orel = samesrcnextdst(orel);
	}

	if(_vbo_ == fmt)actoroutput_layout_vbo(win, st);
	else actoroutput_layout_pixel(win, st);
	return 0;
}




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
int playwith3d_pick(struct arena* win, int x, int y)
{
	int ret;
	vec3 ray[2];
	vec3 out[2];
	struct style* sty;
	struct relation* rel;

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

	rel = win->ireln;
	while(1)
	{
		sty = 0;
		if(rel == 0)break;
		sty = (void*)(rel->srcfoot);

		ret = ray_obb(ray, sty, out);
		say("rel=%llx, ret=%d\n", rel, ret);
		if(ret > 0)break;

		rel = samesrcprevdst(rel);
	}
	if(rel)relation_choose(win, rel);
	return 0;
}
int actorinput_layout(struct arena* win, struct event* ev)
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
		playwith3d_pick(win, x, y);
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