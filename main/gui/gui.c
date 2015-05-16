#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

static char what=0;
//static char showclose=0;
//static char showconsole=0;
//static char showreal=0;
//static char showlogic=0;




void printworld()
{
	int x,y;
	//主界面显示什么
	if(what==1)
	{
		real0();
	}
	else if(what==2)
	{
		logic0();
	}
	else if(what==3)
	{
		printlog();
	}
	else if(what==4)
	{
		printdisk();
	}
	else
	{
		//
		printdisk();

		//左上角
		for(x=16;x<80;x++)
			for(y=0;y<16;y++)
				point(x,y,0x77777777);
		string(2,0,"console");
		//右上角
		for(x=1024-80;x<1024-16;x++)
			for(y=0;y<16;y++)
				point(x,y,0x770000);
		string(0x78,0,"close");
		//左下角
		for(x=16;x<80;x++)
			for(y=640-16;y<640;y++)
				point(x,y,0x77);
		string(2,39,"real");
		//右下角
		for(x=1024-80;x<1024-16;x++)
			for(y=640-16;y<640;y++)
				point(x,y,0x7700);
		string(0x78,39,"logical");
	}
	//左上角
	for(x=0;x<16;x++)
		for(y=0;y<16;y++)
			point(x,y,0xffffffff);
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

				if(what==1)real0kbd(key);
				else if(what==2)logic0kbd(key);
				else if(what==3)loginput(key);

				break;
			}
			case 2:				//鼠标
			{
				int x=key&0xffff;
				int y=(key>>16)&0xffff;
				say("mouse:(%d,%d)\n",x,y);

				//四个角落
				if(y>640-16)
				{
					if(x<16) //showreal^=1;				//左下
					{
						if(what==1)what=0;
						else what=1;

						break;
					}
					else if(x>1024-16)//showlogic^=1;		//右下
					{
						if(what==2)what=0;
						else what=2;

						break;
					}
				}
				else if(y<16)
				{
					if(x<16) //showconsole^=1;		//左上
					{
						if(what==3)what=0;
						else what=3;

						break;
					}
					else if(x>1024-16) return;				//右上
				}

				//上面只判断了四个角落，是那里的话就不会运行到这里
				//这里不能else，否则上下两条的不会传给下面的函数
				if(what==0)disk_mouse(x,y);
				else if(what==1)real0mouse(x,y);
				else if(what==2)logic0mouse(x,y);

				break;
			}//case 2
		}//switch
	}//while(1)
}
