#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

static int tag=0;			//主体显示啥




void background()
{
	QWORD x,y;

	//清屏
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0x88888888);
		}
	}

	//[608,639]:低栏颜色与低栏分界线
	for(y=640-16;y<640;y++)
		for(x=256;x<768;x++)
			point(x,y,0xffffffff);
	for(y=640-16;y<640;y++)
		for(x=256;x<768;x+=64)
		point(x,y,0);

	//+涂黑选中项
	for(y=0;y<640-32;y++)
		for(x=0;x<1024;x++)
			point(x,y,0xcccccccc);
	for(y=640-32;y<640;y++)
		for(x=64*tag+256;x<64*tag+320;x++)
			point(x,y,0xcccccccc);

	//+写标签名
	string(32,39,"/part");
	string(40,39,"/file");

}
void content()
{
	//
	if(tag==0)
	{
		printpartition();
	}
	else if(tag==1)
	{
		printfile();
	}

	//
	printlog();
	printdisk();
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
void main()
{
	QWORD realworld;
	QWORD logicworld;
	whereisrealworld(&realworld);
	whereislogicworld(&logicworld);

	initreal0(realworld+0x10000);
	initlogic0(logicworld+0x100000);
	initdiskman();
	initconsole();
	initmaster();

	while(1)
	{
		//1.这次显示啥
		background();		//背景
		content();			//内容
		foreground();		//四点
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

				if(key==0x25)	//left	0x4b
				{
					if(tag>0)tag--;
				}
				else if(key==0x27)	//right	0x4d
				{
					if(tag<7)tag++;
				}
				else
				{
					if(tag==0)loginput(key);
				}

				break;
			}
			case 2:				//鼠标
			{
				int x=key&0xffff;
				int y=(key>>16)&0xffff;

				//点了右上角退出
				if( (x>1024-16) && (y<16) )return;
				say("mouse:(%d,%d)\n",x,y);

				//最下面两行，控制面板
				if(y>640-16)
				{
					if(x<16) touchconsole();
					else if(x>1024-16) touchdisk();
					else if( (x>256) && (x<768) )
					{
						tag=x/64-4;
					}
				}

				//其他的前面的几十行
				else
				{
					if(tag==1) mouseinputforpartition(x,y);
					else if(tag==2) mouseinputforfile(x,y);
					else if(tag==15) mouseinputfordisk(x,y);
				}

				break;
			}
		}//switch
	}//while(1)
}
