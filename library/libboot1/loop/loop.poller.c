typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
int actorread_all();
int actorevent(void*);
int arenaread_all();
int arenaevent(void*);
//
u64 timeread();
void sleep_us();
//
void* eventread();
void eventwrite(u64,u64,u64,u64);
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




void poller()
{
	//before
	u64 t0;
	u64 dt;
	struct event* ev;

	//forever
	while(alive)
	{
		//cur time
		t0 = timeread();

		//draw frame
		actorread_all();
		arenaread_all();

		//cleanup events
		while(1)
		{
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return;

			arenaevent(ev);
		}

		//max fps
		dt = timeread() - t0;
		//say("dt=%d\n", delta);
		if(dt < 16000)sleep_us(16000-dt);
	}
}
