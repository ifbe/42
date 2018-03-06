#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _device_ hex64('d','e','v','i','c','e',0,0)
#define _driver_ hex64('d','r','i','v','e','r',0,0)
#define _system_ hex64('s','y','s','t','e','m',0,0)
#define _artery_ hex64('a','r','t','e','r','y',0,0)
#define _arena_ hex64('a','r','e','n','a',0,0,0)
#define _actor_ hex64('a','c','t','o','r',0,0,0)
//
int actorlist(void*);
int actorchoose(void*);
int arenalist(void*);
int arenachoose(void*);
int arterylist(void*);
int arterychoose(void*);
int systemlist(void*);
int systemchoose(void*);
int driverlist(void*);
int driverchoose(void*);
int devicelist(void*);
int devicechoose(void*);
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




static u64 path[8];
static int pos = 0;
static u8* input = 0;
static int enq = 0;
void initstdin(void* addr)
{
	input = addr;
	enq = 0;
}



void term_ls(u8* buf)
{
	int j;
	if(0 == buf)goto fail;
	if(buf[2] < 0x20)
	{
		if(0 == pos)goto fail;
		else if(_device_ == path[0])devicelist(0);
		else if(_driver_ == path[0])driverlist(0);
		else if(_system_ == path[0])systemlist(0);
		else if(_artery_ == path[0])arterylist(0);
		else if(_arena_ == path[0])arenalist(0);
		else if(_actor_ == path[0])actorlist(0);
		return;
	}

	for(j=2;j<0x1000;j++)
	{
		if(buf[j] > 0x20)break;
	}

	say("ls(%s)\n", buf+j);
	return;

fail:
	say("----------------\n");
	devicelist(0);
	say("----------------\n");
	driverlist(0);
	say("----------------\n");
	systemlist(0);
	say("----------------\n");
	arterylist(0);
	say("----------------\n");
	arenalist(0);
	say("----------------\n");
	actorlist(0);
}
void term_cd(u8* buf)
{
	int j,k;
	u8* pp;
	if(0 == buf)goto fail;
	if(buf[2] < 0x20)goto fail;

	for(j=2;j<0x1000;j++)
	{
		if(buf[j] > 0x20)break;
	}

	path[pos] = 0;
	pp = (u8*)&path[pos];
	for(k=0;k<8;k++)
	{
		if(buf[j+k] < 0x20)break;
		pp[k] = buf[j+k];
	}
	pos += 1;
	return;

fail:
	pos = 0;
}
void term_cmd(u8* buf)
{
	if(0 == pos)goto fail;
	else if(_device_ == path[0])devicechoose(buf);
	else if(_driver_ == path[0])driverchoose(buf);
	else if(_system_ == path[0])systemchoose(buf);
	else if(_artery_ == path[0])arterychoose(buf);
	else if(_arena_ == path[0])arenachoose(buf);
	else if(_actor_ == path[0])actorchoose(buf);
	else goto fail;
	return;
	
fail:
	say("?: %s\n", buf);
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

	if(0 == ncmp(buf, "ls", 2))term_ls(buf);
	else if(0 == ncmp(buf, "cd", 2))term_cd(buf);
	else term_cmd(buf);

prompt:
	if(0 != pos)
	{
		say("[");
		for(j=0;j<pos;j++)say("%s/", &path[j]);
		say("]");
	}
	else say("[void]");
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