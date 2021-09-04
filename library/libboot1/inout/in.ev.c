#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void* pollenv();
void say(char*, ...);




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




void eventwrite(u64 why, u64 what, u64 where, u64 when)
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
		say("droping event: %llx,%llx,%llx,%llx\n", why, what, where, when);
		return;
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
	//say("%llx,%llx,%llx,%llx\n", p[0], p[1], p[2], p[3]);
}
void* eventread()
{
	int ret;
	if(enq == deq)return pollenv();

	ret = deq;
	deq = (deq+0x20)%0x40000;
	return eventqueue + ret;
}