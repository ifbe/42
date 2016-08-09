#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//special guys
void menu_init(char*,char*);
void menu1_init(char*,char*);
void menu2_init(char*,char*);
void menu3_init(char*,char*);
void menu4_init(char*,char*);
void ascii_init(char*,char*);
void unicode_init(char*,char*);
void background_init(char*,char*);
void shape_init(char*,char*);
//game
void snake_init(char*,char*);
void the2048_init(char*,char*);
void tetris_init(char*,char*);
//test
void color_init(char*,char*);
void doodle_init(char*,char*);
void font_init(char*,char*);
//tool
void console_init(char*,char*);
void hex_init(char*,char*);
void keyboard_init(char*,char*);
void sketchpad_init(char*,char*);
void spectrum_init(char*,char*);
void tree_init(char*,char*);
void qrcode_init(char*,char*);
//
void menu_kill();
void ascii_kill();
void unicode_kill();
void shape_kill();
void background_kill();
//
void the2048_kill();
void tetris_kill();
void snake_kill();
//
void font_kill();
void doodle_kill();
void color_kill();
//
void console_kill();
void hex_kill();
void keyboard_kill();
void sketchpad_kill();
void tree_kill();
void spectrum_kill();
void qrcode_kill();
//
int compare(char*,char*);
int writeevent();
DWORD getrandom();
//
void say(char* , ...);




//worker
static DWORD now=0;		//不能有负数
//screen
static DWORD actualsize;
static void* actualaddr;
//touch
static QWORD pointenter[10];
static QWORD pointleave[10];
static int pointcount=0;
static int pointmax=0;




//[0x000000,0x0fffff]:
//[0x100000,0x1fffff]:
//[0x200000,0x2fffff]:
//[0x300000,0x3fffff]:
static struct working
{
	//种类，名字，位置，
	QWORD type;	//[0,7]:种类	    //'window'
	QWORD id;	//[8,f]:名字	    //'小明'
	QWORD xyze1;	//x,y,z,en
	QWORD xyze2;

	//screenbuffer
	QWORD pixelbuffer;	//address
	QWORD pixelformat;	//rgba8888    bgra8888    rgb565    yuv420
	QWORD width;
	QWORD height;

	//[40,47]
	void (*create)();
	char padding0[ 8 - sizeof(char*) ];

	//[48,4f]
	void (*destory)();
	char padding1[ 8 - sizeof(char*) ];

	//[50,57]:开始
	int (*start)(DWORD size,char* addr);
	char padding2[ 8 - sizeof(char*) ];

	//[58,5f]:结束
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[60,67]:状态
	int (*list)();
	char padding4[ 8 - sizeof(char*) ];

	//[68,6f]:跳关
	int (*choose)();
	char padding5[ 8 - sizeof(char*) ];

	//[70,77]:输出
	int (*read)();
	char padding6[ 8 - sizeof(char*) ];

	//[78,7f]:输入
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
		temp+=0x80;

		//ascii
		ascii_init(addr,temp);
		temp+=0x80;

		//unicode
		unicode_init(addr,temp);
		temp+=0x80;

		//background
		background_init(addr,temp);
		temp+=0x80;

		//shape
		shape_init(addr,temp);
		temp+=0x80;

		//game.2048
		the2048_init(addr,temp);
		temp += 0x80;

		//game.tetris
		tetris_init(addr,temp);
		temp += 0x80;

		//game.snake
		snake_init(addr,temp);
		temp += 0x80;

		//test.color
		color_init(addr,temp);
		temp += 0x80;

		//test.font
		font_init(addr,temp);
		temp += 0x80;

		//test.doodle
		doodle_init(addr,temp);
		temp += 0x80;

		//tool.console
		console_init(addr,temp);
		temp += 0x80;

		//tool.hex
		hex_init(addr,temp);
		temp += 0x80;

		//tool.keyboard
		keyboard_init(addr,temp);
		temp += 0x80;

		//tool.qrcode
		qrcode_init(addr,temp);
		temp += 0x80;

