#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define maxlen 0x100000
#define __act__ hex32('a','c','t',0)
#define __win__ hex32('w','i','n',0)
void say(void*, ...);




struct item
{
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};
};
struct relation
{
	//[0x00,0x1f]
	u64 destchip;
	u64 destfoot;
	u32 desttype;
	u32 destflag;
	u32 samedstprevsrc;
	u32 samedstnextsrc;

	//0x20,0x3f
	u64 selfchip;
	u64 selffoot;
	u32 selftype;
	u32 selfflag;
	u32 samesrcprevdst;
	u32 samesrcnextdst;
};




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
void relation_choose(struct item* item, struct relation* rel)
{
	struct relation* prev;
	struct relation* next;
	struct relation* tmp;

	if(0 == rel->samedstprevsrc)prev = 0;
	else prev = (void*)wirebuf + rel->samedstprevsrc;
	if(0 == rel->samedstnextsrc)next = 0;
	else next = (void*)wirebuf + rel->samedstnextsrc;

	if((0 != prev)&&(0 != next))
	{say("fuck1\n");
		prev->samedstnextsrc = (void*)next - (void*)wirebuf;
		next->samedstprevsrc = (void*)prev - (void*)wirebuf;

		rel->samedstprevsrc = (void*)(item->ireln) - (void*)wirebuf;
		rel->samedstnextsrc = 0;

		tmp = item->ireln;
		tmp->samedstnextsrc = (void*)rel - (void*)wirebuf;

		item->ireln = rel;
		return;
	}

	//if((0 != prev)&&(0 == next))	//change nothing

	if((0 == prev)&&(0 != next))
	{say("fuck2\n");
		next->samedstprevsrc = 0;

		rel->samedstprevsrc = (void*)(item->ireln) - (void*)wirebuf;
		rel->samedstnextsrc = 0;

		tmp = item->ireln;
		tmp->samedstnextsrc = (void*)rel - (void*)wirebuf;

		item->irel0 = next;
		item->ireln = rel;
		return;
	}

	//if((0 == prev)&&(0 == next))	//change nothing
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
	void* uchip, void* ufoot, u32 utype,
	void* bchip, void* bfoot, u32 btype)
{
	struct relation* w = relation_grow();
	if(w == 0)return 0;

	//1.dest
	w->destchip = (u64)uchip;
	w->destfoot = (u64)ufoot;

	w->desttype = utype;
	w->destflag = 0;

	w->samedstprevsrc = 0;
	w->samedstnextsrc = 0;

	//2.self
	w->selfchip = (u64)bchip;
	w->selffoot = (u64)bfoot;

	w->selftype = btype;
	w->selfflag = 0;

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
void* relationread(int off)
{
	if(off == 0)return 0;
	return (void*)wirebuf + off;
}
void* relationwrite()
{
	return 0;
}
void relationdelete(struct relation* this)
{
	struct item* uchip;
	struct relation* prev;
	struct relation* next;
	if(this == 0)return;

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

	uchip = (void*)(this->destchip);
	if(this == uchip->irel0)uchip->irel0 = next;

	relation_recycle(this);
}
void* relationcreate(
	void* uchip, void* ufoot, u32 utype,
	void* bchip, void* bfoot, u32 btype)
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
	ww = relation_generate(uchip, ufoot, utype, bchip, bfoot, btype);

	//dest wire
	h1 = uchip;
	if(0 != h1->ireln)
	{
		wc = h1->ireln;
		wc->samedstnextsrc = (void*)ww - (void*)wirebuf;
		ww->samedstprevsrc = (void*)wc - (void*)wirebuf;
	}
	h1->ireln = ww;
	if(0 == h1->irel0)h1->irel0 = ww;

	h2 = bchip;
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