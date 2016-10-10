#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//libui1
void characterstart(char* addr,char* pixfmt, int width,int height);
void characterstop();
void characterwrite(u64 who, u64 what, u64 how);
void characterread();
void characterchoose(char*);
void characterlist();
//libui0
void windowstart(char* addr,char* pixfmt, int width,int height);
void windowstop();
void windowread();
void windowwrite();
//event
void eventwrite();
void eventread(u64* who, u64* what, u64* how);
//world
void birth();
void death();
//
void printmemory(char*,int);
void say(char*,...);




//
__attribute__((aligned(0x1000))) static char pixbuf[2048*1024*4 + 0x100000];
static char pixfmt[8] = {'b','g','r','a','8','8','8','8'};
static int width=512;
static int height=512;




void main()
{
	u64 who;
	u64 what;
	u64 how;

	//before
	birth();

	//config
	windowstart(pixbuf, pixfmt, width, height);	//it has the right to decide
	characterstart(pixbuf, pixfmt, width, height);	//the changed final argument
	//changewindow("addr",mybuffer);
	//changewindow("size",512+512<<16);
	//changewindow("title","hahahahahaha");

	//forever
	while(1)
	{
		//debug
		//say("i am here\n");

		//1.先在内存里画画，然后一次性写到窗口内
		characterread();
		windowwrite();

		//2.等事件，是退出消息就退出
		eventread(&who, &what, &how);
		if( what==0 )break;

		//3.其他event都交给用户处理
		characterwrite(who, what, how);
	}

	//after
	death();
}
