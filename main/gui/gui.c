#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//0:关闭确认
//0x10:
//0x11:
//0x20:
//0x21:
//0x30:
//0x31:
static DWORD what=0x10;




static showmenu=0;
void hidemenu()
{
	showmenu=0;
}




static living=1;
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
		case 0x10:hex();break;
		case 0x11:console();break;
		case 0x20:real0();break;
		case 0x21:logic0();break;
		case 0x30:tempprint();break;
		case 0x31:jiong();break;
	}
	if(showmenu==1)menu();

	//右上角
	for(x=1024-16;x<1024;x++)
	{
		for(y=0;y<16;y++)
			point(x,y,0xff0000);
		for(y=64;y<96;y++)
			point(x,y,0xff);
		for(y=128;y<160;y++)
			point(x,y,0xff00);
		for(y=192;y<224;y++)
			point(x,y,0xff0000);
	}
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
}
void processmessage(QWORD type,QWORD key)
{
	if(type==1)		//1是键盘
	{
		//按下esc
		if(key==0x1b)
		{
			showmenu^=1;
			return;
		}
	}
	if(type==2)		//2是鼠标
	{
		int x=key&0xffff;			//四个角落
		int y=(key>>16)&0xffff;
		if(x>1024-16)
		{
			if(y<16)				//右上
			{
				showmenu^=1;
				return;
			}
			else if(y<64)
			{}
			else if(y<80)
			{
				what=0x10;
				return;
			}
			else if(y<96) //showconsole^=1;		//左上
			{
				what=0x11;
				return;
			}
			else if(y<128)
			{}
			else if(y<144)
			{
				what=0x20;
				return;
			}
			else if(y<160)
			{
				what=0x21;
				return;
			}
			else if(y<192)
			{}
			else if(y<208)
			{
				what=0x30;
				return;
			}
			else if(y<224)
			{
				what=0x31;
				return;
			}
		}
	}

	//其余所有消息，谁在干活就交给谁
	if(showmenu==1)menumessage(type,key);		//磁盘
	else if(what==0x10)hexmessage(type,key);		//hex在干活就交给hex
	else if(what==0x11)consolemessage(type,key);		//console在干活就交给console
	else if(what==0x20)real0message(type,key);		//real0在干活就交给real0
	else if(what==0x21)logic0message(type,key);		//logic0在干活就交给logic0
	else if(what==0x30)tempmessage(type,key);
	else if(what==0x31)overviewmessage(type,key);		//点了叉
}
















void main()
{
	initmaster();

	menuinit();			//0
	hexinit();			//1
	consoleinit();		//1
	real0init();		//2
	logic0init();		//2

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
		//say("type=%x,key=%x\n",type,key);

		//3.处理事件，如果要求自杀就让它死
		processmessage(type,key);
		if(living==0)return;
	}
}
