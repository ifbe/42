#include "libuser.h"
int input(void*, int);




static int alive = 1;
static struct arena* arena = 0;
static struct actor* actor = 0;




int arenaevent(void* poller, struct event* e)
{
	int j;
	struct halfrel self;
	struct halfrel peer;
	struct event ev;
	struct arena* win;

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
			win = &arena[j];
			if(_win_ == win->type)
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
			arenawrite(&self, &peer, 0, 0, &ev, 0);break;
		}
	}
	return 0;
}
int arenaread_all(void* poller)
{
	int j;
	struct arena* win;
	struct halfrel self;
	struct halfrel peer;

	peer.pchip = poller;
	for(j=31;j>=0;j--)
	{
		win = &arena[j];
		if(0 == win->type)continue;

		if(_win_ == win->type){
			self.pchip = win;
			arenaread(&self, &peer, 0, 0, 0, 0);
		}
		if(_spk_ == win->type){
			self.pchip = win;
			arenaread(&self, &peer, 0, 0, 0, 0);
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

	//forever
	while(alive)
	{
		//cur time
		t0 = timeread();

		//draw frame
		arenaread_all(poller);

		//cleanup events
		while(1)
		{
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return;

			arenaevent(poller, ev);
		}

		//max fps
		dt = timeread() - t0;
		//say("dt=%d\n", delta);
		if(dt < 16000)sleep_us(16000-dt);
	}
}




void freepoller()
{
}
void initpoller(void* addr)
{
	arena = addr + 0xc00000;
	actor = addr + 0xe00000;
}