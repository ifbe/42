#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//init......
void ascii_init(char*,char*);
void unicode_init(char*,char*);
void background_init(char*,char*);
void shape_init(char*,char*);
void menu_init(char*,char*);			//menu.c
void hex_init(char*,char*);			//1.hex.c
void the2048_init(char*,char*);			//2.2048.c
void keyboard_init(char*,char*);		//2.keyboard.c
void tree_init(char*,char*);			//2.tree.c
void sketchpad_init(char*,char*);	//3.sketchpad.c
void console_init(char*,char*);		//4.console.c
//kill......
void menu_kill();
void hex_kill();
void the2048_kill();
void keyboard_kill();
void tree_kill();
void sketchpad_kill();
void console_kill();

//
int arterystart(char*);
int arterystop();
int compare(char*,char*);
DWORD getrandom();
//
void say(char* , ...);




//
static unsigned int now=0;		//不能有负数




//[0x000000,0x0fffff]:
//[0x100000,0x1fffff]:
//[0x200000,0x2fffff]:
//[0x300000,0x3fffff]:
static struct working
{
        //[0,7]:种类            //'window'
        QWORD type;

        //[8,f]:名字            //'小明'
        QWORD id;

        //[10,17]:开始
        int (*start)(DWORD size,char* addr);
        char padding2[ 8 - sizeof(char*) ];

        //[18,1f]:结束
        int (*stop)();
        char padding3[ 8 - sizeof(char*) ];

        //[20,27]:起因
        int (*list)();
        char padding4[ 8 - sizeof(char*) ];

        //[28,2f]:经过
        int (*choose)();
        char padding5[ 8 - sizeof(char*) ];

        //[30,37]:输出
        int (*read)();
        char padding6[ 8 - sizeof(char*) ];

        //[38,3f]:输入
        int (*write)(QWORD type,QWORD key);
        char padding7[ 8 - sizeof(char*) ];
}*worker;
static unsigned char* mega1;
static unsigned char* mega2;
static unsigned char* mega3;








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void characterlist()
{
	//列出所有“人物”
	int x;
	for(x=0;x<16;x++)
	{
		if(worker[x].id != 0)
		{
			say("%d:%s\n",x,&worker[x].id);
		}
	}
}
int characterchoose(char* p)
{
	int i;
	int ret;

	//random
	ret=compare(p,"random");
	if(ret==0)
	{
		for(i=1;i<0x1000/0x40;i++)
		{
			if(worker[i].id == 0)break;
		}

		now=getrandom();
		now=( now % (i-2) ) + 1;
		return 1;
	}

	//找
	for(i=1;i<0x100;i++)
	{
		if(worker[i].id==0)break;

		ret=compare(p,(char*)&worker[i].id);
		if(ret==0)
		{
			now=i;
			return 2;
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//显示，事件处理
void characterread()
{
	//主画
	worker[now].read();
/*
	//菜单
	if(worker[0].startaddr > 0)
	{
		worker[0].read();
	}
*/
}
void characterwrite(QWORD type,QWORD key)
{
	//debug
	//say("%llx,%llx\n",type,key);

	//'dropfile'
	if(type==0x656c6966706f7264)
	{
		//say("debuging::::::::%s\n",(char*)key);
		arterystart((char*)key);
		return;
	}

	//'kbd'
	else if(type==0x64626b)
	{
/*
		//按下esc
		if(key==0x1b)
		{
			worker[0].startaddr ^= 1;
			return;
		}
*/
	}

	//'touch'
	else if(type==0x6863756f74)
	{
		say("touch!\n");
	}//touch

	//debug
	//say("now=%x\n",now);
	//say("write@%llx\n",worker[now].write);
/*
	//其余所有消息，谁在干活就交给谁
	if(worker[0].startaddr > 0)worker[0].write(type,key);
	else worker[now].write(type,key);
*/
	worker[now].write(type,key);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int characterstart(DWORD size,char* addr)
{
	worker[now].start(size,addr);
	return 0;	//ret<=0:failed
}
int characterstop()
{
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void characterinit(char* type,char* addr)
{
	int i;
	char* temp;

	if(type==0)
	{
		//clean everything
		for(i=0;i<0x100000;i++)addr[i]=0;
		temp=addr;

		//
		worker=(struct working*)addr;
		mega1=addr+0x100000;
		mega2=addr+0x200000;
		mega3=addr+0x300000;

		//[+0x00,0x3f]:		menu.c
		menu_init(addr,temp);
		temp+=0x40;

		//[+0x40,+0x7f]:        1.hex.c
		hex_init(addr,temp);
		temp += 0x40;

		//[+0x80,+0xbf]:        2.2048.c
		the2048_init(addr,temp);
		temp += 0x40;

		//[+0xc0,+0xff]:        2.keyboard.c
		keyboard_init(addr,temp);
		temp += 0x40;

		//[+0x100,+0x13f]:      2.tree.c
		tree_init(addr,temp);
		temp += 0x40;

		//[+0x140,+0x17f]:      3.sketchpad.c
		sketchpad_init(addr,temp);
		temp += 0x40;

		//[+0x180,+0x1bf]:      4.console.c
		console_init(addr,temp);
		temp += 0x40;

		characterchoose("2048");
		say("[c,f):inited character\n");
	}
}
void characterkill()
{
	say("[c,f):killing character\n");

	console_kill();
	sketchpad_kill();
	tree_kill();
	keyboard_kill();
	the2048_kill();
	hex_kill();
	menu_kill();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
