#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void characterlist();
void characterchoose(char*);
void characterread();
void characterwrite(QWORD type,QWORD key);
void characterstart(DWORD size,char* addr);
void characterstop();
//
void readevent(QWORD* first,QWORD* second);
void readwindow();
void writewindow();
//void writewindow(DWORD,char*);
//
void birth();
void death();
void say(char*,...);
void printmemory(char*,int);




static char mybuffer[2048*1024*4];		//4m
void main()
{
	QWORD type=0;
	QWORD key=0;

	//before
	birth();
	characterstart(512+(512<<16) , mybuffer);

	//forever
	while(1)
	{
		//debug
		//say("i am here\n");

		//1.先在内存里画画，然后一次性写到窗口内
		characterread();
		//writewindow(512+(512<<16) , mybuffer);
		writewindow();

		//2.等事件，是退出消息就退出
		readevent(&type,&key);
		if( type==0 )break;

		//3.处理事件，如果要求自杀就让它死
		characterwrite(type,key);
	}

	//after
	characterstop();
	death();
}
