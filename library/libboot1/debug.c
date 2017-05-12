#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(char*,...);




//
static int enq = 0;
static int deq = 0;
static int lock = 0;
//
static char* inputqueue;        //stdin
static char* outputqueue;       //stdout
static char* journalqueue;      //stderr
static char* eventqueue;        //stdevent




void printbigint(u8* p, int i)
{
        int j;
        if(i<=0)return;

        say("0x");
        for(j=i-1;j>=0;j--)say("%02x",p[j]);
}
void printmemory(u8* addr,int size)
{
	int x,y;
	u8 ch;
	u8* p;
	say("\n");

	for(y=0;y<size/16;y++)
	{
		p = addr+y*16;

		//addr
		if((y%16) == 0)say( "@%-13llx" , (u64)p );
		else say("+%-13x",y*16);

		//hex
		for(x=0;x<=0xf;x++)
		{
			say("%02x ",p[x]);
		}

		//ascii
		for(x=0;x<=0xf;x++)
		{
			ch = p[x];
			if( (ch>=0x7f)|(ch<0x20) )ch=0x20;
			say("%c",ch);
		}
		say("\n");
	}
	if(y*16 >= size)return;

	p = addr+y*16;
	say("+%-13x",y*16);
	for(x=0;x<16;x++)
	{
		if(x >= size%16)say("   ");
		else say("%02x ",p[x]);
	}
	for(x=0;x<16;x++)
	{
		if(x >= size%16)say(" ");
		else
		{
			ch = p[x];
			if( (ch>=0x7f)|(ch<0x20) )ch=0x20;
			say("%c",ch);
		}
	}
	say("\n");
}




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




void debugcreate(char* type,char* addr)
{
	inputqueue = addr;
	outputqueue = addr+0x100000;
	journalqueue = addr+0x200000;
	eventqueue = addr+0x300000;
	//say("[0,4):createed debug\n");
}
void debugdelete()
{
	//say("[0,4):deleteing debug\n");
}
