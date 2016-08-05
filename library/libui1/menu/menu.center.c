#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int characterchoose(char* p);
//
void printstring(char* str,DWORD xyz,DWORD fg,DWORD bg);
void rect(int x1, int y1, int x2, int y2, DWORD bodycolor, DWORD framecolor);
//
void say(char*,...);




//
static int width;
static int height;
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
		if(	(x<width/4) |
			(x>width*3/4) |
			(y>(height*3/4)&0xfffffff0) |
			(y<height/4) )
		{
			that[0]=0;
			return;
		}

		//点击红色矩形，退出
		else if( (y<256+16) && (x>768-16) )
		{
			//退出
			characterchoose("exit");
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
	//title
	rect(
		width/4,
		(height/4)&0xfffffff0,
		width*3/4,
		(height/4+16)&0xfffffff0,
		0x01234567,
		0xfedcba98
	);

	//body
	rect(
		width/4,
		(height/4+16)&0xfffffff0,
		width*3/4,
		height*3/4,
		0,
		0xffffffff
	);

	//button
	rect(
		(width*3/4) - 16,
		(height/4)&0xfffffff0,
		width*3/4,
		((height/4) + 16)&0xfffffff0,
		0xff0000,
		0
	);

	//string
	printstring(
		"what do you want?",
		(width/16 - 8) + ((height/64)<<8),
		0xffffffff,
		0
	);
	printstring(
		buffer,
		(width/32) + ((height/64 + 1)<<8),
		0xffffffff,
		0
	);
}
static void menu_choose()
{
}
static void menu_list()
{
}




static void menu_start(DWORD size,void* addr)
{
	//
	width=size&0xffff;
	height=(size>>16)&0xffff;

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
	this[5]=(QWORD)menu_choose;
	this[6]=(QWORD)menu_read;
	this[7]=(QWORD)menu_write;
}
void menu_kill()
{
}
