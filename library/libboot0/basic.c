#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void createcpu();
void deletecpu();
//
void creatememory();
void deletememory();
//
void createserial();
void deleteserial();
//
void say(char* , ...);




//
static int enq = 0;
static int deq = 0;
static int lock = 0;
//
static char* inputqueue;	//stdin
static char* outputqueue;	//stdout
static char* journalqueue;	//stderr
static char* eventqueue;	//stdevent




void eventwrite(u64 why, u64 what, u64 where, u64 when)
{
	int this,temp;
	static u64* p;

	//safely update the pointer
	while(__sync_lock_test_and_set(&lock,1) == 1);
	this = enq;
	temp = (this+0x20)%0x100000;
	if(temp == deq)
	{
		//full
		__sync_lock_release(&lock);
		say("droping event: %llx,%llx,%llx,%llx\n", why, what, where, when);
		return;
	}
	enq = temp;
	__sync_lock_release(&lock);

	//put event to place
	p = (u64*)(eventqueue + this);
	p[0] = why;
	p[1] = what;
	p[2] = where;   //where
	p[3] = when;    //when

	//debug
	//say("%llx,%llx,%llx,%llx\n", p[0], p[1], p[2], p[3]);
}
void* eventread()
{
	int ret;
	if(enq == deq)return 0;

	ret = deq;
	deq = (deq+0x20)%0x100000;
	return eventqueue + ret;
}




void basiccreate(void* type, void* addr)
{
	inputqueue = addr;
	outputqueue = addr+0x100000;
	journalqueue = addr+0x200000;
	eventqueue = addr+0x300000;

	//createcpu();
	//creatememory();
	createserial();
	//say("[0,4):createed basic\n");
}
void basicdelete()
{
	//say("[0,4):deleteing basic\n");
	deleteserial();
	//deletememory();
	//deletecpu();
}
