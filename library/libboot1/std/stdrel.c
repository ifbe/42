#include "libboot.h"
#define maxlen 0x100000




static struct relation* recycle = 0;
static struct relation* wirebuf = 0;
static int wirecur = 0x40;
static int wirelen = 0x100000;
void initstdrel(void* addr)
{
	wirebuf = addr;
}
void freestdrel()
{
}





void relation_debug(struct relation* rel)
{
	struct relation* next;
	struct relation* temp;
	while(1)
	{
		say("%x->",rel);

		if(rel->samedstnextsrc == 0){say("a");break;}
		next = (void*)wirebuf + (rel->samedstnextsrc);

		if(next->samedstprevsrc == 0){say("b");break;}
		temp = (void*)wirebuf + (next->samedstprevsrc);

		if(temp == rel)say("<-");
		rel = next;
	}
	say("\n");
}
void relation_recycle(struct relation* rel)
{
	struct relation* temp;
	if(rel == 0)return;

	if(wirecur == (void*)rel - (void*)wirebuf + sizeof(struct relation))
	{
		wirecur -= sizeof(struct relation);
		return;
	}

	rel->samedstprevsrc = 0;
	rel->samedstnextsrc = 0;

	if(recycle == 0)
	{
		recycle = rel;
		return;
	}

	temp = recycle;
	while(1)
	{
		if(temp->samedstnextsrc == 0)
		{
			temp->samedstnextsrc = (void*)rel - (void*)wirebuf;
			break;
		}
		temp = (void*)wirebuf + (temp->samedstnextsrc);
	}
}
void* relation_grow()
{
	struct relation* temp;
	if(recycle == 0)
	{
		temp = (void*)wirebuf + wirecur;
		wirecur += sizeof(struct relation);
		return temp;
	}

	temp = recycle;
	if(temp->samedstnextsrc == 0)recycle = 0;
	else recycle = (void*)wirebuf + (temp->samedstnextsrc);
	return temp;
}
void* relation_generate(
	void* dc, void* df, u32 dctype, u32 dftype,
	void* sc, void* sf, u32 sctype, u32 sftype)
{
	struct relation* w = relation_grow();
	if(w == 0)return 0;

	//1.dst
	w->dstchip = (u64)dc;
	w->dstfoot = (u64)df;

	w->dsttype = dctype;
	w->dstflag = dftype;

	w->samedstprevsrc = 0;
	w->samedstnextsrc = 0;

	//2.src
	w->srcchip = (u64)sc;
	w->srcfoot = (u64)sf;

	w->srctype = sctype;
	w->srcflag = sftype;

	w->samesrcprevdst = 0;
	w->samesrcnextdst = 0;

	return w;
}




void* samedstprevsrc(struct relation* rel)
{
	if(rel == 0)return 0;
	if(rel->samedstprevsrc == 0)return 0;
	return (void*)wirebuf + (rel->samedstprevsrc);
}
void* samedstnextsrc(struct relation* rel)
{
	if(rel == 0)return 0;
	if(rel->samedstnextsrc == 0)return 0;
	return (void*)wirebuf + (rel->samedstnextsrc);
}
void* samesrcprevdst(struct relation* rel)
{
	if(rel == 0)return 0;
	if(rel->samesrcprevdst == 0)return 0;
	return (void*)wirebuf + (rel->samesrcprevdst);
}
void* samesrcnextdst(struct relation* rel)
{
	if(rel == 0)return 0;
	if(rel->samesrcnextdst == 0)return 0;
	return (void*)wirebuf + (rel->samesrcnextdst);
}




