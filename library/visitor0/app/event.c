#include<stdio.h>
#include<stdlib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//libsoft
u64 gettime();
void sleep_us(int);
//libhard
void snatch(void*);
void release(void*);
//libboot
void say(char*,...);




//
static int enq = 0;
static int deq = 0;
static char eventqueue[0x100000];
//
static int lock = 0;
static u64 haha[4];




void* eventread()
{
	int ret;
	if(enq != deq)
	{
		ret = deq;
		deq = (deq+0x20)%0x100000;
		return eventqueue + ret;
	}

	//haha[0] = 0;
	haha[1] = 0x656d6974;
	//haha[2] = 0;
	haha[3] = gettime() + 10000;

	sleep_us(10000);
	return haha;
}




void eventwrite(u64 why, u64 what, u64 where, u64 when)
{
	int this,temp;
	static u64* p;

	//safely update the pointer
	snatch(&lock);
	this = enq;
	temp = (this+0x20)%0x100000;
	if(temp == deq)
	{
		//full
		release(&lock);
		say("droping event: %llx,%llx,%llx,%llx\n", why, what, where, when);
		return;
	}
	enq = temp;
	release(&lock);

	//put event to place
	p = (u64*)(eventqueue + this);
	p[0] = why;
	p[1] = what;
	p[2] = where;	//where
	p[3] = when;	//when

	//debug
	//say("%llx,%llx,%llx,%llx\n", p[0], p[1], p[2], p[3]);
}
