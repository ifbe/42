#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _win_ hex32('w','i','n',0)
#define _act_ hex32('a','c','t',0)
#define _device_ hex64('d','e','v','i','c','e',0,0)
#define _driver_ hex64('d','r','i','v','e','r',0,0)
#define _system_ hex64('s','y','s','t','e','m',0,0)
#define _artery_ hex64('a','r','t','e','r','y',0,0)
#define _arena_ hex64('a','r','e','n','a',0,0,0)
#define _actor_ hex64('a','c','t','o','r',0,0,0)
//
void* actorlist(void*, int);
int actorchoose(void*, int);
void* arenalist(void*, int);
int arenachoose(void*, int);
void* arterylist(void*, int);
int arterychoose(void*, int);
void* systemlist(void*, int);
int systemchoose(void*, int);
void* driverlist(void*, int);
int driverchoose(void*, int);
void* devicelist(void*, int);
int devicechoose(void*, int);
//
void* allocstyle();
void* allocpinid();
int parsestyle(void*, void*, int);
int parsepinid(void*, void*, int);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void relation_write(void*, void*, int, void*, void*, int);
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




static u64 path[8];
static int pos = 0;
static u8* input = 0;
static int enq = 0;




void term_ls0(u8* buf)
{
	say("----------------\n");
	devicelist(0, 0);
	say("----------------\n");
	driverlist(0, 0);
	say("----------------\n");
	systemlist(0, 0);
	say("----------------\n");
	arterylist(0, 0);
	say("----------------\n");
	arenalist(0, 0);
	say("----------------\n");
	actorlist(0, 0);
}
void term_lsn(u8* buf)
{
	int j;
	if(buf[2] < 0x20)
	{
		if(_device_ == path[0])devicelist(0, 0);
		else if(_driver_ == path[0])driverlist(0, 0);
		else if(_system_ == path[0])systemlist(0, 0);
		else if(_artery_ == path[0])arterylist(0, 0);
		else if(_arena_ == path[0])arenalist(0, 0);
		else if(_actor_ == path[0])actorlist(0, 0);
		return;
	}
	for(j=2;j<0x1000;j++){if(buf[j] > 0x20)break;}

	if(0 == ncmp(buf+j, "device", 6))devicelist(0, 0);
	else if(0 == ncmp(buf+j, "driver", 6))driverlist(0, 0);
	else if(0 == ncmp(buf+j, "system", 6))systemlist(0, 0);
	else if(0 == ncmp(buf+j, "artery", 6))arterylist(0, 0);
	else if(0 == ncmp(buf+j, "arena", 5))arenalist(0, 0);
	else if(0 == ncmp(buf+j, "actor", 5))actorlist(0, 0);
	else say("ls(%s)\n", buf+j);
}
void term_cd0(u8* buf)
{
	int j,k;
	u8* pp;

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
void term_cdn(u8* buf)
{
	if(0 == buf)pos = 0;
	else if(buf[2] < 0x20)pos = 0;
}
void term_cmd0(u8* buf)
{
	int j,len;
	int a=-1,b=-1;
	void* win;
	void* act;
	u8* css;
	u8* pin;
	for(j=0;j<0x1000;j++){if(*buf <= 0x20)buf++;}

	//split l and r
	for(j=0;j<0x1000;j++)
	{
		if(buf[j] < 0x20){len = j;break;}
		else if('=' == buf[j]){a = j;}
	}
	if(a < 0)return;

	//eat non-char
	for(;len>0;len--){if(buf[len-1] > 0x20)break;}
	for(b=a+1;b<len;b++){if(buf[b] > 0x20)break;}
	for(a=a-1;a>=0;a--){if(buf[a] > 0x20)break;}
	//say("lval=%.*s\nrval=%.*s\n", a+1, buf, len-b, buf+b);

	//<aaaa> = <bbbb>
	if( ('<' != buf[0]) | ('>' != buf[a]) )return;
	if( ('<' != buf[b]) | ('>' != buf[len-1]) )return;

	//<arena/win0 style="width:50%;height:50%;">
	//<actor/xiangqi pinid="black;expert;">
	//say("<%.*s> = <%.*s>\n", a-1, buf+1, len-b-2, buf+b+1);

	//find
	win = arenalist(buf+1, 8);
	if(0 == win)return;
	act = actorlist(buf+b+1, 8);
	if(0 == act)return;

	//parse
	css = allocstyle();
	pin = allocpinid();
	parsestyle(css, buf+1, a-1);
	//parsepinid(pin, buf+b+1, len-b-2);

	//rel
	say("%llx,%llx,%llx,%llx\n", win, css, act, pin);
	relation_write(win, css, _win_, act, pin, _act_);
}
void term_cmdn(u8* buf)
{
	if(0 == buf)return;
	if(buf[0] < 0x20)return;

	else if(_device_ == path[0])devicechoose(buf, 0);
	else if(_driver_ == path[0])driverchoose(buf, 0);
	else if(_system_ == path[0])systemchoose(buf, 0);
	else if(_artery_ == path[0])arterychoose(buf, 0);
	else if(_arena_ == path[0])arenachoose(buf, 0);
	else if(_actor_ == path[0])actorchoose(buf, 0);
}
void term_read(u8* buf)
{
	int j;
	if(buf == 0)goto prompt;
	if( (buf[0] == 'q') && (buf[1] < 0x20) )goto finish;
	if(ncmp(buf, "exit", 4) == 0)goto finish;

	//
	if(0 == ncmp(buf, "ls", 2))
	{
		if(0 == pos)term_ls0(buf);
		else term_lsn(buf);
	}
	else if(0 == ncmp(buf, "cd", 2))
	{
		if(0 == pos)term_cd0(buf);
		else term_cdn(buf);
	}
	else
	{
		if(0 == pos)term_cmd0(buf);
		else term_cmdn(buf);
	}

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




void initstdin(void* addr)
{
	input = addr;
	enq = 0;
}
void freestdin()
{
}