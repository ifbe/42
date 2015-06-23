#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//硬盘信息
static BYTE* diskinfo;

//
static QWORD choose=0;




void whereisdiskinfo(BYTE** in);
void point(int x,int y,int color);
void string(int x,int y,char* str);
void hidemenu();
void focus(QWORD in);
void die();
void hexinit();




void menuinit()
{
	whereisdiskinfo(&diskinfo);
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
	for(i=0;i<24;i++)
	{
		if(*(DWORD*)(diskinfo+0x100*i) == 0)break;

		for(y=127+16*i;y<143+16*i;y++)
		{
			for(x=256;x<768;x++)
			{
				point(x,y,0x88888888);
			}
		}
		string(0x20,8+i,diskinfo+0x100*i);
		//string(0x40,8+i,diskinfo+0x100*i+0x80);
	}
	//选中
	for(y=127+16*choose;y<143+16*choose;y+=2)
	{
		for(x=256;x<768;x+=2)
		{
			point(x,y,0);
		}
	}
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
	//printprocess0();
	exitbutton();
}
void menumessage(QWORD type,QWORD key)
{
	if(type!=0x7466656c6563696d)return;		//不是鼠标

	int x=key&0xffff;
	int y=(key>>16)&0xffff;
	if(x>256)
	{
		if(x<768)
		{
			if(y>128)		//[80,128),[144,192),[208,256),[272,320)......
			{
				if(y<512)
				{
					choose=(y-128)/16;
					focus(choose);
					hexinit();

					return;
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
					return;
				}
			}
		}
	}
	hidemenu();
	return;
}
