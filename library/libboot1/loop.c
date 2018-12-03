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
int actorread_all();
int actorevent(void*);
int arenaread_all();
int arenaevent(void*);
int arteryread_all();
int arteryevent(void*);
int systemread_all();
int systemevent(void*);
int driverread_all();
int driverevent(void*);
int deviceread_all();
int deviceevent(void*);
//
void* arenacreate(u64,u64);
void arenadelete(void*);
//
u64 timeread();
void timewrite(u64);
void sleep_us();
//
void* eventread();
void eventwrite(u64,u64,u64,u64);
//i
int input(void*, int);
//o
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

	if(_dev_ == ev->what)deviceevent(ev);
	else if(_dri_ == ev->what)driverevent(ev);
	else if(_fd_  == ev->what)systemevent(ev);
	else if(_art_ == ev->what)arteryevent(ev);
	else arenaevent(ev);
}
void loop()
{
	//before
	u64 t0;
	u64 dt;
	void* win;
	struct event* ev;

	//+libhard
	//ahci = devicecreate()
	//xhci = devicecreate()

	//+libsoft
	//hack = arterycreate("HACK://127.0.0.1:2222");
	//hack = arterycreate("CHAT://127.0.0.1:4444");
	//sshd = arterycreate( "SSH://127.0.0.1:2222");
	//http = arterycreate("HTTP://127.0.0.1:4444");

	//+libuser
	win = arenacreate(_win_,  0);

	//forever
	while(alive)
	{
		//cur time
		t0 = timeread();

		//draw frame
		//actorread_all();
		arenaread_all();

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

	//-libuser
	arenadelete(win);

	//-libsoft

	//-libhard
}
