#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void waitevent(QWORD* first,QWORD* second);
void command(char* p);
void masterinto(char* in);
void initall();
void cleanall();
//
char* whereischaracter();
DWORD getrandom();
QWORD readwindow(QWORD);
void writewindow();
//
void say(char*,...);
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
		worker[0].startaddr=0;
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
			//say("count=%x\n",i);

			now=getrandom();
			now=( now % (i-2) ) + 1;
			say("random=%x\n",now);

			worker[0].startaddr=0;
			worker[now].into();
			return;
		}

		//找
		for(i=1;i<0x100;i++)
		{
			if(worker[i].id==0)break;
			if(worker[i].id==temp)
			{
				now=i;
				worker[0].startaddr=0;
				worker[now].into();
				return;
			}
		}

		//全部找不到的话调用libsoft1.command()
		command(p);
	}
}








//显示，事件处理
void printworld()
{
	//where
	say("%llx\n",worker[now].read);

	//主画
	worker[now].read();
	say("background\n");

	//菜单
	if(worker[0].startaddr > 0)worker[0].read();
	say("menu\n");

	//上屏
	writewindow();
	say("window\n");
}
void processmessage(QWORD type,QWORD key)
{
	//say("%s,%llx\n",&type,key);

	//'dropfile'
	if(type==0x656c6966706f7264)
	{
		//say("debuging::::::::%s\n",(char*)key);
		masterinto((char*)key);
		return;
	}

	//'kbd'
	else if(type==0x64626b)
	{
		//按下esc
		if(key==0x1b)
		{
			worker[0].startaddr ^= 1;
			return;
		}
	}

	//'touch'
	else if(type==0x6863756f74)
	{
		say("touch!\n");
	}//touch

	//其余所有消息，谁在干活就交给谁
	if(worker[0].id > 0)worker[0].write(type,key);
	else worker[now].write(type,key);
}








void main()
{
	//before
	initall();
	worker=(struct worker*)whereischaracter();

	//
	now=1;
	worker[1].into();

	//forever
	QWORD type=0;
	QWORD key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		printworld();

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
