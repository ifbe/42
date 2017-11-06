#include "actor.h"
void* arenastart(u64, u64);
int arenastop(void*);
void actorstart(void*, void*);
int actorstop(void*);




static char* input = 0;
static char* output = 0;
static int combo = 0;
static int goooo = 0;




void win_cfg(struct event* ev)
{
	void* ret;
	//say("%x,%x,%x,%x\n", ev->why, ev->what, ev->where, ev->when);
	if(ev->what == hex32('w','+',0,0))
	{
		ret = arenastart(ev->why, ev->where);
		if(ret == 0)
		{
			say("error@w+\n");
			return;
		}
		actorstart(ret, 0);
	}
	else if(ev->what == hex32('w','-',0,0))
	{
		ret = (void*)(ev->where);
		arenastop(ret);
	}
}