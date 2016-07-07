#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//init......
void ascii_init(char*,char*);
void unicode_init(char*,char*);
void background_init(char*,char*);
void shape_init(char*,char*);
//
void menu_init(char*,char*);
void console_init(char*,char*);
void hex_init(char*,char*);
void keyboard_init(char*,char*);
void sketchpad_init(char*,char*);
void spectrum_init(char*,char*);
void the2048_init(char*,char*);
void tree_init(char*,char*);
//kill......
void menu_kill();
void console_kill();
void hex_kill();
void keyboard_kill();
void sketchpad_kill();
void spectrum_kill();
void the2048_kill();
void tree_kill();

//
int startwindow(DWORD,char*);
int stopwindow();
int arterystart(char*);
int arterystop();
int compare(char*,char*);
DWORD getrandom();
//
void say(char* , ...);




//worker
static DWORD now=0;		//不能有负数
//screen
static DWORD actualsize;
static void* actualaddr;
//touch
static QWORD pointenter[11];
static QWORD pointleave[11];
static int pointcount=0;
static int pointmax=0;




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

		//
		spectrum_init(addr,temp);
		temp += 0x40;

		now=1;
		say("[c,f):inited character\n");
	}
}
void characterkill()
{
	say("[c,f):killing character\n");

	tree_kill();
	the2048_kill();
	spectrum_kill();
	sketchpad_kill();
	keyboard_kill();
	hex_kill();
	console_kill();
	menu_kill();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int characterstart(DWORD size,char* addr)
{
	actualsize=size;
	actualaddr=addr;

	//configure window
	startwindow(actualsize,actualaddr);

	//
	worker[now].start(actualsize, actualaddr);
	worker[0].start(actualsize, actualaddr);

	return 0;
}
int characterstop()
{
	//deconfigure character
	worker[now].stop();

	//deconfigure window
	stopwindow();

	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




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
	QWORD temp;

	//
	temp=(QWORD)p;
	if(temp==1)
	{
		if(worker[now+1].id == 0)return 0;

		now++;
		worker[now].start(actualsize, actualaddr);
		worker[0].start(actualsize, actualaddr);
		return 1;
	}
	if( (temp&0xffffffff) == 0xffffffff )
	{
		if(now<=1)return 0;

		now--;
		worker[now].start(actualsize, actualaddr);
		worker[0].start(actualsize, actualaddr);
		return 1;
	}

	//exit
	ret=compare(p,"exit");
	if(ret==0)
	{
		//deathflag=1;
		say("chatacter(%d) wants to die\n",now);
		return 0;
	}

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
		say("%d\n",now);
		return 2;
	}

	//prepare for future hash?
	temp=0;
	for(i=0;i<8;i++)
	{
		if(p[i]==0)break;
		( (char*)&temp )[i]=p[i];
	}

	//start searching
	for(i=1;i<0x100;i++)
	{
		//all searched
		if(worker[i].id == 0)break;

		//lookat this
		if(worker[i].id == temp)
		{
			now=i;
			worker[now].start(actualsize, actualaddr);
			worker[0].start(actualsize, actualaddr);
			return 4;
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

	//菜单
	if(worker[0].type > 0)
	{
		worker[0].read();
	}

}
void characterwrite(QWORD type,QWORD key)
{
	int m,n;

	//debug
	//say("%llx,%llx\n",type,key);

	//size
	if(type==0x657a6973)
	{
		actualsize=key;
/*
		say("size=(%d,%d)\n",
			actualsize&0xffff,
			(actualsize>>16)&0xffff
		);
*/
		characterstart(actualsize, actualaddr);
		return;
	}//size

	//'kbd'
	else if(type==0x64626b)
	{
		//按下esc
		if(key==0x1b)
		{
			worker[0].type ^= 1;
			return;
		}
	}//kbd

	if( (type&0xff) == 'p' )
	{
		say("type=%llx,key=%llx\n",type,key);

		m = (type & 0xff00) >> 8;
		n = (key >> 48) & 0x07;
		if( m == '@' )
		{
			pointleave[n]=key;
		}//point move
		else if( m == '+' )
		{
			pointcount++;
			pointmax++;
			pointenter[n]=key;
		}//point sensed
		else if( m == '-' )
		{
			pointleave[n]=key;
			pointcount--;

			if(pointcount==0)
			{
				if(pointmax==1)
				{
					m = (pointleave[0]&0xffff)
					  - (pointenter[0]&0xffff);
					n = ((pointleave[0]>>16)&0xffff)
					  - ((pointenter[0]>>16)&0xffff);

					if( (n>-250) && (n<250) )
					{
						if(m<-250)	//left
						{
							type=0x64626b;
							key=0x25;
						}
						else if(m>250)	//right
						{
							type=0x64626b;
							key=0x27;
						}
						else	//point
						{
							type=0x7466656C207A7978;
							key&=0xffffffff;
						}
					}
					if( (m>-250) && (m<250) )
					{
						if(n<-250)	//up
						{	
							type=0x64626b;
							key=0x26;
						}
						else if(n>250)	//down
						{
							type=0x64626b;
							key=0x28;
						}
						else	//point
						{
							type=0x7466656C207A7978;
							key&=0xffffffff;
						}
					}
				}

				else if(pointmax>1)
				{
					m = ((pointleave[0]>>16)&0xffff)
                                          - ((pointenter[0]>>16)&0xffff);	//dy1
					n = ((pointleave[1]>>16)&0xffff)
                                          - ((pointenter[1]>>16)&0xffff);	//dy2
					if( (m>-250)&&(m<250)&&(n>-250)&&(n<250) )
					{
						m = (pointleave[0]&0xffff)
						  - (pointenter[0]&0xffff);
						n = (pointleave[1]&0xffff)
						  - (pointenter[1]&0xffff);
						if( (m<-250)&&(n<-250) )
						{
							characterchoose((char*)1);
						}
						else if( (m>250)&&(n>250) )
						{
							characterchoose((char*)0xffffffff);
						}
					}
				}

				pointmax=0;

			}//last one
		}//point gone

	}

	//其余所有消息，谁在干活就交给谁
	if(worker[0].type > 0)worker[0].write(type,key);
	else worker[now].write(type,key);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
