#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

static int realorlogic=0;




void clearscreen()
{
	//清屏
	int x,y;
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0x88888888);
		}
	}
}
void foreground()
{
	//4个角落
	QWORD x,y;

	//左上角
	for(x=0;x<16;x++)
	{
		point(x,0,0xffffff);
		point(x,15,0xffffff);
	}
	for(y=0;y<16;y++)
	{
		point(0,y,0xffffff);
		point(15,y,0xffffff);
	}

	//右上角
	for(x=1024-16;x<1024;x++)
		for(y=0;y<16;y++)
			point(x,y,0xff0000);

	//左下角
	for(x=0;x<16;x++)
		for(y=640-16;y<640;y++)
			point(x,y,0xff);

	//右下角
	for(x=1024-16;x<1024;x++)
		for(y=640-16;y<640;y++)
			point(x,y,0xff00);
}
void printworld()
{
	clearscreen();

	//
	if(realorlogic==0)real0();
	else logic0();

	//
	printlog();
	printdisk();

	foreground();		//四点
}
void main()
{
	QWORD realworld;
	QWORD logicworld;
	whereisrealworld(&realworld);
	whereislogicworld(&logicworld);

	real0init(realworld);
	logic0init(logicworld+0x100000);
	initdiskman();
	initconsole();
	initmaster();

	while(1)
	{
		//1.这次显示啥
		printworld();			//内容
		writescreen();		//写屏

		//2.等事件
		DWORD type=0;
		DWORD key=0;
		waitevent(&type,&key);
		//say("type=%x,key=%x\n",type,key);

		//3.干啥事
		switch(type)
		{
			case 0:return;
			case 1:				//键盘
			{
				if(key==0x1b)return;
				say("keyboard:%x\n",key);

				//loginput(key);
				real0kbd(key);

				break;
			}
			case 2:				//鼠标
			{
				int x=key&0xffff;
				int y=(key>>16)&0xffff;
				say("mouse:(%d,%d)\n",x,y);

				if(y<16)
				{
					if(x>1024-16) return;			//右上
					else if(x<16) touchconsole();	//左上
				}
				//最下面两行，控制面板
				else if(y>640-16)
				{
					if(x<16) touchdisk();			//左下
					else if(x>1024-16)realorlogic^=1;	//右下
					else real0mouse(x,y);
				}

				break;
			}
		}//switch
	}//while(1)
}
