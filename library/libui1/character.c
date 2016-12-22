#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//special guys
void menu_create(u8*,u8*);
void menu_delete();
void roster_create(u8*,u8*);
void roster_delete();
void virtkbd_create(u8*,u8*);
void virtkbd_delete();
//libs
void ascii_create(u8*,u8*);
void ascii_delete();
void unicode_create(u8*,u8*);
void unicode_delete();
void background_create(u8*,u8*);
void background_delete();
void shape_create(u8*,u8*);
void shape_delete();
//game
void the2048_create(u8*,u8*);
void the2048_delete();
void ooxx_create(u8*,u8*);
void ooxx_delete();
void snake_create(u8*,u8*);
void snake_delete();
void sudoku_create(u8*,u8*);
void sudoku_delete();
void tetris_create(u8*,u8*);
void tetris_delete();
void weiqi_create(u8*,u8*);
void weiqi_delete();
void xiangqi_create(u8*,u8*);
void xiangqi_delete();
//test
void color_create(u8*,u8*);
void color_delete();
void doodle_create(u8*,u8*);
void doodle_delete();
void font_create(u8*,u8*);
void font_delete();
void pure_create(u8*,u8*);
void pure_delete();
//tool
void calculator_create(u8*,u8*);
void calculator_delete();
void circuit_create(u8*,u8*);
void circuit_delete();
void console_create(u8*,u8*);
void console_delete();
void control_create(u8*,u8*);
void control_delete();
void hex_create(u8*,u8*);
void hex_delete();
void sketchpad_create(u8*,u8*);
void sketchpad_delete();
void spectrum_create(u8*,u8*);
void spectrum_delete();
void tree_create(u8*,u8*);
void tree_delete();
void qrcode_create(u8*,u8*);
void qrcode_delete();
//
void backgroundcolor();
int cmp(void*,void*);
int ncmp(void*,void*,int);
u32 getrandom();
//
void eventwrite(u64,u64,u64,u64);
//
void say(void*, ...);




//worker
static u32 now=0;		//不能有负数
//screen
static void* pixbuf;
static void* pixfmt;
static int w;
static int h;




//[0x000000,0x0fffff]:
//[0x100000,0x1fffff]:
//[0x200000,0x2fffff]:
//[0x300000,0x3fffff]:
static struct working
{
	//种类，名字，位置，
	u64 type;	//[0,7]:种类	    //'window'
	u64 id;	//[8,f]:名字	    //'小明'
	u64 xyze1;	//x,y,z,en
	u64 xyze2;

	//screenbuffer
	u64 pixelbuffer;	//address
	u64 pixelformat;	//rgba8888    bgra8888    rgb565    yuv420
	u64 width;
	u64 height;

	//[40,47]
	int (*create)();
	char padding0[ 8 - sizeof(char*) ];

	//[48,4f]
	int (*destory)();
	char padding1[ 8 - sizeof(char*) ];

