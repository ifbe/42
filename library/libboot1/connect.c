#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define maxlen 0x100000
#define __act__ hex32('a','c','t',0)
#define __win__ hex32('w','i','n',0)
struct relation
{
	//[0x00,0x1f]
	u64 destchip;
	u64 destfoot;
	u64 desttype;
	int samepinprevchip;
	int samepinnextchip;

	//0x20,0x3f
	u64 selfchip;
	u64 selffoot;
	u64 selftype;
	int samechipprevpin;
	int samechipnextpin;
};
struct item
{
	u64 type;
	u64 fmt;
	union{
		void* first;
		u64 pad0;
	};
	union{
		void* last;
		u64 pad1;
	};
};
static u8 connect[0x100000];
static int wirelen = 0;




void* connect_write_new(
	void* uchip, u64 ufoot, u64 utype,
	void* bchip, u64 bfoot, u64 btype)
{
	struct relation* w;
	if(wirelen > maxlen-0x40)return 0;

	w = (void*)connect + wirelen;
	wirelen += sizeof(struct relation);

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
	struct item* h1;
	struct item* h2;
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