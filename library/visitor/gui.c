#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void command(char* p);
void printworld();
void waitevent(QWORD* first,QWORD* second);
void processmessage(QWORD type,QWORD key);
//
void birth();
void death();
void say(char*,...);
void printmemory(char*,int);




void main()
{
	//before
	birth();

	//forever
	QWORD type=0;
	QWORD key=0;
	while(1)
	{
		//debug
		//say("i am here\n");

		//1.先在内存里画画，然后一次性写到窗口内
		printworld();

		//2.等事件，是退出消息就退出
		waitevent(&type,&key);
		if( type==0 )break;

		//3.处理事件，如果要求自杀就让它死
		processmessage(type,key);
	}

	//after
	death();
}
