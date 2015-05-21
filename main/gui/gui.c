#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//static char showclose=0;
//static char showconsole=0;
//static char showreal=0;
//static char showlogic=0;
static char what=0;
void killmehelpit(int killwho,int helpwho)
{
	what=helpwho;
}




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
		hex();
	}
	else if(what==4)
	{
		console();
	}
	else
	{
		//
		printdisk();
	}
	//左上角
	for(y=0;y<16;y++)
		for(x=0;x<16;x++)
			point(x,y,0xffffffff);
	//右上角
	for(y=0;y<16;y++)
		for(x=1024-16;x<1024;x++)
			point(x,y,0xff0000);
	//左下角
	for(y=640-16;y<640;y++)
		for(x=0;x<16;x++)
			point(x,y,0xff);
	//右下角
	for(y=640-16;y<640;y++)
		for(x=1024-16;x<1024;x++)
			point(x,y,0xff00);
}
void main()
{
	initmaster();

	diskinit();			//0
	real0init();		//1
	logic0init();		//2
	hexinit();			//3
	consoleinit();		//4


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
				say("keyboard:%x\n",key);

				//不管谁在干活，按下esc就显示主屏
				if(key==0x1b)what=0;

				//real0在干活就交给real0
				if(what==1)real0kbd(key);

				//logic0在干活就交给logic0
				else if(what==2)logic0kbd(key);

				//hex在干活就交给hex
				else if(what==3)hexkbd(key);

				//console在干活就交给console
				else if(what==4)consolekbd(key);

				break;
			}
			case 2:				//鼠标
			{
				int x=key&0xffff;
				int y=(key>>16)&0xffff;
				say("mouse:(%d,%d)\n",x,y);

				//四个角落
				if(y<16)
				{
					if(x>1024-16) return;				//右上
				}
				if(y>640-16)
				{
					if(x<16) //showconsole^=1;		//左上
					{
						if(what==3)what=0;
						else what=3;

						break;
					}
					else if(x>1024-16)
					{
						if(what==4)what=0;
						else what=4;

						break;
					}
				}

				//主界面就交给主界面
				if(what==0)disk_mouse(x,y);

				//real0在干活就交给real0
				else if(what==1)real0mouse(x,y);

				//logic0在干活就交给logic0
				else if(what==2)logic0mouse(x,y);

				//logic0在干活就交给logic0
				else if(what==3)hexmouse(x,y);

				//console在干活就交给console
				//else if(what==4)consolemouse(x,y);

				break;
			}//case 2
		}//switch
	}//while(1)
}
