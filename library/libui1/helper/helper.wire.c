#include "actor.h"
#define maxlen 0x100000
#define __act__ hex32('a','c','t',0)
#define __win__ hex32('w','i','n',0)
static struct style* style = 0;
static int stlen = 0;
static struct relation* connect = 0;
static int wirelen = 0;




void* connect_write_new(
	void* uchip, u64 ufoot, u64 utype,
	void* bchip, u64 bfoot, u64 btype)
{
	struct relation* w;
	struct style* st;
	if(wirelen > maxlen-0x40)return 0;

	w = (void*)connect + wirelen;
	wirelen += sizeof(struct relation);

	//position
	if(btype != 0)
	{
		if(ufoot == 0)
		{
			st = (void*)style + stlen;
			stlen += sizeof(struct style);

			st->cx = 0x4000 + (getrandom()%0x1000)*8;
			st->cy = 0x4000 + (getrandom()%0x1000)*8;
			st->wantw = 0x8000;
			st->wanth = 0x8000;
			st->dim = 2;

			ufoot = (u64)st;
		}
	}

	//1.dest
	w->destchip = (u64)uchip;
	w->destfoot = ufoot;
	w->desttype = utype;
	w->samepinprevchip = 0;
	w->samepinnextchip = 0;

	//2.self
	w->selfchip = (u64)bchip;
	w->selffoot = bfoot;
	w->selftype = btype;
	w->samechipprevpin = 0;
	w->samechipnextpin = 0;

	return w;
}
//hashinfo, hashfoot, 'hash', fileinfo, 0, 'file'
//fileinfo, fileline, 'file', funcinfo, 0, 'func'
//funcinfo, funcofst, 'func', hashinfo, 0, 'hash'
//wininfo,  position, 'win',  actor,    0, 'act'
//actinfo,  which,    'act',  userinfo, what,     'user'
int connect_write(void* uchip, u64 ufoot, u64 utype, void* bchip, u64 bfoot, u64 btype)
{
	struct arena* h1;
	struct actor* h2;
	struct relation* w1;
	struct relation* w2;
	struct relation* wc;




	//dest wire
	h1 = uchip;
	if(h1->first == 0)
	{
		w1 = connect_write_new(uchip, ufoot, utype, 0, 0, 0);
		h1->first = w1;
		//h1->last = 0;
	}
	else
	{
		w1 = h1->first;
		if( (w1->desttype != utype) | (w1->selftype != 0) )
		{
			wc = w1;
			w1 = connect_write_new(h1, ufoot, utype, 0, 0, 0);
			h1->first = w1;

			wc->samechipprevpin = (void*)w1 - (void*)connect;
			w1->samechipnextpin = (void*)wc - (void*)connect;
		}
	}




	//src wire
	h2 = bchip;
	w2 = connect_write_new(uchip, ufoot, utype, bchip, bfoot, btype);
	if(h2->first == 0)
	{
		h2->first = w2;
		h2->last = 0;
	}




	//w1->samechipprevpin = ?;		//unchanged
	//w1->samechipnextpin = ?;		//unchanged

	//w1->samepinprevchip = 0;		//certainly
	wc = w1;
	while(wc->samepinnextchip != 0)
	{
		wc = (void*)connect + (wc->samepinnextchip);
	}
	wc->samepinnextchip = (void*)w2 - (void*)connect;
	w2->samepinprevchip = (void*)wc - (void*)connect;
	w2->samepinnextchip = 0;		//certainly





	wc = h2->first;
	while(wc->samechipnextpin != 0)
	{
		wc = (void*)connect + (wc->samechipnextpin);
	}
	if(wc != w2)
	{
		wc->samechipnextpin = (void*)w2 - (void*)connect;
		w2->samechipprevpin = (void*)wc - (void*)connect;
		w2->samechipnextpin = 0;
	}

	return 1;
}
void* connect_read(int off)
{
	if(off == 0)return 0;
	return (void*)connect + off;
}
void connect_create(void* addr)
{
	style = (void*)(addr+0x200000);
	stlen = sizeof(struct style);
	connect = (void*)(addr+0x300000);
	wirelen = sizeof(struct relation);
}
void connect_delete()
{
}




/*
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
*/
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