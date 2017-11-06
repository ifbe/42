#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//libui1
int actorread();
int actorwrite(void*);
int term_read(void*);
int term_write(void*);
//libui0
int arenaread();
int arenawrite();
//libsoft1
int network_explain(void*);
int sound_explain(void*);
int vision_explain(void*);
//libsoft0
u64 gettime();
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
static u64 time;
static int fps;




int main(int argc, char* argv[])
{
	//before
	int ret;
	u64 temp;
	struct event* ev;

	birth();
	//say("@birth\n");

	//config
	term_read(0);
	for(ret=1;ret<argc;ret++)
	{
		term_write(argv[ret]);
		term_write("\n");
	}

	//forever
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
		if(ev->what == 0x656d6974)	//time
		{
			if(ev->when > time+1000000)
			{
				//say("fps=%d\n",fps);
				time = ev->when;
				fps = 0;
			}
			goto again;
		}


		//3.pre process
		temp = (ev->what)&0xff;
		if(temp == 'n')
		{
			//network rawdata -> my event
			ret = network_explain(ev);
			if(ret != 42)goto again;
		}
		else if(temp == 's')
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
