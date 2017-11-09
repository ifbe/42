#include "actor.h"
void* connect_read(u64);
int connect_write(void* uchip, void* ufoot, u64 utype, void* bchip, u64 bfoot, u64 btype);




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
int keyboard_explain(struct arena* win, struct event* ev)
{
	int j = 0;
	if(ev->what == hex32('k','b','d',0))
	{
		if(ev->why == 0x1b)j = 4;
		else if(ev->why == 0xf1)j = 1;
		else if(ev->why == 0xf2)j = 2;
		else if(ev->why == 0xf3)j = 3;
	}
	if(j != 0)
	{
		if(j == win->cw)win->cw = 0;
		else win->cw = j;
	}
	return j;
}
int mouse_explain(struct arena* win, struct style* sty, struct event* ev)
{
	struct point* p;
	struct point* q;
	if((ev->why>>48) != 0x6c)return 0;

	if(ev->what == hex32('p','+',0,0))
	{
		*(u64*)&(win->touch[10]) = ev->why;
	}
	else if(ev->what == hex32('p','@',0,0))
	{
		q = (void*)&(win->touch[10]);
		p = (void*)ev;

		sty->cx += (int)(p->x) - (int)(q->x);
		sty->cy += (int)(p->y) - (int)(q->y);

		*(u64*)&(win->touch[10]) = ev->why;
	}
	return 0;
}
int point_explain(struct arena* win, struct event* ev)
{
	struct style* sty;
	struct relation* rel;

	rel = win->irel;
	if(rel == 0)return 1;

	while(1)
	{
		if(rel->samepinnextchip == 0)break;
		rel = connect_read(rel->samepinnextchip);
	}
	sty = (void*)(rel->destfoot);
	mouse_explain(win, sty, ev);

	return 1;
}