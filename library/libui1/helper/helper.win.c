#include "actor.h"
void* arenastart(u64, u64);
int arenastop(void*);
void actorstart(void*, void*);
int actorstop(void*);




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
	else if(ev->what == hex32('w','@',0,0))
	{
		if(ev->why == hex32('f','i','l','e'))
		{
			ret = (void*)(ev->where);
			actorstart(ret, 0);
		}
		else say("%.*s\n", 8, ev);
	}
}