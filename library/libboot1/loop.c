typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define _char_ hex32('c','h','a','r')
#define _dbg_  hex32('d','b','g', 0)
#define _win_  hex32('w','i','n', 0)
#define _dev_  hex32('d','e','v', 0)
#define _dri_  hex32('d','r','i', 0)
#define _fd_   hex32('f','d', 0 , 0)
#define _art_  hex32('a','r','t', 0)
//
int termread();
int termwrite(void*, int);
//
void* arenacreate(u64,u64);
void arenadelete(void*);
void windowread(void*);
//
u64 timeread();
void timewrite(u64);
void sleep_us();
//
void* threadcreate(void*, void*);
void* threaddelete(u64);
void* eventread();
void eventwrite(u64,u64,u64,u64);
//
void printmemory(void*, int);
void say(void*, ...);




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
static int alive = 1;
static u64 delta = 0;




void aide()
{
	int ret;
	struct event* ev;
	while(1)
	{
		//force redraw
		//actorread_all();
		//arenaread_all();
		//arteryread_all();
		//systemread_all();

again:
		ev = eventread();
		if(0 == ev)continue;
		if(0 == ev->what){alive = 0;return;}

		//say("ev:%x,%x,%x,%x\n",ev->why,ev->what,ev->where,ev->when);
		if((_char_ == ev->what)&&(0 == ev->where))
		{
			termwrite(ev, 0);
			continue;
		}

		//libhard0
		if(_dev_ == ev->what)
		{
			ret = devicewrite_ev(ev);
			if(ret != 42)goto again;
		}

		//libhard1
		if(_dri_ == ev->what)
		{
			ret = driverwrite_ev(ev);
			if(ret != 42)goto again;
		}

		//libsoft0
		if(_fd_ == ev->what)
		{
			ret = systemwrite_ev(ev);
			if(42 == ret)continue;
			else goto again;
		}

		//libsoft1
		if(_art_ == ev->what)
		{
			ret = arterywrite_ev(ev);
			if(42 == ret)continue;
			else goto again;
		}

		//libuser0
		ret = (ev->what)&0xff;
		if('w' == ret)
		{
			ret = arenawrite_ev(ev);
			continue;
		}

		//libuser1
		actorwrite_ev(ev);
	}
}
void loop()
{
	//before
	u64 j,k;
	void* dbg;
	void* win;

	//help thread
	threadcreate(aide, 0);

	//main thread
	dbg = arenacreate(_dbg_, 0);
	win = arenacreate(_win_,  0);
	while(alive)
	{
		//draw frame, cleanup events
		j = timeread();
		windowread(win);
		k = timeread();

		//max fps
		delta = k-j;
		//say("dt=%d\n", delta);
		if(delta < 16000)sleep_us(16000-delta);
	}
	arenadelete(win);
}