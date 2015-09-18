#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




QWORD whereisscreen();
void blackstring(int x,int y,char* str);
void line(int,int,int,int);
void rectangle(int,int,int,int);




void menubg()
{
	//背景
	int x,y;
	DWORD* screenbuf=(DWORD*)whereisscreen();

	for(y=64;y<640-64;y++)
	{
		for(x=256;x<768;x++)
		{
			screenbuf[(y*1024)+x]=0x44444444;
		}
	}
	for(y=64;y<640-64;y++)
	{
		screenbuf[(y*1024)+256]=0;
		screenbuf[(y*1024)+767]=0;
	}
	for(x=256;x<768;x++)
	{
		screenbuf[(64*1024)+x]=0;
		screenbuf[((639-64)*1024)+x]=0;
	}

}




void background1()
{
	DWORD thatcolor=0xf0f0f0f0;
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	QWORD x,y;
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=0xf0f0f0f0;
	}

	//上下
	//DWORD b=thatcolor&0xff;
	//DWORD g=(thatcolor>>8)&0xff;
	//DWORD r=(thatcolor>>16)&0xff;
	//thatcolor=(b>>4)+( (g>>4) << 8 )+( (r>>4) << 16 );
	for(y=0;y<16;y++)
	{
		DWORD color=0x40404040+(0x0b0b0b0b*y);

		//上，编译器会不会用rep stosd指令优化呢?
		DWORD* p=screenbuf+y*1024;
		for(x=y;x<1024-y;x++)p[x]=color;

		//下
		p=screenbuf+(639-y)*1024;
		for(x=y;x<1024-y;x++)p[x]=color;
	}
	//左右
	for(x=0;x<16;x++)
	{
		DWORD color=0x40404040+(0x0b0b0b0b*x);

		for(y=x;y<640-x;y++)
		{
			screenbuf[(y*1024)+x]=color;
			screenbuf[(y*1024)+1023-x]=color;
		}
	}
}






void background2()
{
	int x,y;
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//bg
	for(y=0;y<640*1024;y++)
	{
		screenbuf[y]=0xffffffff;
	}

	//[0,128]
	for(y=0;y<128;y++)
	{
		for(x=384;x<640;x++)
		{
			screenbuf[y*1024+x]=0xffff;
		}
	}
	blackstring(0x3c,0,"data");

	//[128,256]
	for(y=128;y<256;y++)
	{
		for(x=384;x<640;x++)
		{
			screenbuf[y*1024+x]=0x8a67f7;
		}
	}
	blackstring(0x3c,8,"dir");

	//[256,384]
	for(y=256;y<384;y++)
	{
		for(x=384;x<640;x++)
		{
			screenbuf[y*1024+x]=0xe578bd;
		}
	}
	blackstring(0x3c,16,"fs");

	//[384,512]
	for(y=384;y<512;y++)
	{
		for(x=384;x<640;x++)
		{
			screenbuf[y*1024+x]=0xff00;
		}
	}
	blackstring(0x3c,24,"disk");

	//[512,640]
	for(y=512;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			screenbuf[y*1024+x]=0xfedcba98;
		}
	}
	blackstring(0x3c,32,"choose");

	rectangle(932,234,27,99);
	line(35,67,944,285);
/*
	for(y=0;y<16;y++)	//上下
	{
		DWORD color=(QWORD)y*thatcolor;

		//上，编译器会不会用rep stosd指令优化呢?
		for(x=y;x<1024-y;x++)screenbuf[(y<<10) + x]=color;

		//下
		for(x=y;x<1024-y;x++)screenbuf[( (639-y) << 10 ) + x]=color;
	}
	for(x=0;x<16;x++)	//左右
	{
		DWORD color=(QWORD)x*thatcolor;

		for(y=x;y<640-x;y++)
		{
			screenbuf[(y*1024)+x]=color;
			screenbuf[(y*1024)+1023-x]=color;
		}
	}
*/
}







void background3()
{
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=0x456789ab;
	}
}








void background4(QWORD showaddr,QWORD temp)
{
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*624;x++)
	{
		screenbuf[x]=0;
	}

	for(y=624;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			screenbuf[(y*1024) + x]=(x/4)*0x01010101;
		}
	}

	for(y=0;y<640;y++)
	{
		for(x=1024-16;x<1024;x++)
		{
			screenbuf[(y*1024) + x] = 0xffffff00+(y*0xff/640);//0x44444488;
		}
	}

	//位置
	if(temp>=0x80*40)
	{
		QWORD end=64+(640-64*2)*showaddr/temp;			//temp变量=max
		QWORD start=end-(640-64*2)*0x80*40/temp;
		for(y=start;y<end;y++)
		{
			for(x=1024-32+4;x<1024-16-4;x++)
			{
				screenbuf[(y*1024) + x] = 0xccccccff;
			}
		}
	}
}
