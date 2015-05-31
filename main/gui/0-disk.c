#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//硬盘信息
static BYTE* diskinfo;

//
static int choose=0;
static int complex=0;		//主体华丽程度




void diskinit()
{
	whereisdiskinfo(&diskinfo);
}
disk_mouse(int x,int y)
{
	int val=0;

	if(y>320-128)
	{
		if(y<320+128)
		{
			if(x<32)
			{
				killmehelpit(0,1);
				return;
			}
			if(x>1024-32)
			{
				killmehelpit(0,2);
				return;
			}
		}
	}
	if(x>256)
	{
		if(x<768)
		{
			if(y>64)		//[80,128),[144,192),[208,256),[272,320)......
			{
				if(y<512)
				{
					val=(y-64)/16;
					if( (val%4) != 0 )		//val=4n不行，val=4n+1,4n+2,4n+3都可以
					{
						choose=val/4;

						char* arg0="disk";
						DWORD arg1=0x30+choose;
						realcommand(arg0,&arg1);
					}
				}
			}
		}
	}
}
disk_kbd()
{
}
disk_bg()
{
	int x,y;
	unsigned int color,i=0;
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0xe0e0e0);
		}
	}
	//上下
	for(y=0;y<16;y++)
	{
		color=(QWORD)y*0x0e0e0e;

		for(x=y;x<1024-y;x++)
		{
			point(x,y,color);
			point(x,639-y,color);
		}
	}
	//左右
	for(x=0;x<16;x++)
	{
		color=(QWORD)x*0x0e0e0e;

		for(y=x;y<640-x;y++)
		{
			point(x,y,color);
			point(1023-x,y,color);
		}
	}
	/*
	int x,y,i;
	DWORD color;
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0);
		}
	}
	for(y=0;y<640;y++)
	{
		for(x=y-576;x<=y+960;x+=64)
		{
			if( (x>=0) && (x<1024) )
			{
				point(x,y,0xffffffff);
				point(x,639-y,0xffffffff);
			}
		}
	}
	for(x=0;x<256;x++)
	{
		color=0xff00ff-0x010001*x;
		for(y=64;y<576;y++)		//左
		{
			point(x,y,color);
		}
	}
	for(x=0;x<256;x++)
	{
		color=0xffff00-0x010100*x;
		for(y=64;y<576;y++)		//右
		{
			point(1023-x,y,color);
		}
	}
	*/

	//背景色
	/*
	for(y=0;y<256;y++)		//上
	{
		color=0x010101*(y*3/4);
		for(x=0;x<1024;x++)
		{
			point(x,y,color);
		}
	}
	for(y=0;y<256;y++)		//下
	{
		color=0x010101*(y*3/4);
		for(x=0;x<1024;x++)
		{
			point(x,639-y,color);
		}
	}*/
	/*
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0);
		}
	}
	for(x=0;x<256;x++)		//左
	{
		color=0xc0+0x010100*(x*3/4);

		for(y=x;y<640-x;y++)
		{
			point(x,y,color);
		}
	}
	for(x=0;x<256;x++)		//右
	{
		color=0xc000+0x010001*(x*3/4);

		for(y=x;y<640-x;y++)
		{
			point(1023-x,y,color);
		}
	}
	for(y=256;y<320;y++)		//横
	{
		color=0x010101*( (y-256)*0xff/(320-256) );
		for(x=256;x<1024-256;x++)
		{
			point(x,y,color);
			point(x,639-y,color);
		}
	}*/
	/*
	for(x=0;x<64;x++)		//竖
	{
		color=0x040404*x;
		for(y=64;y<256;y++)
		{
			point(512-64+x,y,color);
			point(511+64-x,y,color);
		}
		for(y=320+64;y<640-64;y++)
		{
			point(512-64+x,y,color);
			point(511+64-x,y,color);
		}
	}
	for(x=0;x<64;x++)		//竖
	{
		color=0x040404*x;
		for(y=256;y<320-64+x;y++)
		{
			point(512-64+x,y,color);
			point(511+64-x,y,color);
		}
		for(y=320+64-x;y<640-256;y++)
		{
			point(512-64+x,y,color);
			point(511+64-x,y,color);
		}
	}
	*/

	/*
	//上下
	for(y=0;y<16;y++)
	{
		color=0x0c0c0c*y;

		for(x=y;x<1024-y;x++)
		{
			point(x,y,color);
			point(x,639-y,color);
		}
	}
	//左右
	for(x=0;x<16;x++)
	{
		color=0x0c0c0c*x;

		for(y=x;y<640-x;y++)
		{
			point(x,y,color);
			point(1023-x,y,color);
		}
	}
	*/
	/*
	for(y=0;y<640;y++)
	{
		for(x=0;x<512;x++)
		{
			point(x,y,0x7f+0x010100*(x/4));
		}
	}
	for(y=0;y<640;y++)
	{
		for(x=512;x<1024;x++)
		{
			point(x,y,0x7f00+0x010001*((1023-x)/4));
		}
	}
	*/
	/*
	for(x=192;x<832;x++)
	{
		point(x,0,0xffffffff);
		point(x,639,0xffffffff);
	}
	for(y=0;y<640;y++)
	{
		point(192,y,0xffffffff);
		point(831,y,0xffffffff);
	}
	*/

	/*
	//左上角
	for(x=16;x<80;x++)
		for(y=0;y<16;y++)
			point(x,y,0x77777777);
	string(2,0,"");
	//右上角
	for(x=1024-80;x<1024-16;x++)
		for(y=0;y<16;y++)
			point(x,y,0x770000);
	string(0x78,0,"close");
	//左下角
	for(x=16;x<80;x++)
		for(y=640-16;y<640;y++)
			point(x,y,0x77);
	string(2,39,"hex");
	//右下角
	for(x=1024-80;x<1024-16;x++)
		for(y=640-16;y<640;y++)
			point(x,y,0x7700);
	string(0x78,39,"console");
	*/
	//左箭头
	//for(x=16;x<80;x++)
		//for(y=640-16;y<640;y++)
			//point(x,y,0x77);
	for(x=0;x<32;x++)
		for(y=320-4*x;y<320+4*x;y++)
			point(x,y,0xff00ff);
	string(0,20,"real");
	//右箭头
	//for(x=1024-80;x<1024-16;x++)
		//for(y=640-16;y<640;y++)
			//point(x,y,0x7700);
	for(x=1024-32;x<1024;x++)
		for(y=320-(1024-x)*4;y<320+(1024-x)*4;y++)
			point(x,y,0xffff00);
	string(0x78,20,"logical");
}
void printdisk0()
{
	//背景
	char* p=(char*)diskinfo;
	int x,y,i;
	string(0x10,10,"i can recognize these disks");
	string(0x10,33,"u can choose a specific one");

	//内容
	for(i=0;i<10;i++)
	{
		if(*(DWORD*)(diskinfo+0x100*i) == 0)break;

		for(y=80+64*i;y<128+64*i;y++)
		{
			for(x=256;x<768;x++)
			{
				point(x,y,0xffffffff);
			}
		}
		string(0x20,6+4*i,diskinfo+0x100*i);
		string(0x20,7+4*i,diskinfo+0x100*i+0x80);
	}
	//选中
	for(y=80+64*choose;y<128+64*choose;y+=2)
	{
		for(x=256;x<768;x+=2)
		{
			point(x,y,0);
		}
	}

	//手工输入
	for(y=512;y<512+48;y++)
	{
		for(x=256;x<768;x++)
		{
			point(x,y,0xffffffff);
		}
	}
	string(0x20,32+2,diskinfo);

}
void printdisk1()
{
}
void printdisk2()
{
	
}
void printdisk()
{
	disk_bg();

	if(complex==0)
	{
		printdisk0();
	}
	else if(complex==1)
	{
		printdisk1();
	}
	else
	{
		printdisk2();
	}
}