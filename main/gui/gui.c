#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void registerhex(char*);		//1.hex.c
void register2048(char*);		//2.2048.c
void registerkeyboard(char*);		//2.keyboard.c
void registertree(char*);		//2.tree.c
void registersketchpad(char*);		//3.sketchpad.c
void registerconsole(char*);		//4.console.c
//
void rectangle(QWORD leftup,QWORD rightdown,DWORD color);
void colorstring(int x,int y,char* str,unsigned int color);
void waitevent(QWORD* first,QWORD* second);
//
void command(char* p);
void listmemory();
void masterinto(char* in);
void initall();
void cleanall();
//
QWORD readwindow(QWORD);
void writewindow();
char* whereischaracter();
//
void diary(char*,...);
void printmemory(char*,int);




//苦工
struct worker
{
	//[0,7]:种类
	QWORD type;			//'window'

	//[8,f]:概括
	QWORD id;			//'小明'

	//[10,17]:开始
	QWORD startaddr;		//左上角:x+(y<<16)+(z<<32)+(w<<48)

	//[18,1f]:结束
	QWORD endaddr;			//右下角:x+(y<<16)+(z<<32)+(w<<48)

	//[20,27]:起因
	int (*list)();
	char padding1[ 8 - sizeof(char*) ];

	//[28,2f]:经过
	int (*into)();
	char padding2[ 8 - sizeof(char*) ];

	//[30,37]:输出
	int (*read)();
	char padding3[ 8 - sizeof(char*) ];

	//[38,3f]:输入
	int (*write)(QWORD type,QWORD key);
	char padding4[ 8 - sizeof(char*) ];
};
static struct worker* worker;








//界面控制
static unsigned int top=0;
static unsigned int now=1; 
void guicommand(char* p)
{
	//先在在这里找所要求的那个character，比如'hex','console'
	int i=0;
	char buf[8];
	QWORD temp=0;

	//
	if(p==0)
	{
		now=0;
		return;
	}
	else if((QWORD)p<0x40)
	{
		top=0;
		now=(QWORD)p;
		worker[now].into();
		return;
	}
	else
	{
		//到最后一个ascii码
		*(QWORD*)buf=0;
		for(i=0;i<8;i++)
		{
			if(p[i]==0)break;
			buf[i]=p[i];
		}
		for(i=0;i<8;i++)
		{
			if( (buf[i]<'a') && (buf[i]>'z') )break;
		}
		for(;i<8;i++){buf[i]=0;}

		//拿最终结果，比较是不是退出
		temp=*(QWORD*)buf;
		if(temp == 0x74697865)	//exit
		{
			now=0;
			return;
		}

		if(temp == 0x6d6f646e6172)	//random
		{
			for(i=1;i<0x1000/0x40;i++)
			{
				if(worker[i].id == 0)break;
			}
			//diary("count=%x\n",i);

			top=0;
			now=getrandom();
			now=( now % (i-2) ) + 1;
			diary("random=%x\n",now);

			worker[now].into();
			return;
		}

		//找
		for(i=1;i<0x100;i++)
		{
			if(worker[i].id==0)break;
			if(worker[i].id==temp)
			{
				top=0;
				now=i;
				worker[now].into();
				return;
			}
		}

		//全部找不到的话调用libsoft1.command()
		command(p);
	}
}








//菜单
static char buffer[128];
static int bufp=0;
void readmenu()
{
	//body
	rectangle((256<<16)+256 , (512<<16)+768  , 0);
	squareframe((256<<16)+256 , (512<<16)+768  , 0xcccccc);

	//close button
	rectangle((256<<16)+768-16 , ((256+16)<<16)+768  , 0xff0000);

	//string
	colorstring(0x20 , 16 , "what do you want?" , 0xcccccc);
	colorstring(0x20 , 17 , buffer , 0xcccccc);
}
void writemenu(QWORD type,QWORD key)
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
		if( (y<256+16) && (x>768-16) )
		{
			//退出
			guicommand(0);
			return;
		}
	}//left

	//'char'
	else if(type==0x72616863)
	{
		if(key==0x8)		//backspace
		{
			if(bufp!=0)
			{
				bufp--;
				buffer[bufp]=0;
			}
		}
		else if(key==0xd)		//回车
		{
			//say("%s\n",buffer);
			guicommand(buffer);

			//clear
			for(bufp=0;bufp<127;bufp++) buffer[bufp]=0;
			bufp=0;
		}
		else
		{
			if(bufp<0x80)
			{
				buffer[bufp]=key&0xff;
				bufp++;
			}
		}
	}//kbd

}








//显示，事件处理
void printworld()
{
	//开始画画
	worker[now].read();
	if(top > 0)readmenu();
}
void processmessage(QWORD type,QWORD key)
{
	//diary("%s,%llx\n",&type,key);

	//'dropfile'
	if(type==0x656c6966706f7264)
	{
		//diary("debuging::::::::%s\n",(char*)key);
		masterinto((char*)key);
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
	if(top > 0)writemenu(type,key);
	else worker[now].write(type,key);
}








//初始化
void registercharacter()
{
	unsigned int i;
	char* baseaddr=whereischaracter();
	char* temp=baseaddr;
	worker=(struct worker*)temp;

	//clean everything
	for(i=0;i<0x100000;i++)temp[i]=0;
	temp+=0x40;

	//[+0x40,+0x7f]:	1.hex.c
	registerhex(temp);
	temp += 0x40;

	//[+0x80,+0xbf]:	2.2048.c
	register2048(temp);
	temp += 0x40;

	//[+0xc0,+0xff]:	2.keyboard.c
	registerkeyboard(temp);
	temp += 0x40;

	//[+0x100,+0x13f]:	2.tree.c
	registertree(temp);
	temp += 0x40;

	//[+0x140,+0x17f]:	3.sketchpad.c
	registersketchpad(temp);
	temp += 0x40;

	//[+0x180,+0x1bf]:	4.console.c
	registerconsole(temp);
	temp += 0x40;
}
void main()
{
	//before
	initall();

	//begin
	registercharacter();
	guicommand("random");

	//forever
	QWORD type=0;
	QWORD key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		printworld();
		writewindow();

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
