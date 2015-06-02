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
		case 0:
		{
			overview();
			break;
		}
		case 1:
		{
			printdisk();
			break;
		}
		case 2:
		{
			real0();
			break;
		}
		case 3:
		{
			logic0();
			break;
		}
		case 4:
		{
			hex();
			break;
		}
		case 5:
		{
			console();
			break;
		}
	}

	//右上角
	for(x=1024-16;x<1024;x++)
	{
		for(y=0;y<16;y++)
			point(x,y,0xffff00);
		anscii(0x7f,0,'0');

		for(y=16;y<32;y++)
			point(x,y,0xff);
		anscii(0x7f,1,'1');

		for(y=32;y<48;y++)
			point(x,y,0xff00);
		anscii(0x7f,2,'2');

		for(y=48;y<64;y++)
			point(x,y,0xff0000);
		anscii(0x7f,3,'3');

		for(y=64;y<80;y++)
			point(x,y,0xff00ff);
		anscii(0x7f,4,'4');

		for(y=80;y<96;y++)
			point(x,y,0xffff);
		anscii(0x7f,5,'5');
	}
}
void main()
{
	initmaster();

						//0
	diskinit();			//1
	real0init();		//2
	logic0init();		//3
	hexinit();			//4
	consoleinit();		//5


	while(1)
	{
		//1.这次显示啥
		printworld();			//内容
		writescreen();		//写屏

		//2.等事件
		DWORD type=0;
		DWORD key=0;
		waitevent(&type,&key);
		say("type=%x,key=%x\n",type,key);
		if(type==0)return;

		//3.干啥事，首先处理掉特殊一些的消息
		int x=key&0xffff;			//四个角落
		int y=(key>>16)&0xffff;
		if(type==1)
		{
			//按下esc
			if(key==0x1b)
			{
				if(what!=0)chooseoperator(0);
				else chooseoperator(0x80);
				continue;
			}
		}
		if(type==2)
		{
			if(x>1024-16)
			{
				if(y<16)				//右上
				{
					chooseoperator(0);
					continue;
				}
				else if(y<32)
				{
					chooseoperator(1);
					continue;
				}
				else if(y<48)
				{
					chooseoperator(2);
					continue;
				}
				else if(y<64) //showconsole^=1;		//左上
				{
					chooseoperator(3);
					continue;
				}
				else if(y<80)
				{
					chooseoperator(4);
					continue;
				}
				else if(y<96)
				{
					chooseoperator(5);
					continue;
				}
			}
		}

		//其余所有消息，谁在干活就交给谁
		if(what==0)overviewmessage(type,key);		//点了叉
		else if(what==1)diskmessage(type,key);		//磁盘
		else if(what==2)real0message(type,key);		//real0在干活就交给real0
		else if(what==3)logic0message(type,key);		//logic0在干活就交给logic0
		else if(what==4)hexmessage(type,key);		//hex在干活就交给hex
		else if(what==5)consolemessage(type,key);		//console在干活就交给console

		if( (what&0xff) == 0xff )return;		//要求自杀，就让它死
	}//while(1)
}
