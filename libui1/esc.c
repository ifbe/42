#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//硬盘信息
static QWORD diskinfo;

//
static QWORD choose=0;




void f1init();
void menubg();
void point(int x,int y,int color);
void string(int x,int y,char* str);
void hidemenu();
void choosetarget(QWORD in);
void die();
QWORD whereisworld();




/*
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
		string(0x20,8+i,(char*)diskinfo+0x100*i);
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
}*/




void exitbutton()
{
	//最后的退出按钮
	/*
	int x,y;
	for(y=512+32;y<512+64;y++)
	{
		for(x=512-64;x<512+64;x++)
		{
			point(x,y,0xff0000);
		}
	}
	*/
	string(0x3e,34,"exit");
	string(0x3c,35,"program");
}








void menushow()
{
	menubg();
	//printdisk0();
	//printprocess0();
	exitbutton();
}
void menumessage(QWORD type,QWORD key)
{
	if(type!=0x7466656c)return;		//不是鼠标

	int x=key&0xffff;
	int y=(key>>16)&0xffff;
	/*
	if(x>256)
	{
		if(x<768)
		{
			if(y>128)		//[80,128),[144,192),[208,256),[272,320)......
			{
				if(y<512)
				{
					choose=(y-128)/16;
					choosetarget(choose);
					f1init();

					return;
				}
			}
		}
	}*/
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








void menuinit(QWORD world)
{
	diskinfo=world+0x700000;
}