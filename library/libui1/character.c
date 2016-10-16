#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//special guys
void menu_create(char*,char*);
void roster_create(char*,char*);
void virtkbd_create(char*,char*);
//libs
void ascii_create(char*,char*);
void unicode_create(char*,char*);
void background_create(char*,char*);
void shape_create(char*,char*);
//game
void ooxx_create(char*,char*);
void snake_create(char*,char*);
void the2048_create(char*,char*);
void tetris_create(char*,char*);
void weiqi_create(char*,char*);
void xiangqi_create(char*,char*);
//test
void color_create(char*,char*);
void doodle_create(char*,char*);
void font_create(char*,char*);
void pure_create(char*,char*);
//tool
void calculator_create(char*,char*);
void console_create(char*,char*);
void control_create(char*,char*);
void hex_create(char*,char*);
void sketchpad_create(char*,char*);
void spectrum_create(char*,char*);
void tree_create(char*,char*);
void qrcode_create(char*,char*);
//
void menu_delete();
void roster_delete();
void virtkbd_delete();
//
void ascii_delete();
void unicode_delete();
void shape_delete();
void background_delete();
//
void xiangqi_delete();
void weiqi_delete();
void the2048_delete();
void tetris_delete();
void snake_delete();
void ooxx_delete();
//
void pure_delete();
void font_delete();
void doodle_delete();
void color_delete();
//
void calculator_delete();
void console_delete();
void control_delete();
void hex_delete();
void sketchpad_delete();
void tree_delete();
void spectrum_delete();
void qrcode_delete();
//
int cmp(void*,void*);
int ncmp(void*,void*,int);
int eventwrite();
u32 getrandom();
//
void say(char* , ...);




