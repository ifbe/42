#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _HTTP_ hex32('H','T','T','P')
//libuser1
#define _char_ hex32('c','h','a','r')
void freeactor();
void initactor(void*);
int actorread_all();
int actorwrite_ev(void*);
//libuser0
#define _win_ hex32('w','i','n',0)
void freearena();
void initarena(void*);
int arenaread_all();
int arenawrite_ev(void*);
void* arenacreate(u64, void*);
void arenadelete(void*);
//libsoft1
#define _art_ hex32('a','r','t',0)
void freeartery();
void initartery(void*);
int arteryread_all();
int arterywrite_ev(void*);
//libsoft0
#define _fd_ hex32('f','d',0,0)
void freesystem();
void initsystem(void*);
int systemread_all();
int systemwrite_ev(void*);
int sleep_us(int);
//libhard1
#define _dri_ hex32('d','r','v',0)
void freedriver();
void initdriver(void*);
int driverread_all();
int driverwrite_ev(void*);
//libhard0
#define _dev_ hex32('d','e','v',0)
void freedevice();
void initdevice(void*);
int deviceread_all();
int devicewrite_ev(void*);
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
void* death();
void* birth();
void* eventwrite(u64,u64,u64,u64);
void* eventread();
void* threadcreate(void*, void*);
void* threaddelete(u64);
//
void openwriteclose(void*,int,void*,int);
void fixarg(void*, void*);
void printmemory(void*, int);
void say(void*, ...);




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
	struct event* ev;
	void* addr = beforedawn();

	//args
	for(ret=1;ret<argc;ret++)
	{
		fixarg(addr, argv[ret]);
		term_read(addr);
	}
	term_write("\n");

	//win0
	arenacreate(_win_, 0);
	arenacreate(_HTTP_, 0);

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
		if(0 == ev->what)break;

		//say("ev:%x,%x,%x,%x\n",ev->why,ev->what,ev->where,ev->when);
		if((_char_ == ev->what)&&(0 == ev->where))
		{
			term_write(ev);
			continue;
		}
/*
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
*/
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

	openwriteclose("universe.bin",0,addr,0x1000000);
	afterdusk();
	return 0;
}


/*
#include <efi.h>
#include <efilib.h>
EFI_STATUS efi_hack(EFI_HANDLE handle, EFI_SYSTEM_TABLE *table);
EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *table)
{
	efi_hack(handle, table);

	main(0, 0);

	return EFI_SUCCESS;
}
void atexit(){}
*/


#ifdef __ANDROID__
#include <stdio.h>
#include <stdlib.h>
void app_dummy();
void android_loop(void* app);
void android_main(void* app)
{
	app_dummy();

	threadcreate(main, 0);

	android_loop(app);

	exit(0);
}
#endif