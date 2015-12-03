#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




void initmaster(QWORD);
void menuinit();	//0
void f1init();
void f2init();
void f3init();
void f4init();

void menushow();
void f1show();
void f2show();
void f3show();
void f4show();

void menumessage(QWORD type,QWORD key);
void f1message(QWORD type,QWORD key);
void f2message(QWORD type,QWORD key);
void f3message(QWORD type,QWORD key);
void f4message(QWORD type,QWORD key);

void point(int x,int y,DWORD color);
void string(int x,int y,char* str);
void writescreen();

void listmemory();
void intomemory(char* in);

void waitevent(QWORD* first,QWORD* second);
void memoryandface();
void cleanall();




//0:关闭确认
//0x10:
//0x11:
//0x20:
//0x21:
//0x30:
//0x31:
static DWORD what=1;
static int showmenu=0;
static int living=1;




void hidemenu()
{
	showmenu=0;
}
void die()
{
	living=0;
}




void printworld()
{
	int x,y;

	//主界面显示什么
	switch(what&0xff)
	{
		case 1:f1show();break;
		case 2:f2show();break;
		case 3:f3show();break;
		case 4:f4show();break;
	}
	if(showmenu==1)menushow();

	//右上角
	for(x=1024-16;x<1024;x++)
	{
		//for(y=0;y<16;y++)
			//point(x,y,0xff0000);
		/*
		for(y=64;y<96;y++)
			point(x,y,0xff);
		for(y=128;y<160;y++)
			point(x,y,0xff00);
		for(y=192;y<224;y++)
			point(x,y,0xff0000);
		*/
	}
	/*
	for(x=1024-8;x<1024;x++)
	{
		for(y=64;y<80;y++)
			point(x,y,0xff00ff);
		string(0x7e,4,"1h");
		for(y=80;y<96;y++)
			point(x,y,0xffff);
		string(0x7e,5,"1c");

		for(y=128;y<144;y++)
			point(x,y,0xffff00);
		string(0x7e,8,"2r");
		for(y=144;y<160;y++)
			point(x,y,0xffff);
		string(0x7e,9,"2l");

		for(y=192;y<208;y++)
			point(x,y,0xff00ff);
		string(0x7e,0xc,"3d");
		for(y=208;y<224;y++)
			point(x,y,0xffff00);
		string(0x7e,0xd,"3j");
	}
	*/
}
void processmessage(QWORD type,QWORD key)
{
	if(type==0x656c6966706f7264)		//dropfile
	{
		//diary("debuging::::::::%s\n",(char*)key);
		intomemory((char*)key);
		return;
	}
	//else if(type==)		//xychange
	//{
		//change xsize,ysize
		//return;
	//}
	else if(type==0x64626b)		//kbd
	{
		if(key==0x1b)
		{
			showmenu^=1;
			menuinit();		//0
			return;
		}		//按下esc
		if(key==0x70)
		{
			what=1;
			f1init();			//1
			return;
		}		//f1
		if(key==0x71)
		{
			what=2;
			f2init();			//2
			return;
		}		//f2
		if(key==0x72)
		{
			what=3;
			f3init();			//3
			return;
		}		//f3
		if(key==0x73)
		{
			what=4;
			f4init();			//4
			return;
		}		//f4
	}
	else if(type==0x7466656c)		//鼠标
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		if(x>1024-16)
		{
			if(y<16)				//右上
			{
				showmenu^=1;
				return;
			}
		}
	}

	//其余所有消息，谁在干活就交给谁
	if(showmenu==1)menumessage(type,key);		//磁盘
	else if(what==1)f1message(type,key);		//hex在干活就交给hex
	else if(what==2)f2message(type,key);		//logic0在干活就交给logic0
	else if(what==3)f3message(type,key);		//点了叉
	else if(what==4)f4message(type,key);		//console在干活就交给console
}
















void main()
{
	memoryandface();
	f1init();			//1
	f2init();			//2
	f3init();			//3
	f4init();			//4

	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		printworld();
		writescreen();

		//2.等事件，是退出消息就退出
		QWORD type=0;
		QWORD key=0;
		waitevent(&type,&key);
		if(type==0)return;

		//3.处理事件，如果要求自杀就让它死
		processmessage(type,key);
		if(living==0)return;
	}
	cleanall();
}
