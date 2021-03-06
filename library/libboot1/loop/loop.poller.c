#include "libboot.h"
int input(void*, int);
int role_fromfile(void* path, int len);




static int alive = 1;
static struct supply* supply = 0;
static struct entity* entity = 0;




int supplyevent(struct halfrel* stack, struct event* e)
{
	int j;
	struct event ev;
	struct supply* win;

	ev.why = e->why;
	ev.what = e->what;
	ev.where = e->where;
	ev.when = e->when;

	if(0 == ev.where)
	{
		//from cmd
		if(_char_ == ev.what){
			input(&ev.why, 0);
			return 0;
		}
		if(_myml_ == ev.what){
			role_fromfile((void*)ev.why, 0);
			return 0;
		}

		//maybe gamepad
		for(j=0;j<16;j++)
		{
			win = &supply[j];
			if(_sup_ == win->type)
			{
				ev.where = (u64)win;
				break;
			}
		}
	}

	win = (void*)(ev.where);
	if(0 == win)return 0;

	switch(win->fmt)
	{
		case _gl41none_:
		case _gl41easy_:
		case _gl41list_:
		case _gl41cmdq_:
		default:{
			stack[1].pchip = win;
			supply_give(win,0, stack,2, 0,0, &ev,0);break;
		}
	}
	return 0;
}
int supplyread_all(struct halfrel* stack)
{
	int j;
	struct supply* win;

	for(j=31;j>=0;j--)
	{
		win = &supply[j];
		if(0 == win->type)continue;

		if(_wnd_ == win->type){
			stack[1].pchip = win;
			supply_take(win,0, stack,2, 0,0, 0,0);
		}
/*		if(_spk_ == win->type){
			stack[1].pchip = win;
			supply_take(win,0, stack,2, 0,0, 0,0);
		}*/
	}
	return 0;
}
void poller(void* poller)
{
	//before
	u64 t0;
	u64 dt;
	struct event* ev;
	struct halfrel stack[0x80];
	stack[0].pchip = poller;

	//forever
	while(alive)
	{
		//cur time
		t0 = timeread();

		//draw frame
		supplyread_all(stack);

		//cleanup events
		while(1)
		{
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return;

			supplyevent(stack, ev);
		}

		//max fps
		dt = timeread() - t0;
		//say("dt=%d\n", delta);
		if(dt < 16000)sleep_us(16000-dt);
	}
}




void poller_delete(struct item* wrk, u8* arg)
{
}
void poller_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
}




void poller_free()
{
}
void poller_init(void* addr)
{
	supply = addr + 0xc00000;
	entity = addr + 0xe00000;
}
