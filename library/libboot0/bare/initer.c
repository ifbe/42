#include "libuser.h"
void inithardware();




//libuser1
void entity_exit();
void entity_init(void*,int);
//libuser0
void supply_exit();
void supply_init(void*,int);
//libsoft1
void artery_exit();
void artery_init(void*,int);
//libsoft0
void system_exit();
void system_init(void*,int);
//libhard1
void driver_exit();
void driver_init(void*,int);
//libhard0
void device_exit();
void device_init(void*,int);
//libboot1
void bootup_exit();
void bootup_init(void*,int);
//libboot0
void origin_exit();
void origin_init(void*,int);
void death()
{
	//libuser
	entity_exit();
	supply_exit();

	//libsoft
	artery_exit();
	system_exit();

	//libhard
	driver_exit();
	device_exit();

	//libboot
	bootup_exit();
	origin_exit();
}
void birth(void* addr, int size)
{
	int eachtier = 0x200000;

	//libboot
	origin_init(addr+0x000000, eachtier);
	bootup_init(addr+0x200000, eachtier);

	//libsoft
	device_init(addr+0x400000, eachtier);
	driver_init(addr+0x600000, eachtier);

	//libsoft
	system_init(addr+0x800000, eachtier);
	artery_init(addr+0xa00000, eachtier);

	//libuser
	supply_init(addr+0xc00000, eachtier);
	entity_init(addr+0xe00000, eachtier);

	//hardware prepare
	inithardware();
}
