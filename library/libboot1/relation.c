#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define maxlen 0x100000
#define __act__ hex32('a','c','t',0)
#define __win__ hex32('w','i','n',0)
void say(void*, ...);




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
static u8* wirebuf;
static int wirecur = 0x40;
static int wirelen = 0x100000;




void initrelation(void* addr)
{
	wirebuf = addr+0x300000;
}
void* relation_generate(
	void* uchip, u64 ufoot, u32 utype,
	void* bchip, u64 bfoot, u32 btype)
{
	struct relation* w = (void*)wirebuf + wirecur;
	wirecur += sizeof(struct relation);

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
void* relation_read(int off)
{
	if(off == 0)return 0;
	return (void*)wirebuf + off;
}
