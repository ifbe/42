#include "actor.h"
#define __act__ hex32('a','c','t',0)
#define __win__ hex32('w','i','n',0)
void* arenastart(u64, u64);
int arenastop(void*);
void actorstart(void*, void*);
int actorstop(void*);
void actorchoose(void*);
void* relation_read(u64);
void relation_write(void* uchip, void* ufoot, u64 utype, void* bchip, u64 bfoot, u64 btype);




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static int stlen = 0;




void act_add()
{
}
void act_del()
{
}
void act_at(struct arena* win, struct actor* act)
{
	struct style* st = (void*)style + stlen;
	stlen += sizeof(struct style);

	st->cx = 0x4000 + (getrandom()%0x1000)*8;
	st->cy = 0x4000 + (getrandom()%0x1000)*8;
	st->wantw = 0x8000;
	st->wanth = 0x8000;

	relation_write(win, st, __win__, act, 0, __act__);
}




void win_add(u64 why, u64 where)
{
	void* ret = arenastart(why, where);
	if(ret == 0)
	{
		say("error@w+\n");
		return;
	}
}
void win_del(u64 why, u64 where)
{
	void* ret = (void*)where;
	arenastop(ret);
}
void win_at(u64 why, u64 where)
{
	void* ret;
	if(why == hex32('f','i','l','e'))
	{
		//ret = (void*)where;
		//actorstart(ret, 0);
		actorchoose("stl");
	}
}




void wmgr_create(void* addr)
{
	arena = addr + 0;
	actor = addr + 0x100000;
	style = addr + 0x200000;
}
