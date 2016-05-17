#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int characterchoose(char* p);
void squareframe(QWORD leftup,QWORD rightdown,DWORD color);
void rectangle(QWORD leftup,QWORD rightdown,DWORD color);
void colorstring(int x,int y,char* str,unsigned int color);
void say(char*,...);




//
static int xsize;
static int ysize;
static DWORD* screenbuf;
//菜单
static char buffer[128];
static int bufp=0;
//
static QWORD* this=0;
static QWORD* that=0;




//write,read,into,list
static void menu_write(QWORD type,QWORD key)
{
	say("%s,%llx\n",&type,key);

	//'xyz left'
	if(type==0x7466656C207A7978)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;

		//点击框框外面，关掉菜单
		if( (x<256)|(x>768)|(y<256)|(y>512) )
		{
			that[0]=0;
			return;
		}

		//点击红色矩形，退出
		if( (y<256+16) && (x>768-16) )
		{
			//退出
			characterchoose(0);
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
			characterchoose(buffer);

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
static void menu_read()
{
	int leftupper=((ysize/4)<<16) + (xsize/4);
	int rightbottom=((ysize*3/4)<<16) + (xsize*3/4);

	//body
	rectangle(leftupper, rightbottom, 0);
	squareframe(leftupper, rightbottom, 0xcccccc);

	//close button
	//rectangle((256<<16)+768-16 , rightbottom+768  , 0xff0000);

	//string
	colorstring(xsize/8/4, ysize/16/4, "what do you want?" , 0xcccccc);
	colorstring(xsize/8/4, ysize/16/4+1, buffer , 0xcccccc);
}
static void menu_switch()
{
}
static void menu_list()
{
}




static void menu_start(DWORD size,void* addr)
{
	//
	xsize=size&0xffff;
	ysize=(size>>16)&0xffff;

	//
	screenbuf=addr;
}
static void menu_stop()
{
}
void menu_init(char* base,char* addr)
{
	this=(QWORD*)addr;
	that=(QWORD*)(addr+0x10);

	this[0]=0;
	this[1]=0x756e656d;
	this[2]=(QWORD)menu_start;
	this[3]=(QWORD)menu_stop;
	this[4]=(QWORD)menu_list;
	this[5]=(QWORD)menu_switch;
	this[6]=(QWORD)menu_read;
	this[7]=(QWORD)menu_write;
}
void menu_kill()
{
}
