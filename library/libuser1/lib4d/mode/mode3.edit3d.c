#include "actor.h"
int relation_swap(void*, void*);




int joystick2style(struct style* sty, short* tmp)
{
	int x0,y0;
	float c,s;
	float tx,ty,tz;

	if(_dl_ == tmp[2])
	{
		//dpad left
		tx = (sty->rx)/16;
		ty = (sty->ry)/16;
		tz = (sty->rz)/16;

		sty->rx -= tx;
		sty->ry -= ty;
		sty->rz -= tz;

		sty->cx += tx;
		sty->cy += ty;
		sty->cz += tz;
		return 0;
	}
	else if(_dr_ == tmp[2])
	{
		//dpad right
		tx = (sty->rx)/16;
		ty = (sty->ry)/16;
		tz = (sty->rz)/16;

		sty->rx -= tx;
		sty->ry -= ty;
		sty->rz -= tz;

		sty->cx -= tx;
		sty->cy -= ty;
		sty->cz -= tz;
		return 0;
	}
	else if(_dn_ == tmp[2])
	{
		//dpad down
		tx = (sty->fx)/16;
		ty = (sty->fy)/16;
		tz = (sty->fz)/16;

		sty->fx -= tx;
		sty->fy -= ty;
		sty->fz -= tz;

		sty->cx += tx;
		sty->cy += ty;
		sty->cz += tz;
		return 0;
	}
	else if(_df_ == tmp[2])
	{
		//dpad up
		tx = (sty->fx)/16;
		ty = (sty->fy)/16;
		tz = (sty->fz)/16;

		sty->fx -= tx;
		sty->fy -= ty;
		sty->fz -= tz;

		sty->cx -= tx;
		sty->cy -= ty;
		sty->cz -= tz;
		return 0;
	}
	else if(_lt_ == tmp[2])
	{
		//left trigger
		tx = (sty->ux)/16;
		ty = (sty->uy)/16;
		tz = (sty->uz)/16;

		sty->ux -= tx;
		sty->uy -= ty;
		sty->uz -= tz;

		sty->cx += tx*2;
		sty->cy += ty*2;
		sty->cz += tz*2;
		return 0;
	}
	else if(_lb_ == tmp[2])
	{
		//left bumper
		tx = (sty->ux)/16;
		ty = (sty->uy)/16;
		tz = (sty->uz)/16;

		sty->ux -= tx;
		sty->uy -= ty;
		sty->uz -= tz;
		return 0;
	}
	else if(_kx_ == tmp[2])
	{
		//key x
		tx = (sty->rx)/16;
		ty = (sty->ry)/16;
		tz = (sty->rz)/16;

		sty->rx += tx;
		sty->ry += ty;
		sty->rz += tz;

		sty->cx -= tx;
		sty->cy -= ty;
		sty->cz -= tz;
		return 0;
	}
	else if(_kb_ == tmp[2])
	{
		//key b
		tx = (sty->rx)/16;
		ty = (sty->ry)/16;
		tz = (sty->rz)/16;

		sty->rx += tx;
		sty->ry += ty;
		sty->rz += tz;

		sty->cx += tx;
		sty->cy += ty;
		sty->cz += tz;
		return 0;
	}
	else if(_ka_ == tmp[2])
	{
		//key a
		tx = (sty->fx)/16;
		ty = (sty->fy)/16;
		tz = (sty->fz)/16;

		sty->fx += tx;
		sty->fy += ty;
		sty->fz += tz;

		sty->cx -= tx;
		sty->cy -= ty;
		sty->cz -= tz;
		return 0;
	}
	else if(_ky_ == tmp[2])
	{
		//key y
		tx = (sty->fx)/16;
		ty = (sty->fy)/16;
		tz = (sty->fz)/16;

		sty->fx += tx;
		sty->fy += ty;
		sty->fz += tz;

		sty->cx += tx;
		sty->cy += ty;
		sty->cz += tz;
		return 0;
	}
	else if(_rt_ == tmp[2])
	{
		//right trigger
		tx = (sty->ux)/16;
		ty = (sty->uy)/16;
		tz = (sty->uz)/16;

		sty->ux += tx;
		sty->uy += ty;
		sty->uz += tz;

		sty->cx -= tx*2;
		sty->cy -= ty*2;
		sty->cz -= tz*2;
		return 0;
	}
	else if(_rb_ == tmp[2])
	{
		//right bumper
		tx = (sty->ux)/16;
		ty = (sty->uy)/16;
		tz = (sty->uz)/16;

		sty->ux += tx;
		sty->uy += ty;
		sty->uz += tz;
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
		sty->cx += (sty->rx)*x0/16 + (sty->fx)*y0/16;
		sty->cy += (sty->ry)*x0/16 + (sty->fy)*y0/16;
		sty->cz += (sty->rz)*x0/16 + (sty->fz)*y0/16;
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

		tx = sty->rx;
		ty = sty->ry;
		sty->rx = tx*c - ty*s;
		sty->ry = tx*s + ty*c;

		tx = sty->fx;
		ty = sty->fy;
		sty->fx = tx*c - ty*s;
		sty->fy = tx*s + ty*c;
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
		tx = (sty->rx)/16;
		ty = (sty->ry)/16;
		tz = (sty->rz)/16;

		sty->rx -= tx;
		sty->ry -= ty;
		sty->rz -= tz;

		sty->cx += tx;
		sty->cy += ty;
		sty->cz += tz;
		return 0;
	}
	else if('f' == tmp)
	{
		//dpad right
		tx = (sty->rx)/16;
		ty = (sty->ry)/16;
		tz = (sty->rz)/16;

		sty->rx -= tx;
		sty->ry -= ty;
		sty->rz -= tz;

		sty->cx -= tx;
		sty->cy -= ty;
		sty->cz -= tz;
		return 0;
	}
	else if('d' == tmp)
	{
		//dpad down
		tx = (sty->fx)/16;
		ty = (sty->fy)/16;
		tz = (sty->fz)/16;

		sty->fx -= tx;
		sty->fy -= ty;
		sty->fz -= tz;

		sty->cx += tx;
		sty->cy += ty;
		sty->cz += tz;
		return 0;
	}
	else if('e' == tmp)
	{
		//dpad up
		tx = (sty->fx)/16;
		ty = (sty->fy)/16;
		tz = (sty->fz)/16;

		sty->fx -= tx;
		sty->fy -= ty;
		sty->fz -= tz;

		sty->cx -= tx;
		sty->cy -= ty;
		sty->cz -= tz;
		return 0;
	}
	else if('w' == tmp)
	{
		//left trigger
		tx = (sty->ux)/16;
		ty = (sty->uy)/16;
		tz = (sty->uz)/16;

		sty->ux -= tx;
		sty->uy -= ty;
		sty->uz -= tz;

		sty->cx += tx*2;
		sty->cy += ty*2;
		sty->cz += tz*2;
		return 0;
	}
	else if('r' == tmp)
	{
		//left bumper
		tx = (sty->ux)/16;
		ty = (sty->uy)/16;
		tz = (sty->uz)/16;

		sty->ux -= tx;
		sty->uy -= ty;
		sty->uz -= tz;
		return 0;
	}
	else if('j' == tmp)
	{
		//key x
		tx = (sty->rx)/16;
		ty = (sty->ry)/16;
		tz = (sty->rz)/16;

		sty->rx += tx;
		sty->ry += ty;
		sty->rz += tz;

		sty->cx -= tx;
		sty->cy -= ty;
		sty->cz -= tz;
		return 0;
	}
	else if('l' == tmp)
	{
		//key b
		tx = (sty->rx)/16;
		ty = (sty->ry)/16;
		tz = (sty->rz)/16;

		sty->rx += tx;
		sty->ry += ty;
		sty->rz += tz;

		sty->cx += tx;
		sty->cy += ty;
		sty->cz += tz;
		return 0;
	}
	else if('k' == tmp)
	{
		//key a
		tx = (sty->fx)/16;
		ty = (sty->fy)/16;
		tz = (sty->fz)/16;

		sty->fx += tx;
		sty->fy += ty;
		sty->fz += tz;

		sty->cx -= tx;
		sty->cy -= ty;
		sty->cz -= tz;
		return 0;
	}
	else if('i' == tmp)
	{
		//key y
		tx = (sty->fx)/16;
		ty = (sty->fy)/16;
		tz = (sty->fz)/16;

		sty->fx += tx;
		sty->fy += ty;
		sty->fz += tz;

		sty->cx += tx;
		sty->cy += ty;
		sty->cz += tz;
		return 0;
	}
	else if('u' == tmp)
	{
		//right trigger
		tx = (sty->ux)/16;
		ty = (sty->uy)/16;
		tz = (sty->uz)/16;

		sty->ux += tx;
		sty->uy += ty;
		sty->uz += tz;

		sty->cx -= tx*2;
		sty->cy -= ty*2;
		sty->cz -= tz*2;
		return 0;
	}
	else if('o' == tmp)
	{
		//right bumper
		tx = (sty->ux)/16;
		ty = (sty->uy)/16;
		tz = (sty->uz)/16;

		sty->ux += tx;
		sty->uy += ty;
		sty->uz += tz;
		return 0;
	}
	return 0;
}
int playwith3d(struct arena* win, struct event* ev)
{
	float c,s,tx,ty;
	struct relation* reltop;
	struct relation* relwow;
	struct style* stytop;
	struct style* stywow;
	int absx, absy, aaa, bbb;
	int x = (ev->why)&0xffff;
	int y = ((ev->why)>>16)&0xffff;
	int z = ((ev->why)>>32)&0xffff;
	int id = ((ev->why)>>48)&0xffff;

	reltop = win->irel;
	if(reltop == 0)return 1;
	while(1)
	{
		relwow = samedstnextsrc(reltop);
		if(relwow == 0)break;

		reltop = relwow;
	}
	stytop = (void*)(reltop->dstfoot);

	stywow = 0;
	relwow = reltop;
	while(1)
	{
		if(relwow == 0)break;
		stywow = (void*)(relwow->dstfoot);

		if(x > stywow->cx)absx = x - (stywow->cx);
		else absx = (stywow->cx) - x;
		if(y > stywow->cy)absy = y - (stywow->cy);
		else absy = (stywow->cy) - y;

		if((absx <= stywow->rx)&&(absy <= stywow->fy))break;

		relwow = samedstprevsrc(relwow);
		stywow = 0;
	}
	//say("%x,%x,%x,%x\n",reltop,stytop,stytop,stywow);
	//printmemory(ev,0x20);

	if(_char_ == ev->what)
	{
		if(8 == ev->why)relationdelete(reltop);
		else keyboard2style(stytop, (ev->why)&0xff);
		return 0;
	}
	if(_joy_ == ev->what)
	{
		joystick2style(stytop, (void*)ev);
		return 0;
	}
	if('f' == id)
	{
		stytop->rx = (stytop->rx)*17/16;
		stytop->ry = (stytop->ry)*17/16;
		stytop->rz = (stytop->rz)*17/16;

		stytop->fx = (stytop->fx)*17/16;
		stytop->fy = (stytop->fy)*17/16;
		stytop->fz = (stytop->fz)*17/16;

		stytop->ux = (stytop->ux)*17/16;
		stytop->uy = (stytop->uy)*17/16;
		stytop->uz = (stytop->uz)*17/16;
		return 0;
	}
	if('b' == id)
	{
		stytop->rx = (stytop->rx)*15/16;
		stytop->ry = (stytop->ry)*15/16;
		stytop->rz = (stytop->rz)*15/16;

		stytop->fx = (stytop->fx)*15/16;
		stytop->fy = (stytop->fy)*15/16;
		stytop->fz = (stytop->fz)*15/16;

		stytop->ux = (stytop->ux)*15/16;
		stytop->uy = (stytop->uy)*15/16;
		stytop->uz = (stytop->uz)*15/16;
		return 0;
	}
	if(hex32('p','@',0,0) == ev->what)
	{
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;

		if(1 >= id)
		{
			if(0==win->touchdown[0].z)return 0;
			if(0==win->touchdown[1].z)return 0;

			if(0 == id)
			{
				x -= (win->touchmove[1].x);
				y -= (win->touchmove[1].y);
			}
			if(1 == id)
			{
				x -= (win->touchmove[0].x);
				y -= (win->touchmove[0].y);
			}

			absx = (win->touchmove[0].x) - (win->touchmove[1].x);
			absy = (win->touchmove[0].y) - (win->touchmove[1].y);

			aaa = x*x+y*y;
			bbb = absx*absx + absy*absy;

			stytop->rx = (stytop->rx) * aaa / bbb;
			stytop->ry = (stytop->ry) * aaa / bbb;
			stytop->rz = (stytop->rz) * aaa / bbb;

			stytop->fx = (stytop->fx) * aaa / bbb;
			stytop->fy = (stytop->fy) * aaa / bbb;
			stytop->fz = (stytop->fz) * aaa / bbb;

			stytop->ux = (stytop->ux) * aaa / bbb;
			stytop->uy = (stytop->uy) * aaa / bbb;
			stytop->uz = (stytop->uz) * aaa / bbb;
		}
		else if(10 == id)
		{
			stytop->cx += x - (win->touchmove[id].x);
			stytop->cy -= y - (win->touchmove[id].y);
			//say("%x,%x\n", stytop->cx, stytop->cy);
		}
		else if(11 == id)
		{
			if(x > win->touchdown[id].x)
			{
				c = cosine(0.05);
				s = sine(0.05);
			}
			else if(x < win->touchdown[id].x)
			{
				c = cosine(-0.05);
				s = sine(-0.05);
			}
			else return 0;

			tx = stytop->rx;
			ty = stytop->ry;
			stytop->rx = tx*c - ty*s;
			stytop->ry = tx*s + ty*c;

			tx = stytop->fx;
			ty = stytop->fy;
			stytop->fx = tx*c - ty*s;
			stytop->fy = tx*s + ty*c;
		}
	}
	else if(hex32('p','+',0,0) == ev->what)
	{
		if(stywow != 0)relation_swap(reltop, relwow);
	}
	return 1;
}