//worker
static u32 now=0;		//不能有负数
//screen
static void* pixbuf;
static void* pixfmt;
static int w;
static int h;
//touch
static u64 pointenter[10];
static u64 pointleave[10];
static int pointcount=0;
static int pointmax=0;




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
	int (*write)(void* who, void* what, void* how);
	char padding7[ 8 - sizeof(char*) ];
}*worker;
static unsigned char* mega1;
static unsigned char* mega2;
static unsigned char* mega3;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void charactercreate(char* type,char* addr)
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
	calculator_delete();

	color_delete();
	doodle_delete();
	font_delete();
	pure_delete();

	ooxx_delete();
	snake_delete();
	tetris_delete();
	the2048_delete();
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
int characterstart(char* addr, char* fmt, int width, int height)
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
int characterlist(char* p)
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
int characterchoose(char* p)
{
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void parsetouch(u64* who, u64* what, u64* key)
{
	int m,n;
	int dx0,dy0;
	int dx1,dy1;
	int dx2,dy2;

	m = *(u16*)what;
	n = ( (*key) >> 48 ) & 0x07;

	if(m == 0x4070)
	{
		//say("[%d]@@@@@@@@@@@\n",n);
		pointleave[n]=*key;
	}//point move

	else if(m == 0x2b70)
	{
		//say("[%d]++++++++++\n",n);
		pointcount++;
		pointmax++;
		pointenter[n]=*key;
	}//point sensed

	else if(m == 0x2d70)
	{
		//say("[%d]---------\n",n);

		//
		pointleave[n]=*key;
/*
		say("(%llx,%llx)->(%llx,%llx)\n",
			(pointenter[n]&0xffff),
			((pointenter[n]>>16)&0xffff),
			(pointleave[n]&0xffff),
			((pointleave[n]>>16)&0xffff)
		);
*/
		//
		pointcount--;
		if(pointcount==0)
		{
			if(pointmax==1)
			{
				pointmax=0;
				dx0 = (pointleave[0]&0xffff) - (pointenter[0]&0xffff);
				dy0 = ((pointleave[0]>>16)&0xffff) - ((pointenter[0]>>16)&0xffff);

				if( (dy0>-256) && (dy0<256) )
				{
					if(dx0<-128)	//left
					{
						*what = 0x64626b;
						*key = 0x25;
					}
					else if(dx0>128)	//right
					{
						*what = 0x64626b;
						*key = 0x27;
					}
					else	//point
					{
						*what = 0x2d6d;
						*key = (*key&0xffffffff) + ((u64)1<<48);
					}
				}
				if( (dx0>-256) && (dx0<256) )
				{
					if(dy0<-128)	//up
					{	
						*what = 0x64626b;
						*key = 0x26;
					}
					else if(dy0>128)	//down
					{
						*what = 0x64626b;
						*key = 0x28;
					}
					else	//point
					{
						*what = 0x2d6d;
						*key = (*key&0xffffffff) + ((u64)1<<48);
					}
				}
			}

			else if(pointmax==2)
			{
				pointmax=0;
				dx0 = (pointleave[0]&0xffff) - (pointenter[0]&0xffff);
				dy0 = ((pointleave[0]>>16)&0xffff) - ((pointenter[0]>>16)&0xffff);
				dx1 = (pointleave[1]&0xffff) - (pointenter[1]&0xffff);
				dy1 = ((pointleave[1]>>16)&0xffff) - ((pointenter[1]>>16)&0xffff);
			}

			else if(pointmax>=3)
			{
				pointmax=0;
				dx0 = (pointleave[0]&0xffff) - (pointenter[0]&0xffff);
				dy0 = ((pointleave[0]>>16)&0xffff) - ((pointenter[0]>>16)&0xffff);
				dx1 = (pointleave[1]&0xffff) - (pointenter[1]&0xffff);
				dy1 = ((pointleave[1]>>16)&0xffff) - ((pointenter[1]>>16)&0xffff);
				dx2 = (pointleave[2]&0xffff) - (pointenter[2]&0xffff);
				dy2 = ((pointleave[2]>>16)&0xffff) - ((pointenter[2]>>16)&0xffff);
				if( (dx0>-256)&&(dx0<256)&&(dx1>-256)&&(dx1<256)&&(dx2>-256)&&(dx2<256) )
				{
					if( (dy0 > 128)&&(dy1 > 128)&&(dy2 > 128) )
					{
						worker[1].xyze1 ^= 1;
					}
					else if( (dy0 < -128)&&(dy1 < -128)&&(dy2 < -128) )
					{
						worker[2].xyze1 ^= 1;
					}
					else
					{
						worker[0].xyze1 ^= 1;
					}
				}//3touch
			}//max>=3
		}//last one
	}//point gone
}
void characterwrite(u64 who, u64 what,u64 key)
{
	int x;

	//size
	if(what == 0x657a6973)
	{
		w = key & 0xffff;
		h = (key >> 16) & 0xffff;

		for(x=0;x<100;x++)
		{
			if(worker[x].id == 0)break;

			worker[x].width = w;
			worker[x].height = h;
		}
		return;
	}//size

	//kbd
	else if(what == 0x64626b)
	{
		//按下esc
		if(key==0x1b)
		{
			worker[0].xyze1 ^= 1;
			return;
		}
	}//kbd

	//touch
	else if( (what&0xff) == 'p' )
	{
		parsetouch(&who, &what, &key);
	}

	//virtkbd
	if(worker[2].xyze1 > 0)
	{
		x = worker[2].write(&who, &what, &key);
	}

	//其余所有消息，谁在干活就交给谁
	if(worker[0].xyze1 > 0)
	{
		//center
		x = worker[0].write(&who, &what, &key);
	}
	else if(worker[1].xyze1 > 0)
	{
		//roster
		x = worker[1].write(&who, &what, &key);
	}
	else
	{
		//player
		worker[now].write(&who, &what, &key);
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




int charactercommand(char* p)
{
	int j,k,ret;
	u64 temp;

	//exit!
	if(p == 0)
	{
		temp = (worker[0].pixelformat)&0xffffffff;
		if(temp != 0x6c6d7468)eventwrite();

		say("chatacter(%d) wants to die\n",now);
		return 0;
	}

	//exit.
	ret=cmp(p,"exit");
	if(ret==0)
	{
		temp = (worker[0].pixelformat)&0xffffffff;
		if(temp != 0x6c6d7468)eventwrite();

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
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
