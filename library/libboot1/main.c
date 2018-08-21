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
//libuser1
void freeactor();
void initactor(void*);
int actorread_all(void*);
int actorwrite_ev(void*);
//libuser0
void freearena();
void initarena(void*);
int arenaread_all(void*);
int arenawrite_ev(void*);
//libsoft1
void freeartery();
void initartery(void*);
int arteryread_all(void*);
int arterywrite_ev(void*);
//libsoft0
void freesystem();
void initsystem(void*);
int systemread_all(void*);
int systemwrite_ev(void*);
//libhard1
void freedriver();
void initdriver(void*);
int driverread_all(void*);
int driverwrite_ev(void*);
//libhard0
void freedevice();
void initdevice(void*);
int deviceread_all(void*);
int devicewrite_ev(void*);
//libboot1
void freestdin();
void initstdin(void*);
void freestdout();
void initstdout(void*);
void freestdev();
void initstdev(void*);
void freestdrel();
void initstdrel(void*);
//libboot0
void* death();
void* birth();
//
void* arenacreate(u64,u64);
void arenadelete(void*);
void arenaread(void*);
void arenawrite(void*);
//
void* threadcreate(void*, void*);
void* threaddelete(u64);
void* eventread();
void eventwrite(u64,u64,u64,u64);
int termread();
int termwrite(void*, int);
//
int argv2line(void*, void*);
int openwriteclose(void*,int,void*,int);
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




void* beforedawn()
{
	//allocate
	void* addr = birth();

	//libboot
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

	//cleanup
	death();
}
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
int main(int argc, char* argv[])
{
	//before
	int j,k;
	void* dbg;
	void* win;
	u8* addr = beforedawn();

	//cmdline
	k = 0;
	for(j=1;j<argc;j++){k += argv2line(argv[j], addr+k);}
	termwrite(addr, k);

	//help thread
	threadcreate(aide, 0);

	//main thread
	dbg = arenacreate(_dbg_, 0);
	win = arenacreate(_win_,  0);
	while(alive)
	{
		//draw frame, cleanup events
		arenaread(win);
	}
	arenadelete(win);

	//after
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
void setapp(void*);
void android_main(void* app)
{
	app_dummy();

	setapp(app);
	main(0, 0);

	exit(0);
}
#endif
