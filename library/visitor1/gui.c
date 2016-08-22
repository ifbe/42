#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//libui1
void characterstart(char* addr,char* pixfmt, int width,int height);
void characterstop();
void characterwrite(QWORD type,QWORD key);
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
void eventread(QWORD* first,QWORD* second);
//world
void birth();
void death();
//
void printmemory(char*,int);
void say(char*,...);




static char pixbuf[2048*1024*4];
static char pixfmt[8] = {'b','g','r','a','8','8','8','8'};
static int width=512;
static int height=512;




void main()
{
	QWORD type=0;
	QWORD key=0;

	//before
	birth();
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
		eventread(&type,&key);
		if( type==0 )break;

		//3.处理事件，如果要求自杀就让它死
		characterwrite(type,key);
	}

	//after
	characterstop();
	windowstop();
	death();
}
