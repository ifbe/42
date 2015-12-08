#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void menuinit();	//0
void f1init();
void f2init();
void f3init();
void f4init();
//
void point(int x,int y,DWORD color);
void string(int x,int y,char* str);
void writescreen();
void waitevent(QWORD* first,QWORD* second);
//
void listmemory();
void intomemory(char* in);
void memoryandface();
void cleanall();




//servents' report
static int control=0; 
static int what=1;
//things
struct things
{
	//type
	QWORD type;				//[0,7]
	QWORD subtype;			//[8,f]
	QWORD start;			//[10,17]
	QWORD end;				//[18,1f]
	char functions[0x20];	//[0x20,0x3f]
}
static struct things[16];	//总共16个"元素"








int (*show)();		//[20,27]
int (*message)(QWORD type,QWORD key);	//[28,2f]
void changegui(int in)
{
	//退出
	if(in<=0)
	{
		control=in;
	}

	//哪一个:>0
	else
	{
		what=in;
	}
}








//lord's requires
void printworld()
{
	int x,y;

	//主界面显示什么
	switch(what&0xff)
	{
		case 1:f1show();break;
		case 2:f2show();break;
		case 3:f3show();break;
		case 4:f4show();break;
	}
	if(control==1)menushow();
}
void processmessage(QWORD type,QWORD key)
{
	if(type==0x656c6966706f7264)		//dropfile
	{
		//diary("debuging::::::::%s\n",(char*)key);
		intomemory((char*)key);
		return;
	}
	else if(type==0x64626b)		//kbd
	{
		if(key==0x1b)
		{
			control^=1;
			menuinit();		//0
			return;
		}		//按下esc
		if(key==0x70)
		{
			what=1;
			f1init();			//1
			return;
		}		//f1
		if(key==0x71)
		{
			what=2;
			f2init();			//2
			return;
		}		//f2
		if(key==0x72)
		{
			what=3;
			f3init();			//3
			return;
		}		//f3
		if(key==0x73)
		{
			what=4;
			f4init();			//4
			return;
		}		//f4
	}
	else if(type==0x7466656c)		//鼠标
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		if(x>1024-16)
		{
			if(y<16)				//右上
			{
				control^=1;
				return;
			}
		}
	}

	//其余所有消息，谁在干活就交给谁
	if(control==1)menumessage(type,key);		//磁盘
	else if(what==1)f1message(type,key);		//hex在干活就交给hex
	else if(what==2)f2message(type,key);		//logic0在干活就交给logic0
	else if(what==3)f3message(type,key);		//点了叉
	else if(what==4)f4message(type,key);		//console在干活就交给console
}








void main()
{
	//before
	memoryandface();
	f1init();			//1
	f2init();			//2
	f3init();			//3
	f4init();			//4

	//forever
	QWORD type=0;
	QWORD key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		printworld();
		writescreen();

		//2.等事件，是退出消息就退出
		waitevent(&type,&key);
		if( type==0 )break;

		//3.处理事件，如果要求自杀就让它死
		processmessage(type,key);
		if( control<0 )break;
	}

	//after
	cleanall();
}