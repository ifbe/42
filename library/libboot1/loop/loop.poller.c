#include "libuser.h"
int input(void*, int);




static int alive = 1;
static struct supply* supply = 0;
static struct entity* entity = 0;




int supplyevent(void* poller, struct event* e)
{
	int j;
	struct halfrel self;
	struct halfrel peer;
	struct event ev;
	struct supply* win;

	ev.why = e->why;
	ev.what = e->what;
	ev.where = e->where;
	ev.when = e->when;

	if(0 == ev.where)
	{
		//from cmd
		if(_char_ == ev.what)
		{
			input(&ev.why, 0);
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
		case _none_:
		case _easy_:
		case _full_:
		case _coop_:
		default:{
			self.pchip = win;
			peer.pchip = poller;
			supplywrite(&self, &peer, 0, 0, &ev, 0);break;
		}
	}
	return 0;
}
int supplyread_all(void* poller)
{
	int j;
	struct supply* win;
	struct halfrel self;
	struct halfrel peer;

	peer.pchip = poller;
	for(j=31;j>=0;j--)
	{
		win = &supply[j];
		if(0 == win->type)continue;

		if(_phys_ == win->type){
			self.pchip = win;
			supplyread(&self, &peer, 0, 0, 0, 0);
		}
		if(_wnd_ == win->type){
			self.pchip = win;
			supplyread(&self, &peer, 0, 0, 0, 0);
		}
/*		if(_spk_ == win->type){
			self.pchip = win;
			supplyread(&self, &peer, 0, 0, 0, 0);
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

	//forever
	while(alive)
	{
		//cur time
		t0 = timeread();

		//draw frame
		supplyread_all(poller);

		//cleanup events
		while(1)
		{
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return;

			supplyevent(poller, ev);
		}

		//max fps
		dt = timeread() - t0;
		//say("dt=%d\n", delta);
		if(dt < 16000)sleep_us(16000-dt);
	}
}




void poller_delete(struct worker* wrk, u8* arg)
{
}
void poller_create(struct worker* wrk, u8* arg, int argc, u8** argv)
{
}




void freepoller()
{
}
void initpoller(void* addr)
{
	supply = addr + 0xc00000;
	entity = addr + 0xe00000;
}
