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
int actorread_all(void*);
int actorwrite_ev(void*);
int arenaread_all(void*);
int arenawrite_ev(void*);
int arteryread_all(void*);
int arterywrite_ev(void*);
int systemread_all(void*);
int systemwrite_ev(void*);
int driverread_all(void*);
int driverwrite_ev(void*);
int deviceread_all(void*);
int devicewrite_ev(void*);
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




void eventhandler(struct event* ev)
{
	int ret;
	//say("ev:%x,%x,%x,%x\n",ev->why,ev->what,ev->where,ev->when);

	if((_char_ == ev->what)&&(0 == ev->where))termwrite(ev, 0);
	else if(_dev_ == ev->what)devicewrite_ev(ev);
	else if(_dri_ == ev->what)driverwrite_ev(ev);
	else if(_fd_  == ev->what)systemwrite_ev(ev);
	else if(_art_ == ev->what)arterywrite_ev(ev);
	else if(_win_ == ret)arenawrite_ev(ev);
	else actorwrite_ev(ev);
}
void loop()
{
	//before
	u64 t0;
	u64 dt;
	void* dbg;
	void* win;
	struct event* ev;

	//main thread
	dbg = arenacreate(_dbg_, 0);
	win = arenacreate(_win_,  0);
	while(alive)
	{
		//draw frame
		t0 = timeread();
		windowread(win);

		//cleanup events
		while(1)
		{
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return;

			eventhandler(ev);
		}

		//max fps
		dt = timeread() - t0;
		//say("dt=%d\n", delta);
		if(dt < 16000)sleep_us(16000-dt);
	}
	arenadelete(win);
	//arenadelete(dbg);
}
