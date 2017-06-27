#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int actorlist(void*);
int actorchoose(void*);
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void say(void*, ...);




static char* input = 0;
static char* output = 0;




void cli_create(void* addr)
{
	input = addr + 0x400000;
	output = addr + 0x500000;
}
void cli_delete()
{
}
void cli_start()
{
}
void cli_stop()
{
}
void cli_list()
{
}
void cli_choose()
{
}
void cli_read()
{
	say("[void]");
}
void cli_write(u64* ev)
{
	int j;
	int* enq;
	u8* p;
	if(ev == 0)return;
	if((ev[1] != 0x64626b) && (ev[1] != 0x72616863))return;

	//
	p = (void*)ev;
	enq = (void*)(input+0xffff0);
	while(*p != 0)
	{
		j = *p;
		if(j == 0xd)j = 0xa;
		p++;
		say("%c",j);

		if(j == 0x8)		//backspace
		{
			if(*enq <= 0)return;
			(*enq)--;
			input[*enq] = 0;
			return;
		}
		else if(j == 0x1b)
		{
			say("esc\n");
			return;
		}
		else if(j == 0xa)	//enter
		{
			j = actorchoose(input);
			if(j == 0)
			{
				if(ncmp(input, "ls", 2) == 0)actorlist(0);
				else if(ncmp(input, "cd", 2) == 0)actorlist(0);

				for(j=0;j<0x80;j++)input[j] = 0;
				*enq = 0;
			}

			cli_read();
			return;
		}
		else
		{
			input[*enq] = j;
			(*enq)++;
			return;
		}
	}
}