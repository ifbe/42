#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//visitor0
void* eventread();
void eventwrite(u64 why, u64 what, u64 where, u64 when);
void* birth();
void death();
//libui1
int characterstart(u64 buf, u64 fmt, u64 w, u64 h);
int characterstop();
int characterwrite(void* event);
int characterread(void* screen);
int characterlist(char*);
int charactercommand(char* p);
//libui0
int displaystart(u64 buf, u64 fmt, u64 w, u64 h);
int displaystop();
int displaylist();
int displaychoose();
int displayread(void*);
int displaywrite(void*);
//libsoft1
void motion_explain(void*);
void network_explain(void*);
void sound_explain(void*);
void vision_explain(void*);
//libhard
//libboot
void printmemory(char*,int);
void say(char*,...);




//other
static u64 time;
static int fps;

//visitor
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};

//libui
struct screen
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;
	char data[256-4*sizeof(u64)];
};




int main(int argc, char* argv[])
{
	//before
	int ret;
	struct event* ev;
	struct screen* ui = birth() + 0x400000;
	say("ui@%llx\n", (u64)ui);

	//config
	ui[0].buf = 0;
	ui[0].fmt = 0x6267726138383838;
	ui[0].w = 512;
	ui[0].h = 512;
	ret = displaystart(ui[0].buf, ui[0].fmt, ui[0].w, ui[0].h);
	ret = characterstart(ui[0].buf, ui[0].fmt, ui[0].w, ui[0].h);
	for(ret=1;ret<argc;ret++)
	{
		charactercommand(argv[ret]);
	}

	//forever
	while(1)
	{
		//1.show
		characterread(ui);
		displaywrite(ui);
		fps++;

again:
		//2.wait
		ev = eventread();
		if(ev == 0)break;		//error
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
		if(((ev->what)&0xff) == 'p')
		{
			//sensor rawdata -> my event
			motion_explain(ev);
		}
		else if(((ev->what)&0xff) == 'n')
		{
			//network rawdata -> my event
			network_explain(ev);
		}
		else if(((ev->what)&0xff) == 's')
		{
			//sound rawdata -> my event
			sound_explain(ev);
		}
		else if(((ev->what)&0xff) == 'v')
		{
			//video rawdata -> my event
			vision_explain(ev);
		}
		if(ev->what == 0)goto again;


		//4.real process
		characterwrite(ev);
	}

	//after
	death();
	return 0;
}
