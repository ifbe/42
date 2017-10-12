#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int netmgr_write(void*);
//
int uart_list();
int uart_choose();
int uart_write(void*);
//
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
static int combo = 0;
static int goooo = 0;




void term_read(u8* buf)
{
	//say("here\n");
	if( (buf[0] == 'q') && (buf[1] < 0x20) )goto finish;
	if(ncmp(buf, "exit", 4) == 0)goto finish;

	if(ncmp(buf, "say ", 4) == 0)
	{
		say("%s\n", buf+4);
	}
	else if(ncmp(buf, "ls", 2) == 0)
	{
		actorlist(0);
	}
	else if(ncmp(buf, "cd", 2) == 0)
	{
	}
	else if(ncmp(buf, "net ", 4) == 0)
	{
		netmgr_write(buf+4);
	}
	else if(ncmp(buf, "i2c ", 4) == 0)
	{
	}
	else if(ncmp(buf, "uart ", 5) == 0)
	{
		if(ncmp(buf+5, "ls", 2) == 0)uart_list();
		else
		{
			uart_choose(buf+5);
			goooo = 1;
		}
	}

	//command prompt
	say("[void]");
	return;

finish:
	eventwrite(0,0,0,0);
	return;
}
void term_write(u8* p)
{
	int j;
	int* enq;

	//passthrough?
	if((p[0] == 0x1b)&&(*(p+1) != 0x5b))
	{
		combo++;
		if(combo >= 2)
		{
			combo = 0;
			goooo = 0;
		}
	}
	else combo = 0;

	//passthrough!
	if(p == 0)return;
	if(goooo == 1)
	{
		uart_write(p);
		return;
	}
	else if(goooo == 2)
	{
		return;
	}

	//myself
	enq = (void*)(input+0xffff0);
	while(1)
	{
		if(*p < 8)return;
		if(*p == 0x9)	//tab
		{
			say("tab");
		}
		else if((*p==0x8)|(*p==0x7f))		//backspace
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
