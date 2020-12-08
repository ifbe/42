#include "libuser.h"




//libuser1
void freeentity();
void initentity(void*);
//libuser0
void freesupply();
void initsupply(void*);
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
void freebootup();
void initbootup(void*);
//libboot0
void freeorigin();
void initorigin(void*);
void death()
{
	//libuser
	freeentity();
	freesupply();

	//libsoft
	freeartery();
	freesystem();

	//libhard
	freedriver();
	freedevice();

	//libboot
	freebootup();
	freeorigin();
}
void birth(void* addr)
{
	//libboot
	initorigin(addr+0x000000);
	initbootup(addr+0x200000);

	//libsoft
	initdevice(addr+0x400000);
	initdriver(addr+0x600000);

	//libsoft
	initsystem(addr+0x800000);
	initartery(addr+0xa00000);

	//libuser
	initsupply(addr+0xc00000);
	initentity(addr+0xe00000);
}
