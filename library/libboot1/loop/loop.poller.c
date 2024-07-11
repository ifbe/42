#include "libboot.h"
void* bootup_alloc();
int cleanevent(struct halfrel stack[]);




static _obj* mpoller = 0;
//
static _obj* supply = 0;
static _obj* entity = 0;




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
	struct halfrel stack[0x80];
	stack[0].pchip = poller;

	u64 t0 = 0;
	u64 tn;

	//forever
	int alive = 1;
	while(alive)
	{
		//cur time
		//t0 = timeread_us();
		//logtoall("t0=%lld\n",t0);

		//draw frame
		supplyread_all(stack);

		//cleanup events
		if(0 == cleanevent(stack))break;

		//max fps
		tn = timeread_us();
		//logtoall("dt=%d\n", delta);
		if(tn < t0+16000){
			logtoall("%llx,%llx,%llx\n",t0,tn,tn-t0);
			sleep_us(16000-(tn-t0));
		}
		t0 = tn;
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
