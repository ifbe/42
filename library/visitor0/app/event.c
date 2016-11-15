#include<stdio.h>
#include<stdlib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//libsoft
void sleep_us(int);
//libhard
int snatch(void*);
int release(void*);
//libboot
void say(char*,...);




//
int lock = 0;
//
static int enq = 0;
static int deq = 0;
static char eventqueue[0x100000];




void* eventread()
{
	int ret;
	while(enq == deq)
	{
		sleep_us(10000);
	}
	ret = deq;
	deq = (deq+0x20)%0x100000;
	return eventqueue + ret;
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
		say("queue full, droping event!\n");
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
