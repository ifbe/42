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
int parsexml_relation(void*, int);
int parsexml(void*, int);
int parsejson(void*, int);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int openreadclose(void*, u64, void*, u64);
int openwriteclose(void*, u64, void*, u64);
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




static u64 path[8];
static int pos = 0;
//
static u8* input = 0;
static int dx = 0;
static int dy = 0;
static int enq = 0;
static int deq = 0;
void initstdin(void* addr)
{
	input = addr;
	dx = 0;
	dy = 0;
	enq = 0;
	deq = 0;
}
void freestdin()
{
}
void* getstdin()
{
	return input;
}
int getcurin()
{
	return enq;
}





void term_ls0(u8* buf, int len)
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
void term_lsn(u8* buf, int len)
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
void term_cd0(u8* buf, int len)
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
void term_cdn(u8* buf, int len)
{
	if(0 == buf)pos = 0;
	else if(buf[2] < 0x20)pos = 0;
}
void term_cmd0(u8* buf, int len)
{
	int ret;
	u8 data[0x10000];
	if(0 == buf)return;
	if(buf[0] < 0x20)return;

	if(0 == ncmp(buf, "--", 2))
	{
		buf += 2;
		if(0 == ncmp(buf, "json=", 5))
		{
			ret = openreadclose(buf+5, 0, data, 0x10000);
			parsejson(data, ret);
		}
		else if(0 == ncmp(buf, "xml=", 4))
		{
			ret = openreadclose(buf+4, 0, data, 0x10000);
			parsexml(data, ret);
		}
	}
	else
	{
		say("unknown:%.*s\n", len, buf);
	}
}
void term_cmdn(u8* buf, int len)
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




void term_prompt()
{
	int j;
	if(0 != pos)
	{
		say("[term@");
		for(j=0;j<pos;j++)say("%s/", &path[j]);
		say("]");
	}
	else say("[term@void]");
}
void term_parse(u8* buf, int len)
{
	int j;
	if(0 == buf)return;

	if(('q' == buf[0])&&(0x20 > buf[1]))goto byebye;
	if(0 == ncmp(buf, "exit", 4))goto byebye;

	//
	if(0 == ncmp(buf, "ls", 2))
	{
		if(0 == pos)term_ls0(buf, len);
		else term_lsn(buf, len);
	}
	else if(0 == ncmp(buf, "cd", 2))
	{
		if(0 == pos)term_cd0(buf, len);
		else term_cdn(buf, len);
	}
	else
	{
		if(0 == pos)term_cmd0(buf, len);
		else term_cmdn(buf, len);
	}

finish:
	term_prompt();
	return;

byebye:
	eventwrite(0,0,0,0);
	return;
}
void termread(int flag)
{
	int j;
	if(deq > enq)deq = 0;
	if(enq == deq)return;

	for(j=deq;j<enq;j++)
	{
		if('\n' == input[j])
		{
			if(flag)say("%.*s\n", j-deq, input+deq);
			term_parse(input+deq, j-deq);
			deq = j+1;
		}
	}
}
void termwrite(u8* buf, int len)
{
	int j,k,t;
	if(buf == input)
	{
		enq = len;
		term_prompt();
		termread(1);
		return;
	}

	for(j=0;j<0x1000;j++)
	{
		k = buf[j];
		if(k < 8)return;

		if((0x8 == k)|(0x7f == k))
		{
			while(1)
			{
				if(enq <= 0)break;
				if(enq <= deq)break;
				say("\b \b");

				enq--;
				k = input[enq];
				input[enq] = 0;
				if((k < 0x80)|(k > 0xc0))break;
			}
		}
		else if((0x1b == k)&&(0x5b == buf[j+1]))
		{
			k = buf[j+2];
			if((0x41 == k)|(0x42 == k))
			{
				if(0x41 == k)dy++;
				if(0x42 == k)
				{
					if(0 == dy)return;
					dy--;
				}

				//clear line
				while(1)
				{
					if(enq <= 0)break;
					if('\n' == input[enq-1])break;
					enq--;
					input[enq] = 0;
					say("\b \b");
				}
				if(0 == dy)return;

				//print history-dy
				t = 0;
				for(j=enq-1;j>=0;j--)
				{
					if((j>0)&&('\n' != input[j]))continue;

					if(t == dy)
					{
						if(j)j++;
						say("%.*s", k-j, input+j);
						for(t=0;t<k-j;t++)input[enq+t] = input[j+t];

						deq = enq;
						enq += k-j;
						return;
					}

					t++;
					k = j;
				}
			}
			return;
		}
		else
		{
			if(0xd == k)k = 0xa;
			say("%c", k);

			if(enq > 0xf0000)enq = 0;
			input[enq] = k;
			enq++;

			if(0xa == k)termread(0);
			dy = 0;
		}
	}
}
/*
void termwrite(u8* p)
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
			say("\n");
			for(j=0;j<4;j++)say("tab%d\n",j);

			term_prompt();
			say("%s", input);
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
			input[enq] = '\n';
			enq++;

			say("\n");
			termread();
		}
		else
		{
			input[enq] = *p;
			enq++;
		}

		//////////////////
		p++;
	}
}*/