	//[50,57]:开始
	int (*start)();
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
	int (*write)(void* where, void* who, void* what);
	char padding7[ 8 - sizeof(char*) ];
}*worker;
static u8* mega1;
static u8* mega2;
static u8* mega3;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void charactercreate(u8* type, u8* addr)
{
	int i;
	u8* temp;
	if(type!=0)
	{
		//insmod(xxxx)
		return;
	}

	//clean everything
	for(i=0;i<0x100000;i++)addr[i]=0;
	temp=addr;

	//
	worker=(struct working*)addr;
	mega1=addr+0x100000;
	mega2=addr+0x200000;
	mega3=addr+0x300000;

	//menu.center
	menu_create(addr,temp);
	temp+=0x80;

	//menu.roster
	roster_create(addr,temp);
	temp += 0x80;

	//menu.virtkbd
	virtkbd_create(addr,temp);
	temp += 0x80;

	//ascii
	ascii_create(addr,temp);
	temp+=0x80;

	//unicode
	unicode_create(addr,temp);
	temp+=0x80;

	//background
	background_create(addr,temp);
	temp+=0x80;

	//shape
	shape_create(addr,temp);
	temp+=0x80;

	//game.2048
	the2048_create(addr,temp);
	temp += 0x80;

	//game.ooxx
	ooxx_create(addr,temp);
	temp += 0x80;

	//game.snake
	snake_create(addr,temp);
	temp += 0x80;

	//game.sudoku
	sudoku_create(addr,temp);
	temp += 0x80;

	//game.tetris
	tetris_create(addr,temp);
	temp += 0x80;

	//game.weiqi
	weiqi_create(addr,temp);
	temp += 0x80;

	//game.xiangqi
	xiangqi_create(addr,temp);
	temp += 0x80;

	//test.color
	color_create(addr,temp);
	temp += 0x80;

	//test.font
	font_create(addr,temp);
	temp += 0x80;

	//test.doodle
	doodle_create(addr,temp);
	temp += 0x80;

	//test.pure
	pure_create(addr,temp);
	temp += 0x80;

	//tool.calculator
	calculator_create(addr,temp);
	temp += 0x80;

	//tool.circuit
	circuit_create(addr,temp);
	temp += 0x80;

	//tool.console
	console_create(addr,temp);
	temp += 0x80;

	//tool.hex
	hex_create(addr,temp);
	temp += 0x80;

	//tool.control
	control_create(addr,temp);
	temp += 0x80;

	//tool.qrcode
	qrcode_create(addr,temp);
	temp += 0x80;

	//tool.tree
	tree_create(addr,temp);
	temp += 0x80;

	//tool.sketchpad
	sketchpad_create(addr,temp);
	temp += 0x80;

	//tool.spectrum
	spectrum_create(addr,temp);
	temp += 0x80;

	for(now=0;now<100;now++)
	{
		if(worker[now].type != 0)break;
	}
	say("[c,f):createed character\n");
}
void characterdelete()
{
	say("[c,f):deleteing character\n");

	spectrum_delete();
	tree_delete();
	sketchpad_delete();
	hex_delete();
	qrcode_delete();
	control_delete();
	console_delete();
	circuit_delete();
	calculator_delete();

	color_delete();
	doodle_delete();
	font_delete();
	pure_delete();

	the2048_delete();
	ooxx_delete();
	snake_delete();
	sudoku_delete();
	tetris_delete();
	weiqi_delete();
	xiangqi_delete();

	ascii_delete();
	background_delete();
	shape_delete();
	unicode_delete();

	menu_delete();
	roster_delete();
	virtkbd_delete();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int characterstart(u8* addr, u8* fmt, int width, int height)
{
	int j;

	pixbuf = addr;
	pixfmt = fmt;
	w = width;
	h = height;
	//say("@characterstart:%d,%d\n", w, h);

	for(j=0;j<100;j++)
	{
		if(worker[j].id == 0)break;

		worker[j].pixelbuffer = (u64)addr;
		worker[j].pixelformat = *(u64*)fmt;
		worker[j].width = w;
		worker[j].height = h;
	}

	//
	worker[now].start();
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
int characterlist(u8* p)
{
	//列出所有“人物”
	int j;
	int ret;
	if(p == 0)
	{
		for(j=0;j<256;j++)
		{
			if(worker[j].id != 0)
			{
				say("%d:%s\n",j,&worker[j].id);
			}
		}
		return 0;
	}

	else
	{
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
			//say("[%s][%s]\n",&worker[j].id, p);
			ret = ncmp(&worker[j].id, p, 8);
			if(ret == 0)return j;
		}
		return 0;
	}
}
int characterchoose(u8* p)
{
	return 0;
}




void characterwrite(u64* p)
{
	int x;

	//size
	if(p[1] == 0x657a6973)
	{
		w = p[0] & 0xffff;
		h = (p[0] >> 16) & 0xffff;

		for(x=0;x<100;x++)
		{
			if(worker[x].id == 0)break;

			worker[x].width = w;
			worker[x].height = h;

			//cleanup screen
			backgroundcolor(0);
		}
		return;
	}//size

	//kbd
	else if(p[1] == 0x64626b)
	{
		//按下esc
		if(p[0] == 0x1b)
		{
			worker[0].xyze1 ^= 1;
			return;
		}
	}//kbd

	//virtkbd
	if(worker[2].xyze1 > 0)
	{
		x = worker[2].write(p+2, p+1, p);
	}

	//其余所有消息，谁在干活就交给谁
	if(worker[0].xyze1 > 0)
	{
		//center
		x = worker[0].write(p+2, p+1, p);
	}
	else if(worker[1].xyze1 > 0)
	{
		//roster
		x = worker[1].write(p+2, p+1, p);
	}
	else
	{
		//player
		worker[now].write(p+2, p+1, p);
	}
}
void characterread()
{
	//主画
	worker[now].read();

	//菜单
	if(worker[0].xyze1 > 0)worker[0].read();
	if(worker[1].xyze1 > 0)worker[1].read();
	if(worker[2].xyze1 > 0)worker[2].read();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




int charactercommand(u8* p)
{
	int j,k,ret;
	u64 temp;

	//exit!
	if(p == 0)
	{
		temp = (worker[0].pixelformat)&0xffffffff;
		if(temp != 0x6c6d7468)eventwrite(0,0,0,0);

		say("chatacter(%d) wants to die\n",now);
		return 0;
	}

	//exit.
	ret=cmp(p,"exit");
	if(ret==0)
	{
		temp = (worker[0].pixelformat)&0xffffffff;
		if(temp != 0x6c6d7468)eventwrite(0,0,0,0);

		say("chatacter(%d) wants to die\n",now);
		return 0;
	}

	//next
	ret=cmp(p,"+");
	if(ret==0)
	{
		if(worker[now+1].id == 0)return 0;
		now++;
		goto found;
	}

	//last
	ret=cmp(p,"-");
	if(ret==0)
	{
		if(worker[now-1].type == 0)return 0;
		now--;
		goto found;
	}

	//random
	ret=cmp(p,"random");
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

	//search
	ret = characterlist(p);
	if(ret != 0)
	{
		now = ret;
		goto found;
	}

	//ret = arterycommand(p);

found:
	//worker[0].xyze1 = 0;
	characterstart(pixbuf, pixfmt, w, h);
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
