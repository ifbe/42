#include "actor.h"
int relation_swap(void*, void*);




void select_2d(struct arena* win, struct style* sty)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	drawline_rect(win, 0xff00ff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
}
void select_3d(struct arena* win, struct style* sty)
{
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float rx = sty->rx;
	float ry = sty->ry;
	float rz = sty->rz;
	float fx = sty->fx;
	float fy = sty->fy;
	float fz = sty->fz;
	float ux = sty->ux;
	float uy = sty->uy;
	float uz = sty->uz;

	carveline_prism4(
		win, 0xff0000,
		cx+ux, cy+uy, cz+uz,
		rx, ry, rz,
		fx, fy, fz,
		ux, uy, uz
	);
}
int actoroutput_edit(struct arena* win)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct compo* com;

	rel = win->irel;
	while(1)
	{
		if(rel == 0)break;

		if(rel->srctype == _act_)
		{
			act = (void*)(rel->srcchip);
			sty = (void*)(rel->dstfoot);
			com = (void*)(rel->srcfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, com);
			//say("%x\n", rel);

			act->onread(win, sty, act, com);

			if(win->fmt == _vbo_)select_3d(win, sty);
			else select_2d(win, sty);
		}

		rel = samedstnextsrc(rel);
	}
	return 0;
}




int joystick2style_2d(struct style* sty, short* tmp)
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
	else if(_df_ == tmp[2])
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
	else if(_dn_ == tmp[2])
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
	else if(_ky_ == tmp[2])
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
	else if(_ka_ == tmp[2])
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
		sty->cx += (sty->rx)*x0/16 - (sty->fx)*y0/16;
		sty->cy += (sty->ry)*x0/16 - (sty->fy)*y0/16;
		sty->cz += (sty->rz)*x0/16 - (sty->fz)*y0/16;
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
int keyboard2style_2d(struct style* sty, u8 tmp)
{
	int x0,y0;
	float c,s;
	float tx,ty,tz;

	if('s' == tmp)
	{
		//left
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
		//right
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
	else if('e' == tmp)
	{
		//up
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
	else if('d' == tmp)
	{
		//down
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
	else if('i' == tmp)
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
	else if('k' == tmp)
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
int playwith2d(struct arena* win, struct event* ev)
{
	struct relation* reltop;
	struct relation* relwow;
	struct style* stytop;
	struct style* stywow;
	int absx, absy;
	int x = (ev->why)&0xffff;
	int y = ((ev->why)>>16)&0xffff;
	int z = ((ev->why)>>32)&0xffff;
	int btn = ((ev->why)>>48)&0xffff;

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

	if(_char_ == ev->what)
	{
		if(8 == ev->why)relationdelete(reltop);
		else keyboard2style_2d(stytop, (ev->why)&0xff);
		return 0;
	}
	if(_joy_ == ev->what)
	{
		joystick2style_2d(stytop, (void*)ev);
		return 0;
	}
	if('f' == btn)
	{
		stytop->rx = (stytop->rx)*17/16;
		stytop->fy = (stytop->fy)*17/16;
		stytop->uz = (stytop->uz)*17/16;
		return 0;
	}
	if('b' == btn)
	{
		stytop->rx = (stytop->rx)*15/16;
		stytop->fy = (stytop->fy)*15/16;
		stytop->uz = (stytop->uz)*15/16;
		return 0;
	}
	if(hex32('p','+',0,0) == ev->what)
	{
		if(stywow != 0)relation_swap(reltop, relwow);
		return 0;
	}
	if(hex32('p','@',0,0) == ev->what)
	{
		if(btn > 10)btn = 10;
		if(0 != win->touchdown[btn].z)
		{
			stytop->cx += x - (win->touchmove[btn].x);
			stytop->cy += y - (win->touchmove[btn].y);
			//say("%x,%x\n", stytop->cx, stytop->cy);
		}
		if(1 >= btn)
		{
			if(0==win->touchdown[0].z)return 0;
			if(0==win->touchdown[1].z)return 0;

			if(0 == btn)
			{
				x -= (win->touchmove[1].x);
				y -= (win->touchmove[1].y);
			}
			if(1 == btn)
			{
				x -= (win->touchmove[0].x);
				y -= (win->touchmove[0].y);
			}

			absx = (win->touchmove[0].x) - (win->touchmove[1].x);
			absy = (win->touchmove[0].y) - (win->touchmove[1].y);
			stytop->rx = (stytop->rx) * (x*x+y*y) / (absx*absx+absy*absy);
			stytop->fy = (stytop->fy) * (x*x+y*y) / (absx*absx+absy*absy);
			stytop->uz = (stytop->uz) * (x*x+y*y) / (absx*absx+absy*absy);
		}
	}
	return 1;
}