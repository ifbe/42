#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//
int actorlist(void*);
int actorchoose(void*);
int arterylist(void*);
int arterychoose(void*);
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




static u8* input = 0;
static int enq = 0;
void initstdin(void* addr)
{
	input = addr;
}




void term_read(u8* buf)
{
	int j;
	if(buf == 0)goto prompt;
	if( (buf[0] == 'q') && (buf[1] < 0x20) )goto finish;
	if(ncmp(buf, "exit", 4) == 0)goto finish;

	//proto://ipaddr:port/folder/file
	for(j=0;j<0x1000;j++)
	{
		if(0 == ncmp(buf+j, "://", 3))
		{
			arterychoose(buf);
			goto prompt;
		}
	}

	if(0 == ncmp(buf, "say ", 4))
	{
		say("%s\n", buf+4);
	}
	else if(0 == ncmp(buf, "win", 3))
	{
		eventwrite(
			hex32('w','i','n',0),
			hex32('w','+',0,0),
			0, 0);
	}
	else if(0 == ncmp(buf, "ls", 2))
	{
		actorlist(0);
	}
	else if(0 == ncmp(buf, "cd", 2))
	{
	}
	else
	{
		actorchoose(buf);
	}

prompt:
	say("[void]");
	enq = 0;
	return;

finish:
	eventwrite(0,0,0,0);
	return;
}
void term_write(u8* p)
{
	int j;
	if(p == 0)return;
	if(p[0] >= 0x20)say("%s", p);

	//myself
	while(1)
	{
		if(*p < 8)return;
		if(*p == 0x9)	//tab
		{
			say("tab");
		}
		else if((*p==0x8)|(*p==0x7f))		//backspace
		{
			if(enq <= 0)return;
			say("\b \b");

			enq--;
			input[enq] = 0;
		}
		else if((*p==0xa)|(*p==0xd))	//enter
		{
			say("\n");
			input[enq] = 0;
			term_read(input);

			for(j=0;j<0x100;j++)input[j] = 0;
			enq = 0;
		}
		else
		{
			input[enq] = *p;
			enq++;
		}

		//////////////////
		p++;
	}
}