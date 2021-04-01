#include "libuser.h"




//libuser1
void entity_exit();
void entity_init(void*);
//libuser0
void supply_exit();
void supply_init(void*);
//libsoft1
void artery_exit();
void artery_init(void*);
//libsoft0
void system_exit();
void system_init(void*);
//libhard1
void driver_exit();
void driver_init(void*);
//libhard0
void device_exit();
void device_init(void*);
//libboot1
void bootup_exit();
void bootup_init(void*);
//libboot0
void origin_exit();
void origin_init(void*);
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
void birth(void* addr)
{
	//libboot
	origin_init(addr+0x000000);
	bootup_init(addr+0x200000);

	//libsoft
	device_init(addr+0x400000);
	driver_init(addr+0x600000);

	//libsoft
	system_init(addr+0x800000);
	artery_init(addr+0xa00000);

	//libuser
	supply_init(addr+0xc00000);
	entity_init(addr+0xe00000);
}
