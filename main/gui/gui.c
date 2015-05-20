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
		printlog();
	}
	else
	{
		//
		printdisk();
	}
	//左上角
	for(x=0;x<16;x++)
		for(y=0;y<16;y++)
			point(x,y,0xffffffff);
	//右上角
	for(x=1024-16;x<1024;x++)
		for(y=0;y<16;y++)
			point(x,y,0xff0000);
	/*
	*/
}
void main()
{
	initmaster();

	initdiskman();
	initconsole();

	real0init();
	logic0init();


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

				//console在干活就交给console
				else if(what==3)loginput(key);

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
					if(x<16) //showconsole^=1;		//左上
					{
						if(what==3)what=0;
						else what=3;

						break;
					}
					else if(x>1024-16) return;				//右上
				}

				//
				if(what==0)disk_mouse(x,y);

				//real0在干活就交给real0
				else if(what==1)real0mouse(x,y);

				//logic0在干活就交给logic0
				else if(what==2)logic0mouse(x,y);

				//console在干活就交给console
				/*
				if( (y>320-128) && (y<320+128) )
				{
					if(x<64) //showreal^=1;				//左下
					{
						if(what==1)what=0;
						else what=1;

						break;
					}
					else if(x>1024-64)//showlogic^=1;		//右下
					{
						if(what==2)what=0;
						else what=2;

						break;
					}
				}
				*/

				break;
			}//case 2
		}//switch
	}//while(1)
}
