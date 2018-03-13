#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//libuser1
#define _char_ hex32('c','h','a','r')
void freeactor();
void initactor(void*);
int actorread();
int actorwrite(void*);
//libuser0
#define _win_ hex32('w','i','n',0)
void freearena();
void initarena(void*);
int arenaread();
int arenawrite(void*);
//libsoft1
#define _fd_ hex32('f','d',0,0)
void freeartery();
void initartery(void*);
int arteryread();
int arterywrite(void*);
//libsoft0
#define _sys_ hex32('s','y','s',0)
void freesystem();
void initsystem(void*);
int systemread();
int systemwrite(void*);
int sleep_us(int);
//libhard1
#define _drv_ hex32('d','r','v',0)
void freedriver();
void initdriver(void*);
int driverread();
int driverwrite(void*);
//libhard0
#define _dev_ hex32('d','e','v',0)
void freedevice();
void initdevice(void*);
int deviceread();
int devicewrite(void*);
//libboot1
#define _hash_ hex32('h','a','s','h')
void freestdin();
void initstdin(void*);
void freestdout();
void initstdout(void*);
void freestdev();
void initstdev(void*);
void freestdrel();
void initstdrel(void*);
int term_read(void*);
int term_write(void*);
//libboot0
#define _0101_ hex32('0','1','0','1')
void death();
void* birth();
void eventwrite(u64,u64,u64,u64);
void* eventread();
void printmemory(void*, int);
void say(void*, ...);
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
	//libboot
	void* addr = birth();
	initstdin( addr+0x000000);
	initstdout(addr+0x100000);
	initstdev( addr+0x200000);
	initstdrel(addr+0x300000);

	//libsoft
	initdevice(addr+0x400000);
	initdriver(addr+0x400000);

	//libsoft
	initsystem(addr+0x800000);
	initartery(addr+0x800000);

	//libuser
	initarena(addr+0xc00000);
	initactor(addr+0xc00000);

	return addr;
}
void afterdusk()
{
	//libuser
	freeactor();
	freearena();

	//libsoft
	freeartery();
	freesystem();

	//libhard
	freedriver();
	freedevice();

	//libboot
	freestdev();
	freestdrel();
	freestdout();
	freestdin();
	death();
}
int main(int argc, char* argv[])
{
	int ret;
	void* addr;
	struct event* ev;

	addr = beforedawn();
	for(ret=1;ret<argc;ret++)
	{
		fixarg(addr, argv[ret]);
		term_read(addr);
	}
	term_write("\n");

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

		//say("ev:%x,%x,%x,%x\n",ev->why,ev->what,ev->where,ev->when);
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
