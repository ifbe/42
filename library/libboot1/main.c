#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define __time__ hex32('t','i','m','e')
#define __fd__ hex32('f','d',0,0)
//libui1
int actorread();
int actorwrite(void*);
int term_read(void*);
int term_write(void*);
//libsoft1
int artery_explain(void*);
int sound_explain(void*);
int vision_explain(void*);
//libsoft0
void sleep_us(int);
//libboot
void printmemory(void*, int);
void say(void*, ...);
//
void* eventread();
void eventwrite(u64,u64,u64,u64);
void* birth();
void death();




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};




int main(int argc, char* argv[])
{
	int fps;
	int ret;
	u64 temp;
	u64 time;
	struct event* ev;

	birth();
	//say("@birth\n");

	term_read(0);
	for(ret=1;ret<argc;ret++)
	{
		term_write(argv[ret]);
		term_write("\n");
	}

	while(1)
	{
		//1.show
		actorread();
		fps++;

again:
		//2.wait
		ev = eventread();
		if(ev == 0)
		{
			sleep_us(1000);
			goto again;
		}

		//exit or timer
		if(ev->what == 0)break;		//exit
		if(ev->what == __time__)	//time
		{
			if(ev->when > time+1000000)
			{
				//say("fps=%d\n",fps);
				time = ev->when;
				fps = 0;
			}
			goto again;
		}
		if(ev->what == __fd__)
		{
			//network rawdata -> my event
			ret = artery_explain(ev);
			if(ret != 42)goto again;
		}


		//3.pre process
		temp = (ev->what)&0xff;
		if(temp == 's')
		{
			//sound rawdata -> my event
			ret = sound_explain(ev);
			if(ret != 42)goto again;
		}
		else if(temp == 'v')
		{
			//video rawdata -> my event
			ret = vision_explain(ev);
			if(ret != 42)goto again;
		}


		//4.real process
		actorwrite(ev);
	}

	//after
	//say("@death\n");
	death();
	return 0;
}
