#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int netmgr_write(void*);
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
