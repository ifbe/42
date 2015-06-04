#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//0:关闭确认
//1:
//2:
//3:
//4:
//5:
//0xfe:退出
//0xff:前一个
static DWORD lastone=1;
static DWORD what=1;		//4次全是主界面
void chooseoperator(BYTE want)
{
	if(want==0x80)
	{
		what=lastone;
	}
	else
	{
		lastone=what;
		what=want;
	}
}
















void printworld()
{
	int x,y;
	//主界面显示什么
	switch(what&0xff)
	{
		case 0:overview();break;
		case 1:printdisk();break;
		case 0x10:hex();break;
		case 0x11:console();break;
		case 0x20:real0();break;
		case 0x21:logic0();break;
		case 0x30:tempprint();break;
	}

	//右上角
	for(x=1024-16;x<1024;x++)
	{
		for(y=0;y<16;y++)
			point(x,y,0);
		string(0x7e,0,"0o");

		for(y=16;y<32;y++)
			point(x,y,0xffffff);
		string(0x7e,1,"0d");

		for(y=64;y<80;y++)
			point(x,y,0xff);
		string(0x7e,4,"1h");

		for(y=80;y<96;y++)
			point(x,y,0xffff00);
		string(0x7e,5,"1c");

		for(y=128;y<144;y++)
			point(x,y,0xff00);
		string(0x7e,8,"2r");

		for(y=144;y<160;y++)
			point(x,y,0xff00ff);
		string(0x7e,9,"2l");

		for(y=192;y<208;y++)
			point(x,y,0xff0000);
		string(0x7e,0xc,"3d");

		for(y=208;y<224;y++)
			point(x,y,0xffff);
	}
}
void processmessage(DWORD type,DWORD key)
{
	if(type==1)
	{
		//按下esc
		if(key==0x1b)
		{
			if(what!=0)chooseoperator(0);
			else chooseoperator(0x80);
			return;
		}
	}
	if(type==2)
	{
		int x=key&0xffff;			//四个角落
		int y=(key>>16)&0xffff;
		if(x>1024-16)
		{
			if(y<16)				//右上
			{
				chooseoperator(0);
				return;
			}
			else if(y<32)
			{
				chooseoperator(1);
				return;
			}
			else if(y<64)
			{}
			else if(y<80)
			{
				chooseoperator(0x10);
				return;
			}
			else if(y<96) //showconsole^=1;		//左上
			{
				chooseoperator(0x11);
				return;
			}
			else if(y<128)
			{}
			else if(y<144)
			{
				chooseoperator(0x20);
				return;
			}
			else if(y<160)
			{
				chooseoperator(0x21);
				return;
			}
			else if(y<192)
			{}
			else if(y<208)
			{
				chooseoperator(0x30);
				return;
			}
		}
	}

	//其余所有消息，谁在干活就交给谁
	if(what==0)overviewmessage(type,key);		//点了叉
	else if(what==1)diskmessage(type,key);		//磁盘
	else if(what==0x10)hexmessage(type,key);		//hex在干活就交给hex
	else if(what==0x11)consolemessage(type,key);		//console在干活就交给console
	else if(what==0x20)real0message(type,key);		//real0在干活就交给real0
	else if(what==0x21)logic0message(type,key);		//logic0在干活就交给logic0
	//else if(what==0x30)tempmessage(type,key);
}
















void main()
{
						//0
	diskinit();			//0

	hexinit();			//1
	consoleinit();		//1

	real0init();		//2
	logic0init();		//2

	initmaster();

	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		printworld();
		writescreen();

		//2.等事件，是退出消息就退出
		DWORD type=0;
		DWORD key=0;
		waitevent(&type,&key);
		if(type==0)return;
		//say("type=%x,key=%x\n",type,key);

		//3.处理事件，如果要求自杀就让它死
		processmessage(type,key);
		if( (what&0xff) == 0xff )return;
	}
}
