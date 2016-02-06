#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int command(char* p);
void squareframe(QWORD leftup,QWORD rightdown,DWORD color);
void rectangle(QWORD leftup,QWORD rightdown,DWORD color);
void colorstring(int x,int y,char* str,unsigned int color);




//
static int xsize;
static int ysize;
static DWORD* screenbuf;
//菜单
static char buffer[128];
static int bufp=0;
//enable and disable
static char* thataddr=0;




//write,read,into,list
void writemenu(QWORD type,QWORD key)
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
			thataddr[0]=0;
			return;
		}

		//点击红色矩形，退出
		if( (y<256+16) && (x>768-16) )
		{
			//退出
			command(0);
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
			command(buffer);

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
void readmenu()
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
static void intomenu()
{
}
void listmenu(QWORD* this)
{
	thataddr=(char*)this+0x10;

	this[0]=0x776f646e6977;
	this[1]=0x38343032;
	this[2]=0;		//left,up
	this[3]=0;		//right,down
	this[4]=(QWORD)listmenu;
	this[5]=(QWORD)intomenu;
	this[6]=(QWORD)readmenu;
	this[7]=(QWORD)writemenu;
}




void initmenu(QWORD size,void* addr)
{
	//
	xsize=size&0xffff;
	ysize=(size>>16)&0xffff;

	//
	screenbuf=addr;
}
void killmenu()
{
}
