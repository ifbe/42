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
	u64 type;
	u64 fmt;
	union{
		void* irel;
		u64 pad0;
	};
	union{
		void* orel;
		u64 pad1;
	};
};
struct relation
{
	//[0x00,0x1f]
	u64 destchip;
	u64 destfoot;
	u32 desttype;
	u32 destflag;
	u32 samepinprevchip;
	u32 samepinnextchip;

	//0x20,0x3f
	u64 selfchip;
	u64 selffoot;
	u32 selftype;
	u32 selfflag;
	u32 samechipprevpin;
	u32 samechipnextpin;
};
static struct relation* recycle = 0;
static struct relation* wirebuf = 0;
static int wirecur = 0x40;
static int wirelen = 0x100000;
void initrelation(void* addr)
{
	wirebuf = addr+0x300000;
}
void relation_debug(struct relation* rel)
{
	struct relation* next;
	struct relation* temp;
	while(1)
	{
		say("%x->",rel);

		if(rel->samepinnextchip == 0){say("a");break;}
		next = (void*)wirebuf + (rel->samepinnextchip);

		if(next->samepinprevchip == 0){say("b");break;}
		temp = (void*)wirebuf + (next->samepinprevchip);

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

	rel->samepinprevchip = 0;
	rel->samepinnextchip = 0;

	if(recycle == 0)
	{
		recycle = rel;
		return;
	}

	temp = recycle;
	while(1)
	{
		if(temp->samepinnextchip == 0)
		{
			temp->samepinnextchip = (void*)rel - (void*)wirebuf;
			break;
		}
		temp = (void*)wirebuf + (temp->samepinnextchip);
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
	if(temp->samepinnextchip == 0)recycle = 0;
	else recycle = (void*)wirebuf + (temp->samepinnextchip);
	return temp;
}
void relation_swap(struct relation* m, struct relation* n)
{
	u64 temp;
	struct item* uchip;
	struct relation* mprev = 0;
	struct relation* mnext = 0;
	struct relation* nprev = 0;
	struct relation* nnext = 0;
	if(m == 0)return;
	if(n == 0)return;
	if(m == n)return;

	uchip = (void*)(m->destchip);
//relation_debug(uchip->irel);
//say("uchip:%x,%x,%x,%x\n",uchip,uchip->irel,m,n);
	if(uchip->irel == m)uchip->irel = n;
	else if(uchip->irel == n)uchip->irel = m;

	if(m->samepinprevchip != 0)
	{mprev = (void*)wirebuf + (m->samepinprevchip);}
	if(m->samepinnextchip != 0)
	{mnext = (void*)wirebuf + (m->samepinnextchip);}
	if(n->samepinprevchip != 0)
	{nprev = (void*)wirebuf + (n->samepinprevchip);}
	if(n->samepinnextchip != 0)
	{nnext = (void*)wirebuf + (n->samepinnextchip);}
//say("%x,%x,%x,%x\n",mprev,mnext,nprev,nnext);

	if(mnext == n)
	{
//say("case1\n");
		m->samepinnextchip = n->samepinnextchip;
		n->samepinprevchip = m->samepinprevchip;
		m->samepinprevchip = (void*)n - (void*)wirebuf;
		n->samepinnextchip = (void*)m - (void*)wirebuf;
		mnext = 0;
		nprev = 0;
	}
	else if(mprev == n)
	{
//say("case2\n");
		m->samepinprevchip = n->samepinprevchip;
		n->samepinnextchip = m->samepinnextchip;
		n->samepinprevchip = (void*)m - (void*)wirebuf;
		m->samepinnextchip = (void*)n - (void*)wirebuf;
		mprev = 0;
		nnext = 0;
	}
	else
	{
//say("case3\n");
		temp = m->samepinprevchip;
		m->samepinprevchip = n->samepinprevchip;
		n->samepinprevchip = temp;

		temp = m->samepinnextchip;
		m->samepinnextchip = n->samepinnextchip;
		n->samepinnextchip = temp;
	}

	//say("!!!!%x,%x,%x,%x\n",mprev,mnext,nprev,nnext);
	if(mprev != 0)
	{
		mprev->samepinnextchip = (void*)n - (void*)wirebuf;
		//say("1:%x->%x\n",mprev,n);
	}
	if(mnext != 0)
	{
		mnext->samepinprevchip = (void*)n - (void*)wirebuf;
		//say("2:%x->%x\n",mnext,n);
	}
	if(nprev != 0)
	{
		nprev->samepinnextchip = (void*)m - (void*)wirebuf;
		//say("3:%x->%x\n",nprev,m);
	}
	if(nnext != 0)
	{
		nnext->samepinprevchip = (void*)m - (void*)wirebuf;
		//say("4:%x->%x\n",nnext,m);
	}

	//relation_debug(uchip->irel);
}








void* relation_generate(
	void* uchip, u64 ufoot, u32 utype,
	void* bchip, u64 bfoot, u32 btype)
{
	struct relation* w = relation_grow();
	if(w == 0)return 0;

	//1.dest
	w->destchip = (u64)uchip;
	w->destfoot = ufoot;

	w->desttype = utype;
	w->destflag = 0;

	w->samepinprevchip = 0;
	w->samepinnextchip = 0;

	//2.self
	w->selfchip = (u64)bchip;
	w->selffoot = bfoot;

	w->selftype = btype;
	w->selfflag = 0;

	w->samechipprevpin = 0;
	w->samechipnextpin = 0;

	return w;
}
void relation_destory(struct relation* this)
{
	struct item* uchip;
	struct relation* prev;
	struct relation* next;
	if(this == 0)return;

	if(this->samepinprevchip == 0)prev = 0;
	else prev = (void*)wirebuf + (this->samepinprevchip);
	if(this->samepinnextchip == 0)next = 0;
	else next = (void*)wirebuf + (this->samepinnextchip);

	if(prev != 0)
	{
		if(next == 0)prev->samepinnextchip = 0;
		else prev->samepinnextchip = (void*)next - (void*)wirebuf;
	}
	if(next != 0)
	{
		if(prev == 0)next->samepinprevchip = 0;
		else next->samepinprevchip = (void*)prev - (void*)wirebuf;
	}

	uchip = (void*)(this->destchip);
	if(this == uchip->irel)uchip->irel = next;

	relation_recycle(this);
}




//wininfo,  position, 'win',  actor,    0, 'act'
//actinfo,  which,    'act',  userinfo, what,     'user'
int relation_write(
	void* uchip, u64 ufoot, u32 utype,
	void* bchip, u64 bfoot, u32 btype)
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
	if(h1->irel == 0)h1->irel = ww;
	else
	{
		wc = h1->irel;
		while(wc->samepinnextchip != 0)wc = (void*)wirebuf + (wc->samepinnextchip);
		wc->samepinnextchip = (void*)ww - (void*)wirebuf;
		ww->samepinprevchip = (void*)wc - (void*)wirebuf;
	}

	h2 = bchip;
	if(h2->orel == 0)h2->orel = ww;
	else
	{
		wc = h2->orel;
		while(wc->samechipnextpin != 0)wc = (void*)wirebuf + (wc->samechipnextpin);
		wc->samechipnextpin = (void*)ww - (void*)wirebuf;
		ww->samechipprevpin = (void*)wc - (void*)wirebuf;
	}

	return 1;
}




void* samepinprevchip(struct relation* rel)
{
	if(rel == 0)return 0;
	if(rel->samepinprevchip == 0)return 0;
	return (void*)wirebuf + (rel->samepinprevchip);
}
void* samepinnextchip(struct relation* rel)
{
	if(rel == 0)return 0;
	if(rel->samepinnextchip == 0)return 0;
	return (void*)wirebuf + (rel->samepinnextchip);
}
void* samechipprevpin(struct relation* rel)
{
	if(rel == 0)return 0;
	if(rel->samechipprevpin == 0)return 0;
	return (void*)wirebuf + (rel->samechipprevpin);
}
void* samechipnextpin(struct relation* rel)
{
	if(rel == 0)return 0;
	if(rel->samechipnextpin == 0)return 0;
	return (void*)wirebuf + (rel->samechipnextpin);
}
void* relation_read(int off)
{
	if(off == 0)return 0;
	return (void*)wirebuf + off;
}
