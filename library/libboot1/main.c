#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//libuser
#define _char_ hex32('c','h','a','r')
#define _win_ hex32('w','i','n',0)
int actorread();
int actorwrite(void*);
int arenaread();
int arenawrite(void*);
//libsoft
#define _fd_ hex32('f','d',0,0)
#define _sys_ hex32('s','y','s',0)
int arteryread();
int arterywrite(void*);
int systemread();
int systemwrite(void*);
void sleep_us(int);
//libhard
#define _drv_ hex32('d','r','v',0)
#define _dev_ hex32('i','n','t',0)
int driverread();
int driverwrite(void*);
int deviceread();
int devicewrite(void*);
//libboot
int term_read(void*);
int term_write(void*);
void eventwrite(u64,u64,u64,u64);
void* eventread();
void printmemory(void*, int);
void say(void*, ...);
//
void actorcreate(void*);
void actordelete();
void arenacreate(void*);
void arenadelete();
void arterycreate(void*);
void arterydelete();
void systemcreate(void*);
void systemdelete();
void drivercreate(void*);
void driverdelete();
void devicecreate(void*);
void devicedelete();
//
void initstdin(void*);
void initstdout(void*);
void initstdev(void*);
void initstdrel(void*);
//
void* birth();
void death();
void fixarg(void*, void*);




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};




void* beforedawn()
{
	//libboot0
	void* addr = birth();

	//libboot1
	initstdin( addr+0x000000);
	initstdout(addr+0x100000);
	initstdev( addr+0x200000);
	initstdrel(addr+0x300000);

	//libsoft
	devicecreate(addr+0x400000);
	drivercreate(addr+0x400000);

	//libsoft
	systemcreate(addr+0x800000);
	arterycreate(addr+0x800000);

	//libuser
	arenacreate(addr+0xc00000);
	actorcreate(addr+0xc00000);

	return addr;
}
void afterdusk()
{
	//libuser
	actordelete();
	arenadelete();

	//libsoft
	arterydelete();
	systemdelete();

	//libhard
	driverdelete();
	devicedelete();

	//libboot
	death();
}
int main(int argc, char* argv[])
{
	int ret;
	void* addr;
	struct event* ev;

	addr = beforedawn();
	term_read(0);

	for(ret=1;ret<argc;ret++)
	{
		fixarg(addr, argv[ret]);
		term_read(addr);
	}

	while(1)
	{
		actorread();
		//say("after\n");

again:
		ev = eventread();
		if(0 == ev)
		{
			sleep_us(1000);
			goto again;
		}
		if(0 == ev->what)break;

		say("ev:%x,%x,%x,%x\n",ev->why,ev->what,ev->where,ev->when);
		if(_char_ == ev->what)
		{
			if(0 == ev->where)
			{
				term_write(ev);
				continue;
			}
		}

		//libhard0
		if(_dev_ == ev->what)
		{
			ret = devicewrite(ev);
			if(ret != 42)goto again;
		}

		//libhard1
		if(_drv_ == ev->what)
		{
			ret = driverwrite(ev);
			if(ret != 42)goto again;
		}

		//libsoft0
		if(_sys_ == ev->what)
		{
			ret = systemwrite(ev);
			if(ret != 42)goto again;
		}

		//libsoft1
		ret = (ev->what)&0xff;
		if(_fd_ == ev->what)
		{
			ret = arterywrite(ev);
			if(ret != 42)goto again;
		}

		//libuser0
		ret = (ev->what)&0xff;
		if('w' == ret)
		{
			arenawrite(ev);
			continue;
		}

		//libuser1
		actorwrite(ev);
	}

	afterdusk();
	return 0;
}
