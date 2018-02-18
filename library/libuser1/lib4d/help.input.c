#include "actor.h"
int term_write(void*);
int login_write(void*, void*);
int relation_swap(void*, void*);
int relation_destory(void*);
void* samepinprevchip(void*);
void* samepinnextchip(void*);
void* samechipprevpin(void*);
void* samechipnextpin(void*);
void* relation_read(u64);




//touch
static u64 pointenter[10];
static u64 pointleave[10];
static int pointcount=0;
static int pointmax=0;




//why, what, where, when
void motion_explain(u64* p)
{
	int m,n;
	int dx0,dy0;
	int dx1,dy1;
	int dx2,dy2;

	n = (p[0] >> 48) & 0xffff;
	if(n>0x10)return;

	m = p[1]&0xffff;
	if(m == 0x4070)
	{
		//say("[%d]@@@@@@@@@@@\n",n);
		pointleave[n] = p[0];
	}//point move

	else if(m == 0x2b70)
	{
		//say("[%d]++++++++++\n",n);
		pointcount++;
		pointmax++;
		pointenter[n] = p[0];
	}//point sensed

	else if(m == 0x2d70)
	{
		//say("[%d]---------\n",n);

		//
		pointleave[n] = p[0];
/*
		say("(%llx,%llx)->(%llx,%llx)\n",
			(pointenter[n]&0xffff),
			((pointenter[n]>>16)&0xffff),
			(pointleave[n]&0xffff),
			((pointleave[n]>>16)&0xffff)
		);
*/
		//
		pointcount--;
		if(pointcount==0)
		{
			if(pointmax==1)
			{
				pointmax=0;
				dx0 = (pointleave[0]&0xffff) - (pointenter[0]&0xffff);
				dy0 = ((pointleave[0]>>16)&0xffff) - ((pointenter[0]>>16)&0xffff);

				if( (dy0 > -0x2000) && (dy0 < 0x2000) )
				{
					if(dx0 < -0x2000)    //left
					{
						p[1] = 0x64626b;
						p[0] = 0x25;
					}
					else if(dx0 > 0x2000)	//right
					{
						p[1] = 0x64626b;
						p[0] = 0x27;
					}
					else    //point
					{
						p[1] = 0x2d6d;
						p[0] = (p[0]&0xffffffff) + ((u64)1<<48);
					}
				}
				if( (dx0 > -0x2000) && (dx0 < 0x2000) )
				{
					if(dy0 < -0x2000)    //up
					{
						p[1] = 0x64626b;
						p[0] = 0x26;
					}
					else if(dy0 > 0x2000)	//down
					{
						p[1] = 0x64626b;
						p[0] = 0x28;
					}
					else    //point
					{
						p[1] = 0x2d70;
						p[0] = (p[0]&0xffffffff) + ((u64)1<<48);
					}
				}
			}

			else if(pointmax==2)
			{
				pointmax=0;
				dx0 = (pointleave[0]&0xffff) - (pointenter[0]&0xffff);
				dy0 = ((pointleave[0]>>16)&0xffff) - ((pointenter[0]>>16)&0xffff);
				dx1 = (pointleave[1]&0xffff) - (pointenter[1]&0xffff);
				dy1 = ((pointleave[1]>>16)&0xffff) - ((pointenter[1]>>16)&0xffff);
			}

			else if(pointmax>=3)
			{
				pointmax=0;
				dx0 = (pointleave[0]&0xffff) - (pointenter[0]&0xffff);
				dy0 = ((pointleave[0]>>16)&0xffff) - ((pointenter[0]>>16)&0xffff);
				dx1 = (pointleave[1]&0xffff) - (pointenter[1]&0xffff);
				dy1 = ((pointleave[1]>>16)&0xffff) - ((pointenter[1]>>16)&0xffff);
				dx2 = (pointleave[2]&0xffff) - (pointenter[2]&0xffff);
				dy2 = ((pointleave[2]>>16)&0xffff) - ((pointenter[2]>>16)&0xffff);
				if( (dx0 > -0x2000)&&(dx0 < 0x2000)&&
					(dx1 > -0x2000)&&(dx1 < 0x2000)&&
					(dx2 > -0x2000)&&(dx2 < 0x2000) )
				{
					if( (dy0 > 0x2000)&&(dy1 > 0x2000)&&(dy2 > 0x2000) )
					{
						//worker[1].xyze1 ^= 1;
					}
					else if( (dy0 < -0x2000)&&(dy1 < -0x2000)&&(dy2 < -0x2000) )
					{
						//worker[2].xyze1 ^= 1;
					}
					else
					{
						p[1] = 0x64626b;
						p[0] = 0x1b;
					}
				}//3touch
			}//max>=3
		}//last one
	}//point gone
}
int point_explain(struct arena* win, struct event* ev)
{
	int btn, absx, absy;
	struct relation* reltop;
	struct relation* relwow;
	struct style* stytop;
	struct style* stywow;
	struct point* p;
	struct point* q;

	reltop = win->irel;
	if(reltop == 0)return 1;
	while(1)
	{
		relwow = samepinnextchip(reltop);
		if(relwow == 0)break;

		reltop = relwow;
	}
	stytop = (void*)(reltop->destfoot);

	stywow = 0;
	p = (void*)ev;
	relwow = reltop;
	while(1)
	{
		if(relwow == 0)break;
		stywow = (void*)(relwow->destfoot);

		if(p->x > stywow->i_cx)absx = (p->x) - (stywow->i_cx);
		else absx = (stywow->i_cx) - (p->x);
		if(p->y > stywow->i_cy)absy = (p->y) - (stywow->i_cy);
		else absy = (stywow->i_cy) - (p->y);

		if((absx*2 <= stywow->i_rx)&&(absy*2 <= stywow->i_fy))break;

		relwow = samepinprevchip(relwow);
		stywow = 0;
	}
	//say("%x,%x,%x,%x\n",reltop,stytop,stytop,stywow);

	btn = (ev->why) >> 48;
	if(btn == 'f')
	{
		stytop->i_rx = (stytop->i_rx)*15/16;
		stytop->i_fy = (stytop->i_fy)*15/16;
		stytop->i_uz = (stytop->i_uz)*15/16;
		return 0;
	}
	else if(btn == 'b')
	{
		stytop->i_rx = (stytop->i_rx)*17/16;
		stytop->i_fy = (stytop->i_fy)*17/16;
		stytop->i_uz = (stytop->i_uz)*17/16;
		return 0;
	}

	if(btn == 0x6c)btn = 10;
	if(btn > 10)return 0;
	q = (void*)&(win->touch[btn]);

	if(ev->what == hex32('p','+',0,0))
	{
		q->x = p->x;
		q->y = p->y;
		if(stywow != 0)relation_swap(reltop, relwow);
	}
	if(ev->what == hex32('p','@',0,0))
	{
		stytop->i_cx += (int)(p->x) - (int)(q->x);
		stytop->i_cy += (int)(p->y) - (int)(q->y);
		//say("%x,%x\n", stytop->i_cx, stytop->i_cy);

		q->x = p->x;
		q->y = p->y;
	}
	return 1;
}
int delete_thisact(struct arena* win)
{
	struct relation* rel;
	struct relation* tmp;
	if(win == 0)return 0;
	if(win->irel == 0)return 0;

	rel = win->irel;
	while(1)
	{
		tmp = samepinnextchip(rel);
		if(tmp == 0)break;

		rel = tmp;
	}
	relation_destory(rel);
	return 1;
}
int input_write(struct arena* win, struct event* ev)
{
	int ret;

	//no actor
	if(win->irel == 0)
	{
		if(win->fmt == hex32('c','l','i',0))
		{
			term_write(ev);
		}
		else
		{
			login_write(win, ev);
		}
		return 0;
	}

	//f11, f12
	if(ev->what == _kbd_)
	{
		ret=0;
		if(ev->why == 0xfb)ret = 11;
		else if(ev->why == 0xfc)ret = 12;

		if(ret != 0)
		{
			if(ret == win->cw)win->cw = 0;
			else win->cw = ret;
			return 0;
		}
	}

	//build
	if(win->cw == 11)
	{
		login_write(win, ev);
		return 0;
	}

	//chosen
	if(win->cw == 12)
	{
		if(ev->what == _char_)
		{
			delete_thisact(win);
			return 0;
		}
		else
		{
			ret = point_explain(win, ev);
			if(ret != 0)return 0;
		}
	}

	return 1;
}