		//tool.tree
		tree_init(addr,temp);
		temp += 0x80;

		//tool.sketchpad
		sketchpad_init(addr,temp);
		temp += 0x80;

		//tool.spectrum
		spectrum_init(addr,temp);
		temp += 0x80;

		for(now=0;now<100;now++)
		{
			if(worker[now].type != 0)break;
		}
		say("[c,f):inited character\n");
	}
}
void characterkill()
{
	say("[c,f):killing character\n");

	spectrum_kill();
	tree_kill();
	sketchpad_kill();
	keyboard_kill();
	hex_kill();
	console_kill();
	qrcode_kill();

	color_kill();
	doodle_kill();
	font_kill();

	snake_kill();
	tetris_kill();
	the2048_kill();

	ascii_kill();
	background_kill();
	shape_kill();
	unicode_kill();
	menu_kill();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int characterstart(DWORD size,char* addr)
{
	int j;
	for(j=0;j<100;j++)
	{
		if(worker[j].type != 0)break;

		worker[j].width=size&0xffff;
		worker[j].height=(size>>16)&0xffff;
		worker[j].pixelbuffer=(QWORD)addr;
	}

	actualsize=size;
	actualaddr=addr;

	//
	worker[now].start(actualsize, actualaddr);
	worker[0].start(actualsize, actualaddr);

	worker[now].choose();
	return 0;
}
int characterstop()
{
	//deconfigure character
	worker[now].stop();
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void characterlist()
{
	//列出所有“人物”
	int x;
	for(x=0;x<256;x++)
	{
		if(worker[x].id != 0)
		{
			say("%d:%s\n",x,&worker[x].id);
		}
	}
}
int characterchoose(char* p)
{
	int j,k,ret;
	QWORD temp;

	//exit
	ret=compare(p,"exit");
	if(ret==0)
	{
		//deathflag=1;
		say("chatacter(%d) wants to die\n",now);
		writeevent();
		return 0;
	}

	//
	ret=compare(p,"+");
	if(ret==0)
	{
		if(worker[now+1].id == 0)return 0;
		now++;
		goto found;
	}

	ret=compare(p,"-");
	if(ret==0)
	{
		if(worker[now-1].type == 0)return 0;
		now--;
		goto found;
	}

	//random
	ret=compare(p,"random");
	if(ret==0)
	{
		for(j=0;j<10;j++)
		{
			if(worker[j].type != 0)break;	//skip menu|draw
		}
		k=j;

		for(;k<0x100000/0x80;k++)
		{
			if(worker[k].id == 0)break;
		}

		now=( getrandom() % (k-j) ) + j;
		goto found;
	}

	//start searching
	for(j=0;j<10;j++)
	{
		if(worker[j].type != 0)break;	//skip menu|draw
	}
	for(;j<0x100;j++)
	{
		//all searched
		if(worker[j].id == 0)return 0;

		//lookat this
		if(worker[j].id == temp)
		{
			now=j;
			goto found;
		}
	}

found:
	characterstart(actualsize, actualaddr);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//显示，事件处理
void characterread()
{
	//主画
	worker[now].read();

	//菜单
	if(worker[0].xyze1 > 0)
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
		characterstart(actualsize, actualaddr);
		return;
	}//size

	//'kbd'
	else if(type==0x64626b)
	{
		//按下esc
		if(key==0x1b)
		{
			worker[0].xyze1 ^= 1;
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
			//
			pointleave[n]=key;
			say("(%llx,%llx)->(%llx,%llx)\n",
				(pointenter[n]&0xffff),
				((pointenter[n]>>16)&0xffff),
				(pointleave[n]&0xffff),
				((pointleave[n]>>16)&0xffff)
			);

			//
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
							characterchoose("+");
						}
						else if( (m>250)&&(n>250) )
						{
							characterchoose("-");
						}
					}
				}

				pointmax=0;

			}//last one
		}//point gone

	}

	//其余所有消息，谁在干活就交给谁
	if(worker[0].xyze1 > 0)worker[0].write(type,key);
	else worker[now].write(type,key);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
