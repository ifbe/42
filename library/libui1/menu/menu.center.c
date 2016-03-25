#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int characterstart(char* p);
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
			characterstart(0);
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
			characterstart(buffer);

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
	//body
	rectangle((256<<16)+256 , (512<<16)+768  , 0);
	squareframe((256<<16)+256 , (512<<16)+768  , 0xcccccc);

	//close button
	rectangle((256<<16)+768-16 , ((256+16)<<16)+768  , 0xff0000);

	//string
	colorstring(0x20 , 16 , "what do you want?" , 0xcccccc);
	colorstring(0x20 , 17 , buffer , 0xcccccc);
}
static void menu_switch()
{
}
static void menu_list()
{
}




static void menu_start()
{
}
static void menu_stop()
{
}
void menu_init(QWORD size,void* addr)
{
	if(size==0)
	{
		this=(QWORD*)addr;
		that=(QWORD*)(addr+0x10);

		this[0]=0x776f646e6977;
		this[1]=0x38343032;
		this[2]=0;		//left,up
		this[3]=0;		//right,down
		this[4]=(QWORD)menu_list;
		this[5]=(QWORD)menu_switch;
		this[6]=(QWORD)menu_read;
		this[7]=(QWORD)menu_write;
	}
	else
	{
		//
		xsize=size&0xffff;
		ysize=(size>>16)&0xffff;

		//
		screenbuf=addr;
	}
}
void menu_kill()
{
}
