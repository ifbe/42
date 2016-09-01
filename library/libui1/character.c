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
void console_delete();
void control_delete();
void hex_delete();
void sketchpad_delete();
void tree_delete();
void spectrum_delete();
void qrcode_delete();
//
int compare(char*,char*);
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
	void (*create)();
	char padding0[ 8 - sizeof(char*) ];

	//[48,4f]
	void (*destory)();
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
	int (*write)(void* type, void* key);
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
	say("@characterstart:%d,%d\n", w, h);

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
	char q[8];
	u64 temp;

	//exit!
	if(p == 0)
	{
		temp = (worker[0].pixelformat)&0xffffffff;
		if(temp != 0x6c6d7468)eventwrite;

		say("chatacter(%d) wants to die\n",now);
		return 0;
	}

	//exit.
	ret=compare(p,"exit");
	if(ret==0)
	{
		temp = (worker[0].pixelformat)&0xffffffff;
		if(temp != 0x6c6d7468)eventwrite();

		say("chatacter(%d) wants to die\n",now);
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

	//prepare searching
	for(j=0;j<8;j++)
	{
		if(p[j] == 0)break;
		q[j] = p[j];
	}
	for(;j<8;j++)
	{
		q[j] = 0;
	}
	temp = *(u64*)q;

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
		if(temp == worker[j].id)
		{
			now=j;
			goto found;
		}
	}

found:
	characterstart(pixbuf, pixfmt, w, h);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//显示，事件处理
void characterread()
{
	//主画
	worker[now].read();

	//菜单
	if(worker[0].xyze1 > 0)worker[0].read();
	if(worker[1].xyze1 > 0)worker[1].read();
	if(worker[2].xyze1 > 0)worker[2].read();
}
void characterwrite(u64 type,u64 key)
{
	int m,n;
	int dx0,dy0;
	int dx1,dy1;
	int dx2,dy2;

	//debug
	//say("%llx,%llx\n",type,key);

	//size
	if(type==0x657a6973)
	{
		w = key & 0xffff;
		h = (key >> 16) & 0xffff;

		for(m=0;m<100;m++)
		{
			if(worker[m].id == 0)break;

			worker[m].width = w;
			worker[m].height = h;
		}
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
		//say("type=%llx,key=%llx\n",type,key);

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
							type=0x64626b;
							key=0x25;
						}
						else if(dx0>128)	//right
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
					if( (dx0>-256) && (dx0<256) )
					{
						if(dy0<-128)	//up
						{	
							type=0x64626b;
							key=0x26;
						}
						else if(dy0>128)	//down
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
						else if( (dy0>-128)&&(dy0<128)&&(dy1>-128)&&(dy1<128)&&(dy2>-128)&&(dy2<128) )
						{
							worker[0].xyze1 ^= 1;
							return;
						}
					}
					else if( (dy0>-256)&&(dy0<256)&&(dy1>-256)&&(dy1<256)&&(dy2>-256)&&(dy2<256) )
					{
						if( (dx0 < -128)&&(dx1 < -128)&&(dx2 < -128) )
						{
							characterchoose("+");
						}
						else if( (dx0 > 128)&&(dx1 > 128)&&(dx2 > 128) )
						{
							characterchoose("-");
						}
						else if( (dx0>-128)&&(dx0<128)&&(dx1>-128)&&(dx1<128)&&(dx2>-128)&&(dx2<128) )
						{
							worker[0].xyze1 ^= 1;
							return;
						}
					}
				}

			}//last one
		}//point gone

	}

	//其余所有消息，谁在干活就交给谁
	if(worker[0].xyze1 > 0)
	{
		//center
		m = worker[0].write(&type,&key);
	}
	else if(worker[1].xyze1 > 0)
	{
		//roster
		m = worker[1].write(&type,&key);
	}
	else if(worker[2].xyze1 > 0)
	{
		//virtkbd
		m = worker[2].write(&type,&key);
		if(m > 0)worker[now].write(&type,&key);
	}
	else
	{
		//player
		worker[now].write(&type,&key);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
