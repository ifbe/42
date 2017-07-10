#include "actor.h"
void rectframe(void*,
	int,int,
	int,int,
	u32
);
void navi_create(void*);
void navi_delete();
void vkbd_create(void*);
void vkbd_delete();
void vt100_create(void*);
void vt100_delete();
//
int navi_read(void*);
int navi_write(void*);
int vkbd_read(void*);
int vkbd_write(void*);
int vt100_read(void*, int, int, int, int);
int vt100_write(void*);
//
void cli_write(void*);




//
static u64 tmp=0;
//
static int showvt100 = 0;
static int showvkbd = 0;
static int shownavi = 0;




void levitate_delete()
{
	vt100_delete();
	vkbd_delete();
	navi_delete();
}
void levitate_create(void* base)
{
	navi_create(base);
	vkbd_create(base);
	vt100_create(base);
}





void levitate_read(struct arena* win)
{
	int x0,y0,x1,y1;
	struct relation* rel = win->top;
	while(1)
	{
		x0 = (win->w)*((rel->cx)-(rel->wantw/2))/0x10000;
		y0 = (win->h)*((rel->cy)-(rel->wanth/2))/0x10000;
		x1 = (win->w)*((rel->cx)+(rel->wantw/2))/0x10000;
		y1 = (win->h)*((rel->cy)+(rel->wanth/2))/0x10000;

		rectframe(win,
			x0, y0,
			x1, y1,
			0xff00ff
		);

		rel = rel->above;
		if(rel == 0)break;
	}

	if(showvt100&1)vt100_read(win, 0x2000, 0x2000, 0xe000, 0x8000);
	if(showvkbd&1)vkbd_read(win);
	navi_read(win);
}
int levitate_write(struct event* ev)
{
	int j;
	int x,y,z,w;
	struct relation* rel;
	struct relation* top;
	struct relation* belowtop;
	struct relation* belowthis;
	struct relation* abovethis;
	struct arena* win = (void*)(ev->where);

	w = (ev->what)&0xff;
	if(w == 'p')
	{
		x = (ev->why)&0xffff;
		y = ((ev->why)>>16)&0xffff;
		w = ((ev->why)>>48)&0xffff;
		rel = win->top;
		while(1)
		{
			if(rel == 0)break;
			if(rel == win->bot)break;

			if(x > (int)(rel->cx) - (int)(rel->wantw)/2){
			if(x < (int)(rel->cx) + (int)(rel->wantw)/2){
			if(y > (int)(rel->cy) - (int)(rel->wanth)/2){
			if(y < (int)(rel->cy) + (int)(rel->wanth)/2){
				break;
			}
			}
			}
			}

			rel = rel->below;
		}

		if(ev->what == 0x2b70)
		{
			if((y>0xe000)&&(x<0x8000))
			{
				showvt100++;
				showvkbd++;
				return 0;
			}
			else if(showvt100&1)
			{
				j = vkbd_write(ev);
				if(j == 1)cli_write(ev);
				return 0;
			}
			else
			{
				tmp = ev->why;
				if(rel == 0)return 0;
				if(rel == win->bot)return 0;
				if(rel == win->top)return 0;

				rel->below->above = rel->above;
				rel->above->below = rel->below;

				win->top->above = rel;
				//win->top->below keep

				rel->below = win->top;
				rel->above = 0;

				win->top = rel;

				//debug
				rel = win->bot;
				for(j=0;j<16;j++)
				{
					say("%x", rel);
					if(rel == 0)break;

					say("->");
					rel = rel->above;
				}
				say("\n");
				return 0;
			}
		}
		else if(ev->what == 0x2d70)
		{
			tmp = 0;
			return 0;
		}
		else if(ev->what == 0x4070)
		{
			if(w != ((tmp>>48)&0xffff))return 0;
			if(tmp != 0)
			{
				x = (ev->why&0xffff) - (tmp&0xffff);
				y = ((ev->why>>16)&0xffff) - ((tmp>>16)&0xffff);
				rel->cx += x;
				rel->cy += y;
				tmp = ev->why;
			}
		}
		/*
		else if(w == 'b')
		{
			rel->wantw = rel->wantw*90/100;
			rel->wanth = rel->wanth*90/100;
		}
		else if(w == 'f')
		{
			rel->wantw = rel->wantw*110/100;
			rel->wanth = rel->wanth*110/100;
		}
		*/
	}

	return 0;
}