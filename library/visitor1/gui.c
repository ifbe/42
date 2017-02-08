#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//libui1
int charactercreate();
int characterdelete();
int characterstart(char* addr,char* pixfmt, int width,int height);
int characterstop();
int characterwrite(u64* p);
int characterread();
int characterchoose(char*);
int characterlist(char*);
int charactercommand(char* p);
//libui0
int windowcreate();
int windowdelete();
int windowstart(char* addr,char* pixfmt, int width,int height);
int windowstop();
int windowlist();
int windowchoose();
int windowread();
int windowwrite();
//libsoft1
void motion_explain(void*);
void network_explain(void*);
void sound_explain(void*);
void vision_explain(void*);
//libhard
//libboot
void printmemory(char*,int);
void say(char*,...);
//visitor0
u64* eventread();
void eventwrite(u64 why, u64 what, u64 where, u64 when);
void birth();
void death();




//
static u64 time;
static int fps;
//
__attribute__((aligned(0x1000))) static char pixbuf[2048*1024*4 + 0x100000];
static char pixfmt[8] = {'b','g','r','a','8','8','8','8'};
static int width=512;
static int height=512;




int main(int argc, char* argv[])
{
	int ret;
	u64* addr;

	//before
	birth();

	//config
	windowstart(pixbuf, pixfmt, width, height);	//it has the right to decide
	characterstart(pixbuf, pixfmt, width, height);	//the changed final argument
	for(ret=1;ret<argc;ret++)
	{
		charactercommand(argv[ret]);
	}

	//forever
	while(1)
	{
/*
		//1.display
		//[+00,+07]addr		pointer to actual memory
		//[+08,+0f]fmt		rgba, text, html, ...
		//[+10,+17]width
		//[+18,+1f]height
		for(ret=0;ret<max;ret++)
		{
			characterread(list + ret*0x20);
			windowwrite(  list + ret*0x20);
		}
		fps++;
*/
		characterread();
		windowwrite();
		fps++;

again:
		//2.event
		//[+00,+07]why
		//[+08,+0f]what
		//[+10,+17]where
		//[+18,+1f]when
		addr = eventread();
		if(addr == 0)break;	//error
		if(addr[1] == 0)break;	//exit
		if(addr[1] == 0x656d6974)	//time
		{
			if(addr[3] - time > 1000000)
			{
				say("fps=%d\n",fps);
				time = addr[3];
				fps = 0;
			}
			goto again;
		}


		//3.pre change
		else if((addr[1]&0xff) == 'p')	//motion
		{
			motion_explain(addr);
		}
		else if((addr[1]&0xff) == 'n')	//net
		{
			network_explain(addr);
		}
		else if((addr[1]&0xff) == 's')	//sound
		{
			sound_explain(addr);
		}
		else if((addr[1]&0xff) == 'v')	//vision
		{
			vision_explain(addr);
		}
		if(addr[1] == 0)goto again;


		//4.real change
		characterwrite(addr);
	}

	//after
	death();
	return 0;
}
