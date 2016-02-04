#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void initmenu(char*);		//menu.c
void inithex(char*);                //1.hex.c
void init2048(char*);               //2.2048.c
void initkeyboard(char*);           //2.keyboard.c
void inittree(char*);               //2.tree.c
void initsketchpad(char*);          //3.sketchpad.c
void initconsole(char*);            //4.console.c
//
void initascii(char*);
void initunicode(char*);
void initbackground(char*);
void initshape(char*);
void writewindow();
//
DWORD getrandom();
void softinto(char*);
void say(char* , ...);




//苦工
struct working
{
        //[0,7]:种类            //'window'
        QWORD type;

        //[8,f]:名字            //'小明'
        QWORD id;

        //[10,17]:开始          //x+(y<<16)+(z<<32)+(w<<48)
        QWORD startaddr;

        //[18,1f]:结束          //x+(y<<16)+(z<<32)+(w<<48)
        QWORD endaddr;

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
static struct working* worker;




static unsigned int now=0;		//不能有负数
int uicommand(char* p)
{
	int i;
	unsigned long long temp;
	char buf[8];

	//get
        *(unsigned long long*)buf=0;
        for(i=0;i<8;i++)
        {
                if(p[i]<0x20)break;
		else buf[i]=p[i];
        }
	temp=*(unsigned long long*)buf;

	//faceless
        if(temp == 0x7373656c65636166)
        {
                for(i=1;i<0x1000/0x40;i++)
                {
                        if(worker[i].id == 0)break;
                }

                now=getrandom();
                now=( now % (i-2) ) + 1;
                say("random=%x\n",now);

                worker[0].startaddr=0;
                worker[now].into();
                return 1;
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
                        return 2;
                }
        }

	return 0;	//ret<=0:failed
}
//显示，事件处理
void printworld()
{
        //主画
        //say("background\n");
        worker[now].read();

        //菜单
        //say("menu\n");
        if(worker[0].startaddr > 0)worker[0].read();

        //上屏
        //say("window\n");
        writewindow();
}
void processmessage(QWORD type,QWORD key)
{
        say("%s,%llx\n",&type,key);

        //'dropfile'
        if(type==0x656c6966706f7264)
        {
                //say("debuging::::::::%s\n",(char*)key);
                softinto((char*)key);
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
        if(worker[0].startaddr > 0)worker[0].write(type,key);
        else worker[now].write(type,key);
}








void initcharacter(char* addr)
{
	//clean everything
	int i;
	for(i=0;i<0x100000;i++)addr[i]=0;
	worker=(struct working*)addr;

	//words
	initascii(addr);
	initunicode(addr);

	//pictures
	initbackground(addr);
	initshape(addr);

	//[+0x00,0x3f]:		menu.c
	initmenu(addr);
	addr+=0x40;

	//[+0x40,+0x7f]:        1.hex.c
	inithex(addr);
	addr += 0x40;

	//[+0x80,+0xbf]:        2.2048.c
	init2048(addr);
	addr += 0x40;

	//[+0xc0,+0xff]:        2.keyboard.c
	initkeyboard(addr);
	addr += 0x40;

	//[+0x100,+0x13f]:      2.tree.c
	inittree(addr);
	addr += 0x40;

	//[+0x140,+0x17f]:      3.sketchpad.c
	initsketchpad(addr);
	addr += 0x40;

	//[+0x180,+0x1bf]:      4.console.c
	initconsole(addr);
	addr += 0x40;

	//default face
	uicommand("2048");
}
void killcharacter()
{
}
