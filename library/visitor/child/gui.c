#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//libui1
void characterlist();
void characterchoose(char*);
void characterread();
void characterwrite(QWORD type,QWORD key);
void characterstart(DWORD size,char* addr);
void characterstop();
//libui0
void windowstart(DWORD,char*);
void windowstop();
void windowread();
void windowwrite();
//event
void writeevent();
void readevent(QWORD* first,QWORD* second);
//world
void birth();
void death();
//
void printmemory(char*,int);
void say(char*,...);




static char mybuffer[2048*1024*4];		//4m
void main()
{
	QWORD type=0;
	QWORD key=0;

	//before
	birth();
	windowstart(512+(512<<16), mybuffer);
	characterstart(512+(512<<16), mybuffer);
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
		readevent(&type,&key);
		if( type==0 )break;

		//3.处理事件，如果要求自杀就让它死
		characterwrite(type,key);
	}

	//after
	characterstop();
	windowstop();
	death();
}
