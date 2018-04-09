#include "actor.h"
int relation_swap(void*, void*);
void quaternionoperation(float*, float*, float);




void camera_deltax(struct arena* win, float delta)
{
	float c,s;
	float x,y,z;
	float cx,cy,cz;		//camera
	float tx,ty,tz;		//target
	float vx,vy,vz;		//vector(tar to cam)

	//camera
	cx = win->cx;
	cy = win->cy;
	cz = win->cz;

	//vector = -front
	vx = -(win->fx);
	vy = -(win->fy);
	vz = -(win->fz);

	//target = camera-front
	tx = cx-vx;
	ty = cy-vy;
	tz = cz-vz;

	c = cosine(delta);
	s = sine(delta);

	//rotate
	x = vx*c + vy*s;
	y = -vx*s + vy*c;
	z = vz;

	//camera = target+vector
	win->cx = tx+x;
	win->cy = ty+y;
	win->cz = tz+z;

	//front = -vector
	win->fx = -x;
	win->fy = -y;
	win->fz = -z;
}
void camera_deltay(struct arena* win, float delta)
{
	float v[4];
	float q[4];
	float cx,cy,cz;		//camera
	float tx,ty,tz;		//target

	//camera
	cx = win->cx;
	cy = win->cy;
	cz = win->cz;

	//vector = -front
	v[0] = -(win->fx);
	v[1] = -(win->fy);
	v[2] = -(win->fz);

	//target = camera-front
	tx = cx-v[0];
	ty = cy-v[1];
	tz = cz-v[2];

	//right = cross(front, (0,0,1))
	q[0] = v[1]*1 - v[2]*0;
	q[1] = v[2]*0 - v[0]*1;
	q[2] = v[0]*0 - v[1]*0;
	quaternionoperation(v, q, delta);

	//camera = target+vector
	win->cx = tx+v[0];
	win->cy = ty+v[1];
	win->cz = tz+v[2];

	//front = -vector
	win->fx = -v[0];
	win->fy = -v[1];
	win->fz = -v[2];
}
void camera_deltaxy(struct arena* win, int dx, int dy)
{
	float delta;

	if(dx > 0)delta = 0.02;
	else delta = -0.02;
	camera_deltax(win, delta);

	if(dy > 0)delta = 0.02;
	else delta = -0.02;
	camera_deltay(win, delta);
}
void camera_zoom(struct arena* win, float delta)
{
	float x = delta*(win->fx);
	float y = delta*(win->fy);
	float z = delta*(win->fz);

	win->cx += x;
	win->cy += y;
	win->cz += z;

	win->fx -= x;
	win->fy -= y;
	win->fz -= z;
}
int camera_event(struct arena* win, struct event* ev)
{
	float x,y,z;
	int x0,y0,x1,y1,btn;

	btn = (ev->why)>>48;
	if(0x4070 == ev->what)
	{
		if(btn > 10)btn = 10;
		if(0 == win->touchdown[btn].z)return 0;

		if((0 != win->touchdown[0].z)&&(0 != win->touchdown[1].z))
		{
			x1 = (ev->why)&0xffff;
			y1 = ((ev->why)>>16)&0xffff;
			if(0 == btn)
			{
				x1 -= (win->touchmove[1].x);
				y1 -= (win->touchmove[1].y);
			}
			if(1 == btn)
			{
				x1 -= (win->touchmove[0].x);
				y1 -= (win->touchmove[0].y);
			}

			x0 = (win->touchmove[0].x) - (win->touchmove[1].x);
			y0 = (win->touchmove[0].y) - (win->touchmove[1].y);

			if((x0*x0+y0*y0) < (x1*x1+y1*y1))camera_zoom(win, 0.05);
			else camera_zoom(win, -0.05);
		}
		else if((0 == btn)|(10 == btn))
		{
			x0 = win->touchmove[btn].x;
			y0 = win->touchmove[btn].y;
			x1 = (ev->why)&0xffff;
			y1 = ((ev->why)>>16)&0xffff;
			camera_deltaxy(win, x1-x0, y1-y0);
		}
	}
	else if(0x2b70 == ev->what)
	{
		if('f' == btn)camera_zoom(win, 0.1);
		if('b' == btn)camera_zoom(win, -0.1);
	}
	return 0;
}




int playwith3d(struct arena* win, struct event* ev)
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

	if('f' == btn)
	{
		stytop->rx = (stytop->rx)*17/16;
		stytop->fy = (stytop->fy)*17/16;
		stytop->uz = (stytop->uz)*17/16;
		return 0;
	}
	else if('b' == btn)
	{
		stytop->rx = (stytop->rx)*15/16;
		stytop->fy = (stytop->fy)*15/16;
		stytop->uz = (stytop->uz)*15/16;
		return 0;
	}

	if(hex32('p','@',0,0) == ev->what)
	{
		if(btn > 10)btn = 10;
		if(0 != win->touchdown[btn].z)
		{
			stytop->cx += x - (win->touchmove[btn].x);
			stytop->cy -= y - (win->touchmove[btn].y);
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
	else if(hex32('p','+',0,0) == ev->what)
	{
		if(stywow != 0)relation_swap(reltop, relwow);
	}
	return 1;
}