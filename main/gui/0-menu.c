#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//硬盘信息
static BYTE* diskinfo;
static BYTE* processinfo;

//
static int choose=0;




void diskinit()
{
	whereisdiskinfo(&diskinfo);
	whereisprocessinfo(&processinfo);
}
void menubg()
{
	//背景
	int x,y;

	for(y=64;y<640-64;y++)
	{
		for(x=256;x<768;x++)
		{
			point(x,y,0xffffff);
		}
	}
	for(y=64;y<640-64;y++)
	{
		point(256,y,0);
		point(767,y,0);
	}
	for(x=256;x<768;x++)
	{
		point(x,64,0);
		point(x,639-64,0);
	}

}


void printdisk0()
{
	//内容
	int x,y,i;
	for(y=64;y<80;y++)
	{
		for(x=256;x<768;x++)
		{
			point(x,y,0xff);
		}
	}
	string(0x20,4,"----process----");

	//
	for(i=0;i<10;i++)
	{
		if(*(DWORD*)(diskinfo+0x100*i) == 0)break;

		for(y=65+16*i;y<79+16*i;y++)
		{
			for(x=256+64;x<768-64;x++)
			{
				point(x,y,0x88888888);
			}
		}
		string(0x28,4+i,diskinfo+0x100*i);
		//string(0x40,8+i,diskinfo+0x100*i+0x80);
	}
	//选中
	for(y=65+16*choose;y<79+16*choose;y+=2)
	{
		for(x=256;x<768;x+=2)
		{
			point(x,y,0);
		}
	}

	//手工输入
	string(0x20,19,diskinfo);

}
void printprocess0()
{
	//中间一条横线
	int x,y,i;
	for(y=320;y<336;y++)
	{
		for(x=256;x<768;x++)
		{
			point(x,y,0xff);
		}
	}
	string(0x20,20,"----process----");

	//
	for(i=0;i<10;i++)
	{
		if(*(DWORD*)(processinfo+0x80*i) == 0)break;

		for(y=321+16*i;y<335+16*i;y++)
		{
			for(x=256+64;x<768-64;x++)
			{
				point(x,y,0x88888888);
			}
		}
		string(0x28,20+i,processinfo+0x80*i);
	}
	//选中
	/*
	for(y=321+16*choose;y<335+16*choose;y+=2)
	{
		for(x=256;x<768;x+=2)
		{
			point(x,y,0);
		}
	}
	*/
}
void exitbutton()
{
	//最后的退出按钮
	int x,y;
	for(y=512+32;y<512+64;y++)
	{
		for(x=512-64;x<512+64;x++)
		{
			point(x,y,0xff0000);
		}
	}
	string(0x3e,34,"exit");
	string(0x3c,35,"program");
}
void menu()
{
	menubg();
	printdisk0();
	printprocess0();
	exitbutton();
}
menumessage(DWORD type,DWORD key)
{
	if(type!=2)return;

	int x=key&0xffff;
	int y=(key>>16)&0xffff;
	if(x>256)
	{
		if(x<768)
		{
			if(y>64)		//[80,128),[144,192),[208,256),[272,320)......
			{
				if(y<320)
				{
					//int val=(y-128)/16;
					//if( ( (val%4) == 1 ) | ( (val%4) == 2 ) )
					//{
					//	choose=val/4;
						choose=(y-64)/16;
						char* arg0="disk";
						DWORD arg1=0x30+choose;
						realcommand(arg0,&arg1);
					//}
				}
			}
		}
	}
	if(y>512+32)
	{
		if(y<512+64)
		{
			if(x>512-64)
			{
				if(x<512+64)
				{
					die();
				}
			}
		}
	}
}