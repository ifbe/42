#include "libboot.h"
int input(void*, int);
int role_fromfile(void* path, int len);
void* bootup_alloc();




static _obj* mpoller = 0;
//
static _obj* supply = 0;
static _obj* entity = 0;




int supplyevent(struct halfrel* stack, struct event* e)
{
	int j;
	struct event ev;
	_obj* win;

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

	switch(win->type)
	{
		case _gl41none_:
		case _gl41easy_:
		case _gl41list_:
		case _gl41cmdq_:
		default:{
			stack[1].pchip = win;
			supply_giveby(win,0, stack,2, 0,0, &ev,0);break;
		}
	}
	return 0;
}
int supplyread_all(struct halfrel* stack)
{
	int j;
	_obj* win;

	for(j=31;j>=0;j--)
	{
		win = &supply[j];
		if(0 == win->type)continue;

		switch(win->type){
		//case _gl41none_:
		//case _gl41easy_:
		//case _gl41list_:
		//case _gl41cmdq_:
		case _render_:
			stack[1].pchip = win;
			supply_takeby(win,0, stack,2, 0,0, 0,0);
			break;
		case _wnd_:
			stack[1].pchip = win;
			supply_takeby(win,0, stack,2, 0,0, 0,0);
			break;
		}
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
	int alive = 1;
	while(alive)
	{
		//cur time
		t0 = timeread_us();
		//logtoall("t0=%lld\n",t0);

		//draw frame
		supplyread_all(stack);

		//cleanup events
		while(1)
		{
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)goto byebye;

			supplyevent(stack, ev);
		}

		//max fps
		dt = timeread_us() - t0;
		//logtoall("dt=%d\n", delta);
		if(dt < 16000)sleep_us(16000-dt);
	}
byebye:
	logtoall("poller@%p exiting\n", poller);
}




void poller_delete(struct item* wrk, u8* arg)
{
}
void poller_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
}




void* poller_alloc()
{
	if(mpoller)return 0;

	mpoller = bootup_alloc();
	return mpoller;
}




void poller_free()
{
}
void poller_init(void* addr)
{
	supply = addr + 0xc00000;
	entity = addr + 0xe00000;
}
