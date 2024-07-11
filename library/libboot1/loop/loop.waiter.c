#include "libuser.h"
int input(void*, int);
int role_fromfile(void* path, int len);


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

	switch(win->type){
	case _render_:
	case _wnd_:
		stack[1].pchip = win;
		supply_giveby(win,0, stack,2, 0,0, &ev,0);
		break;
	}
	return 0;
}
int cleanevent(struct halfrel stack[])
{
	struct event* ev;
	while(1){
		ev = eventread();
		if(0 == ev)break;
		if(0 == ev->what)return 0;

		supplyevent(stack, ev);
	}
	return 1;
}
void waiter(void* w)
{
	struct halfrel stack[0x80];
	stack[0].pchip = w;

	while(1){
		if(0 == cleanevent(stack))break;
		sleep_us(10*1000);
	}
}




void waiter_delete(struct item* wrk, u8* arg)
{
}
void waiter_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
}




void* waiter_alloc()
{
	return bootup_alloc();
}




void waiter_exit()
{
}
void waiter_init(void* addr)
{
	supply = addr + 0xc00000;
	entity = addr + 0xe00000;
}