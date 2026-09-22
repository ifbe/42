#include "libuser.h"
void* memory_rsvd_nodepool(int*);
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




//
void birth_at(void* addr, int size)
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
}
void birth()
{
	int len;
	u8* buf = memory_rsvd_nodepool(&len);		//at 16MB
	birth_at(buf, len);
}
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




//
void example_rpi()
{
	//
	birth();

	//hardware prepare
	inithardware();

	//init world, store args
	void* all = origin_alloc_fromarg(_start_, 0, 0 ,0);
	origin_create(all, 0, 0, 0);
	//call subcmd, until return
	void* thr = bootup_alloc_fromtype(_kernel_);
	bootup_create(thr, 0, 0, 0);

	bootup_delete(thr);
	origin_delete(all);

	death();
}
void example_efi()
{
	//
	birth();

	//hardware prepare
	inithardware();

	//init world, store args
	void* all = origin_alloc_fromarg(_efimain_, 0, 0 ,0);
	origin_create(all, 0, 0, 0);
	//call subcmd, until return
	void* thr = bootup_alloc_fromtype(_kernel_);
	bootup_create(thr, 0, 0, 0);

	bootup_delete(thr);
	origin_delete(all);

	death();
}