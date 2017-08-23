#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int arenastart(void*);
int arenastop(void*);
int actorlist(void*);
int actorchoose(void*);
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




static char* input = 0;
static char* output = 0;




void term_read(u8* input)
{
	//say("here\n");
	if(ncmp(input, "exit", 2) == 0)
	{
		eventwrite(0,0,0,0);
		return;
	}
	else if(ncmp(input, "ls", 2) == 0)actorlist(0);
	else if(ncmp(input, "cd", 2) == 0)actorlist(0);
	else if(ncmp(input, "new", 3) == 0)arenastart(0);

	//command prompt
	say("[void]");
}
void term_write(u8* p)
{
	int j;
	int* enq;
	if(p == 0)return;

	enq = (void*)(input+0xffff0);
	while(1)
	{
		if(*p < 8)return;
		if(*p == 0x1b)
		{
			say("esc\n");
		}
		if((*p==0x8)|(*p==0x7f))		//backspace
		{
			say("\b \b");
			if(*enq <= 0)return;

			(*enq)--;
			input[*enq] = 0;
		}
		else if((*p==0xa)|(*p==0xd))	//enter
		{
			say("\n");
			term_read(input);

			for(j=0;j<0x100;j++)input[j] = 0;
			*enq = 0;
		}
		else
		{
			say("%c",*p);
			input[*enq] = *p;
			(*enq)++;
		}

		//////////////////
		p++;
	}
}
void term_list()
{
}
void term_choose()
{
}
void term_start()
{
}
void term_stop()
{
}
void term_create(void* addr)
{
	input = addr + 0x400000;
	output = addr + 0x500000;
}
void term_delete()
{
}