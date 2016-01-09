#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void menuinit(char*);		//menu.c
void hexinit(char*);		//1.hex.c
void keyboardinit(char*);	//2.keyboard.c
void treeinit(char*);		//2.tree.c
void sketchpadinit(char*);	//3.sketchpad.c
void consoleinit(char*);	//4.console.c
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
char* whereischaracter();
void diary(char*,...);
void printmemory(char*,int);




//苦工
struct worker
{
	//概括，人物
	QWORD type;			//'window'
	QWORD id;			//'小明'
	QWORD startaddr;		//左上角:x+(y<<16)+(z<<32)+(w<<48)
	QWORD endaddr;			//右下角:x+(y<<16)+(z<<32)+(w<<48)

	//起因，经过，结果
	int (*show)();				//[20,27]
	int (*message)(QWORD type,QWORD key);	//[28,2f]
	int (*otherfunc)();
	int (*otherother)();

	//对齐0x80字节
	char padding[ 0x40 - (4*sizeof(QWORD)) - (4*sizeof(char*)) ];
};
static struct worker* worker;		//whereisworker
	//1.hex.c
	//1.vi.c
	//2.fs.c
	//2.keyboard.c
	//2.tree.c
	//3.sketchpad.c
	//4.console.c
	//......




//界面控制
static int now=1; 
static int top=0;
void guireport(int in)
{
	if(in==0)now=0;
}
void guicommand(char* p)
{
	//先在在这里找所要求的那个character，比如'hex','console'
	char buf[8];
	QWORD temp=0;
	int i=0;

	//到最后一个ascii码
	*(QWORD*)buf=*(QWORD*)p;
	for(i=0;i<8;i++)
	{
		if( (buf[i]<'a') && (buf[i]>'z') )break;
	}

	//清空后面的
	for(;i<8;i++){buf[i]=0;}
	temp=*(QWORD*)buf;

	//找
	for(i=1;i<0x100;i++)
	{
		if(worker[i].id==0)break;
		if(worker[i].id==temp)
		{
			now=i;
			top=0;
			return;
		}
	}

	//全部找不到的话调用libsoft1.command()
	command(p);
}




//菜单
static char buffer[128];
static int bufcount=0;
void menuprint()
{
	rectangle((256<<16)+256 , (512<<16)+768  , 0);
	rectangle((256<<16)+768-32 , ((256+32)<<16)+768  , 0xff0000);
	string(0x20 , 16 , "what do you want?" );
	string(0x20 , 17 , buffer );
}
void menumessage(QWORD type,QWORD key)
{
	//diary("%s,%llx\n",&type,key);

	//'xyz left'
	if(type==0x7466656C207A7978)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;

		//点击框框外面，关掉菜单
		if( (x<256)|(x>768)|(y<256)|(y>512) )
		{
			top=0;
			return;
		}

		//点击红色矩形，退出
		if( (y<256+32) && (x>768-32) )
		{
			//退出
			guireport(0);
			return;
		}
	}//left

	//'char'
	else if(type==0x72616863)
	{
		if(key==0x8)		//backspace
		{
			if(bufcount!=0)
			{
				bufcount--;
				buffer[bufcount]=0;
			}
		}
		else if(key==0xd)		//回车
		{
			if(compare( buffer , "exit" ) == 0)
			{
				guireport(0);
				return;
			}
			else
			{
				//say("%s\n",buffer);
				guicommand(buffer);

				for(bufcount=0;bufcount<127;bufcount++) buffer[bufcount]=0;
				bufcount=0;
			}
		}
		else
		{
			if(bufcount<0x80)
			{
				buffer[bufcount]=key&0xff;
				bufcount++;
			}
		}
	}//kbd

}




//显示，事件处理
void printworld()
{
	//开始画画
	worker[now].show();
	if(top > 0)menuprint();
}
void processmessage(QWORD type,QWORD key)
{
	//diary("%s,%llx\n",&type,key);

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
			top ^= 1;
			return;
		}
	}

	//'touch'
	else if(type==0x6863756f74)
	{
		diary("touch!\n");
	}//touch

	//其余所有消息，谁在干活就交给谁
	if(top > 0)menumessage(type,key);
	else worker[now].message(type,key);
}








//初始化
void wakeupcharacter()
{
	int i;
	char* temp=whereischaracter();
	worker=(struct worker*)temp;

	//clean everything
	for(i=0;i<0x100000;i++)temp[i]=0;
	temp+=0x40;

	//[+0x40,+0x7f]:	1.hex.c
	hexinit(temp);
	temp += 0x40;

	//[+0x80,+0xbf]:	2.keyboard.c
	keyboardinit(temp);
	temp += 0x40;

	//[+0xc0,+0xff]:	2.tree.c
	treeinit(temp);
	temp += 0x40;

	//[+0x100,+0x13f]:	3.sketchpad.c
	sketchpadinit(temp);
	temp += 0x40;

	//[+0x140,+0x17f]:	4.console.c
	consoleinit(temp);
	temp += 0x40;
}
void main()
{
	//before
	initall();

	//begin
	wakeupcharacter();

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
		if( now<=0 )break;
	}

	//after
	cleanall();
}
