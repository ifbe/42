#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//visitor0
void eventwrite();
void eventread(u64* who, u64* what, u64* how);
void birth();
void death();
//libui1
int charactercreate();
int characterdelete();
int characterstart(char* addr,char* pixfmt, int width,int height);
int characterstop();
int characterwrite(u64 who, u64 what, u64 how);
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
//
void printmemory(char*,int);
void say(char*,...);




//
__attribute__((aligned(0x1000))) static char pixbuf[2048*1024*4 + 0x100000];
static char pixfmt[8] = {'b','g','r','a','8','8','8','8'};
static int width=512;
static int height=512;




int main(int argc, char* argv[])
{
	int ret;
	u64 who, what, how;

	//before
	birth();

	//config
	windowstart(pixbuf, pixfmt, width, height);	//it has the right to decide
	characterstart(pixbuf, pixfmt, width, height);	//the changed final argument
	for(ret=1;ret<argc;ret++)
	{
		say("%s\n",argv[ret]);
		charactercommand(argv[ret]);
	}

	//forever
	while(1)
	{
		//1.先在内存里画画, 然后一次性写到窗口内
		characterread();
		windowwrite();

		//2.等事件, 是退出消息就退出, 其他event都交给用户处理
		eventread(&who, &what, &how);
		if( what==0 )break;
		characterwrite(who, what, how);
	}

	//after
	death();
	return 0;
}
