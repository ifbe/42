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
void initall();
void cleanall();
//
char* whereisface();
void menuinit(char*);		//menu.c
void hexinit(char*);		//1.hex.c
void keyboardinit(char*);	//2.keyboard.c
void treeinit(char*);		//2.tree.c
void sketchpadinit(char*);	//3.sketchpad.c
void consoleinit(char*);	//4.console.c




//things
struct things
{
	//概括，人物
	QWORD type;			//'window'
	QWORD subtype;
	QWORD id;			//'小明'
	QWORD subid;

	//时间，地点
	QWORD starttime;		//
	QWORD endtime;			//
	QWORD startaddr;		//左上角:x+(y<<16)+(z<<32)+(w<<48)
	QWORD endaddr;			//右下角:x+(y<<16)+(z<<32)+(w<<48)

	//起因，经过，结果
	int (*show)();				//[20,27]
	int (*message)(QWORD type,QWORD key);	//[28,2f]

	//对齐0x80字节
	char padding[ 0x80 - (8*sizeof(QWORD)) - (2*sizeof(char*)) ];
};




//0.menu.c
//1.hex.c
//1.vi.c
//2.fs.c
//2.keyboard.c
//2.tree.c
//3.sketchpad.c
//4.console.c
//......
static struct things* things;		//whereisface
static int control=0; 
void serventreport(int in)
{
	control=in;
}








//lord's requires
void printworld()
{
	int i;
	printmemory(things,0x400);

	for(i=0;i<8;i++)
	{
		if(things[i].type==0)break;

		diary(
			"%s,%s,%llx,%llx\n" ,
			&things[i].type,
			&things[i].id,
			things[i].show,
			things[i].message
		);
	}

	if(control > 0)things[0].show();
}
void processmessage(QWORD type,QWORD key)
{
	diary("%s,%llx\n",&type,key);

	//'dropfile'
	if(type==0x656c6966706f7264)
	{
		//diary("debuging::::::::%s\n",(char*)key);
		intomemory((char*)key);
		return;
	}

	//'kbd'
	else if(type==0x64626b)
	{
		//按下esc
		if(key==0x1b)
		{
			control^=1;
			return;
		}
	}

	//'mouse'
	else if(type==0x7466656c)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;

		//右上
		if(x>1024-16)
		{
			if(y<16)
			{
				control^=1;
				return;
			}
		}
	}

	//'touch'
	else if(type==0x6863756f74)
	{
		diary("touch!\n");
	}

	//其余所有消息，谁在干活就交给谁
}








void initgui()
{
	int i;
	char* face=whereisface();
	things=(struct things*)face;

	//clean everything
	for(i=0;i<0x100000;i++)face[i]=0;

	//menu.c
	menuinit(face);
	face += 0x80;

	//1.hex.c
	hexinit(face);
	face += 0x80;

	//2.keyboard.c
	keyboardinit(face);
	face += 0x80;

	//2.tree.c
	treeinit(face);
	face += 0x80;

	//3.sketchpad.c
	sketchpadinit(face);
	face += 0x80;

	//4.console.c
	consoleinit(face);
	face += 0x80;

}
void main()
{
	//before
	initall();

	//begin
	initgui();

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
