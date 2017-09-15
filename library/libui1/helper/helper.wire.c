#include "actor.h"
#define __act__ hex32('a','c','t',0)
#define __win__ hex32('w','i','n',0)
static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct relation* connect = 0;




void* connect_read()
{
}
void* connect_write(u64 pininfo, u64 destiny, u64 chipinfo, u64 footinfo)
{
	int j;
	struct relation* this;
	struct style* st;

	//locate first
	for(j=0;j<0x400;j++)
	{
		if(connect[j].pininfo == 0)
		{
			this = &connect[j];
			break;
		}
	}

	//style
	if(footinfo == 0)footinfo = (u64)&style[j];
	st = (void*)footinfo;
	st->cx = 0x4000 + (getrandom()%0x1000)*8;
	st->cy = 0x4000 + (getrandom()%0x1000)*8;
	st->dim = 2;
	st->wantw = 0x8000;
	st->wanth = 0x8000;

	//generate
	this->pininfo = pininfo;
	this->destiny = destiny;
	this->samepinprevact = 0;
	this->samepinnextact = 0;

	this->chipinfo = chipinfo;
	this->footinfo = footinfo;
	this->samewinprevpin = 0;
	this->samewinnextpin = 0;

	return this;
}
void connect_create(void* addr)
{
	arena = (void*)(addr+0);
	actor = (void*)(addr+0x100000);
	style = (void*)(addr+0x200000);
	connect = (void*)(addr+0x300000);
}
void connect_delete()
{
}




/*
	actor[0].start();
	j = actorstart(0, 0);
	connect[j].cx = 0x8000;
	connect[j].cy = 0x8000;
	connect[j].wantw = 0xffff;
	connect[j].wanth = 0xffff;

	actor[2].start();
	j = actorstart(0, 2);
	connect[j].cx = 0x4000;
	connect[j].cy = 0x4000;
	connect[j].wantw = 0x8000;
	connect[j].wanth = 0x8000;

	actor[3].start();
	j = actorstart(0, 3);
	connect[j].cx = 0xc000;
	connect[j].cy = 0x4000;
	connect[j].wantw = 0x8000;
	connect[j].wanth = 0x8000;

	actor[21].start();
	j = actorstart(0, 21);
	connect[j].cx = 0x4000;
	connect[j].cy = 0xc000;
	connect[j].wantw = 0x8000;
	connect[j].wanth = 0x8000;
*/
/*
	int t;
	for(t=1;t<50;t++)
	{
		if(connect[t].parent_this == 0)break;
	}
	//
	connect[t].parent_type = 0;
	connect[t].parent_this = &arena[w];
	if(arena[w].bot == 0)	//0
	{
		connect[t].below = 0;
		connect[t].above = 0;

		arena[w].bot = &connect[t];
		arena[w].top = 0;
	}
	else if(arena[w].top == 0)	//1
	{
		(arena[w].bot)->above = &connect[t];

		connect[t].below = arena[w].bot;
		connect[t].above = 0;

		//arena[w].bot = itself
		arena[w].top = &connect[t];
	}
	else	//more
	{
		(arena[w].top)->above = &connect[t];

		connect[t].below = arena[w].top;
		connect[t].above = 0;

		//arena[w].bot = itself
		arena[w].top = &connect[t];
	}

	//
	connect[t].child_type = 0xffffffff;
	connect[t].child_this = &actor[a];
	if(actor[a].first == 0)		//0
	{
		connect[t].prev = 0;
		connect[t].next = 0;

		actor[a].first = &connect[t];
	}
	else if(actor[a].last == 0)		//1
	{
		(actor[a].first)->next = &connect[t];

		connect[t].prev = actor[a].first;
		connect[t].next = 0;

		actor[a].last = &connect[t];
	}
	else	//more
	{
		(actor[a].last)->next = &connect[t];

		connect[t].prev = actor[a].last;
		connect[t].next = 0;

		actor[a].last = &connect[t];
	}

	//
	return t;
*/