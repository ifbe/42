#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//event
void* eventread();
void eventwrite(u64 why, u64 what, u64 where, u64 when);
void* birth();
void death();
//soft
int arteryprompt();
int arterycommand(void*);
void motion_explain(void*);
void network_explain(void*);
void sound_explain(void*);
void vision_explain(void*);
//boot
void printmemory(char*, int);
void say(char*, ...);




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
	void* buf;
	u64 fmt;
	u64 w;
	u64 h;
};




int main(int argc,char* argv[])
{
	//before
	int ret;
	struct event* ev;
	birth();

	//config
	for(ret=1;ret<argc;ret++)
	{
		arterycommand( argv[ret] );
		arterycommand( "\n" );
	}

	//forever
	while(1)
	{
		//1.show
		arteryprompt();

again:
		//2.wait
		ev = eventread();
		if(ev == 0)break;		//error
		if(ev->what == 0)break;		//exit


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
			//vision rawdata -> my event
			vision_explain(ev);
		}
		if((ev->what) != 0x64626b)goto again;


		//4.real process
		arterycommand(ev);
	}

	//after
	death();
	return 0;
}