int relation_r(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	switch(self->type){
		case _dev_:return deviceread(self, peer, arg, idx, buf, len);
		case _dri_:return driverread(self, peer, arg, idx, buf, len);
		case  _fd_:return systemread(self, peer, arg, idx, buf, len);
		case _art_:return arteryread(self, peer, arg, idx, buf, len);
		case _win_:return  arenaread(self, peer, arg, idx, buf, len);
		case _act_:return  actorread(self, peer, arg, idx, buf, len);
	}
	return 0;
}
int relationread(void* chip, int foot, void* arg, int idx, void* buf, int len)
{
	struct item* item;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

	item = chip;
	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		if(foot == rel->dstflag){
			self = (void*)&rel->srcchip;
			peer = (void*)&rel->dstchip;
			return relation_r(self, peer, arg, idx, buf, len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		if(foot == rel->srcflag){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			return relation_r(self, peer, arg, idx, buf, len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}




int relation_w(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	switch(self->type){
		case _dev_:return devicewrite(self, peer, arg, idx, buf, len);
		case _dri_:return driverwrite(self, peer, arg, idx, buf, len);
		case  _fd_:return systemwrite(self, peer, arg, idx, buf, len);
		case _art_:return arterywrite(self, peer, arg, idx, buf, len);
		case _win_:return  arenawrite(self, peer, arg, idx, buf, len);
		case _act_:return  actorwrite(self, peer, arg, idx, buf, len);
	}
	return 0;
}
int relationwrite(void* chip, int foot, void* arg, int idx, void* buf, int len)
{
	struct item* item;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

	item = chip;
	rel = item->irel0;
	while(1){
		if(0 == rel)break;
		//say("irel:%.8s\n",&rel->dstflag);
		if(foot == rel->dstflag){
			self = (void*)&rel->srcchip;
			peer = (void*)&rel->dstchip;
			return relation_w(self, peer, arg, idx, buf, len);
		}
		rel = samedstnextsrc(rel);
	}

	rel = item->orel0;
	while(1){
		if(0 == rel)break;
		//say("orel:%.8s\n",&rel->srcflag);
		if(foot == rel->srcflag){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			return relation_w(self, peer, arg, idx, buf, len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}




int relationstop(struct halfrel* self, struct halfrel* peer)
{
	switch(self->type){
		case _fd_: return systemstop(self, peer);
		case _art_:return arterystop(self, peer);
		case _win_:return  arenastop(self, peer);
		case _act_:return  actorstop(self, peer);
	}
	return 0;
}




int relationstart(struct halfrel* self, struct halfrel* peer)
{
	switch(self->type){
		case _fd_: return systemstart(self, peer);
		case _art_:return arterystart(self, peer);
		case _win_:return  arenastart(self, peer);
		case _act_:return  actorstart(self, peer);
	}
	return 0;
}




int relationdelete(struct relation* this)
{
	struct item* uchip;
	struct relation* prev;
	struct relation* next;
	if(this == 0)return 0;

	if(this->samedstprevsrc == 0)prev = 0;
	else prev = (void*)wirebuf + (this->samedstprevsrc);
	if(this->samedstnextsrc == 0)next = 0;
	else next = (void*)wirebuf + (this->samedstnextsrc);

	if(prev != 0)
	{
		if(next == 0)prev->samedstnextsrc = 0;
		else prev->samedstnextsrc = (void*)next - (void*)wirebuf;
	}
	if(next != 0)
	{
		if(prev == 0)next->samedstprevsrc = 0;
		else next->samedstprevsrc = (void*)prev - (void*)wirebuf;
	}

	uchip = (void*)(this->dstchip);
	if(this == uchip->irel0)uchip->irel0 = next;

	relation_recycle(this);
	return 0;
}
void* relationcreate(
	void* dc, void* df, u32 dctype, u32 dftype,
	void* sc, void* sf, u32 sctype, u32 sftype)
{
	struct item* h1;
	struct item* h2;
	struct relation* ww;
	struct relation* wc;
	if(wirecur >= wirelen)
	{
		say("wire buf not enough\n");
		return 0;
	}

	//
	ww = relation_generate(dc, df, dctype, dftype, sc, sf, sctype, sftype);

	//dst wire
	h1 = dc;
	if(0 != h1->ireln)
	{
		wc = h1->ireln;
		wc->samedstnextsrc = (void*)ww - (void*)wirebuf;
		ww->samedstprevsrc = (void*)wc - (void*)wirebuf;
	}
	h1->ireln = ww;
	if(0 == h1->irel0)h1->irel0 = ww;

	h2 = sc;
	if(0 != h2->oreln)
	{
		wc = h2->oreln;
		wc->samesrcnextdst = (void*)ww - (void*)wirebuf;
		ww->samesrcprevdst = (void*)wc - (void*)wirebuf;
	}
	h2->oreln = ww;
	if(0 == h2->orel0)h2->orel0 = ww;

	return ww;
}
void relation_choose(struct item* item, struct relation* rel)
{
	struct relation* prev;
	struct relation* next;
	struct relation* tmp;

	//no next: do nothing
	if(0 == rel->samesrcnextdst)next = 0;
	else next = (void*)wirebuf + rel->samesrcnextdst;
	if(0 == next)return;

	//prev
	if(0 == rel->samesrcprevdst)prev = 0;
	else prev = (void*)wirebuf + rel->samesrcprevdst;
	if(prev)
	{
		prev->samesrcnextdst = (void*)next - (void*)wirebuf;
		next->samesrcprevdst = (void*)prev - (void*)wirebuf;

		rel->samesrcprevdst = (void*)(item->oreln) - (void*)wirebuf;
		rel->samesrcnextdst = 0;

		tmp = item->oreln;
		tmp->samesrcnextdst = (void*)rel - (void*)wirebuf;

		item->oreln = rel;
		return;
	}
	else
	{
		next->samesrcprevdst = 0;

		rel->samesrcprevdst = (void*)(item->oreln) - (void*)wirebuf;
		rel->samesrcnextdst = 0;

		tmp = item->oreln;
		tmp->samesrcnextdst = (void*)rel - (void*)wirebuf;

		item->orel0 = next;
		item->oreln = rel;
		return;
	}
}
void* relation_search(struct item* item, u32 foottype)
{
	struct relation* rel;

	rel = item->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(foottype == rel->dstflag)return rel;

		rel = samesrcnextdst(rel);
	}

	rel = item->irel0;
	while(1)
	{
		if(0 == rel)break;

		if(foottype == rel->srcflag)return rel;

		rel = samedstnextsrc(rel);
	}

	return 0;
}
