#include "libboot.h"
void* pollenv();




//
static void* eventqueue;
static int lock = 0;
static int enq = 0;
static int deq = 0;
void initstdev(void* addr)
{
	eventqueue = addr;
}
void freestdev()
{
}


static struct item* boardserialnode = 0;
void stdev_setboardserial(void* node)
{
	//asm("int3");
	boardserialnode = node;
}


void* eventwrite(u64 why, u64 what, u64 where, u64 when)
{
	int this,temp;
	static u64* p;

	//safely update the pointer
	while(__sync_lock_test_and_set(&lock,1) == 1);
	this = enq;
	temp = (this+0x20)%0x40000;
	if(temp == deq)
	{
		//full
		__sync_lock_release(&lock);
		logtoall("droping event: %llx,%llx,%llx,%llx\n", why, what, where, when);
		return 0;
	}

	//put event to place
	p = (u64*)(eventqueue + this);
	p[0] = why;
	p[1] = what;
	p[2] = where;
	p[3] = when;

	enq = temp;
	__sync_lock_release(&lock);

	//debug
	//logtoall("%llx,%llx,%llx,%llx\n", p[0], p[1], p[2], p[3]);
	return p;
}


static u64 tmpev[4];
void* eventread()
{
	//from queue
	if(enq != deq){
		int ret = deq;
		deq = (deq+0x20)%0x40000;
		return eventqueue + ret;
	}

	//from serial
	if(boardserialnode){
		int ret = boardserialnode->onreader(boardserialnode,0, 0,0, (u8*)&tmpev[0], 1);
		if(ret > 0){
			tmpev[1] = hex32('c','h','a','r');
			return tmpev;
		}
	}

	//from env
	return pollenv();
}