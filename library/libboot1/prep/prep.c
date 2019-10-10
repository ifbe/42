#include "libuser.h"




//libuser1
void freeactor();
void initactor(void*);
//libuser0
void freearena();
void initarena(void*);
//libsoft1
void freeartery();
void initartery(void*);
//libsoft0
void freesystem();
void initsystem(void*);
//libhard1
void freedriver();
void initdriver(void*);
//libhard0
void freedevice();
void initdevice(void*);
//libboot1
void freeworker();
void initworker(void*);
//libboot0
void freepwrclk();
void initpwrclk(void*);
void death()
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
	freeworker();
	freepwrclk();
}
void birth(void* addr)
{
	//libboot
	initpwrclk(addr+0x000000);
	initworker(addr+0x200000);

	//libsoft
	initdevice(addr+0x400000);
	initdriver(addr+0x600000);

	//libsoft
	initsystem(addr+0x800000);
	initartery(addr+0xa00000);

	//libuser
	initarena( addr+0xc00000);
	initactor( addr+0xe00000);
}