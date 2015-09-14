#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




QWORD whereisscreen();
void blackstring(int x,int y,char* str);




void backgroundx()
{
	int x,y;
	DWORD thatcolor=0;
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=thatcolor;
	}
/*
	//横
	for(y=16;y<640;y+=16)
	{
		for(x=0;x<1024;x++)
		{
			screenbuf[(y<<10) + x]=0x44;
		}
	}
	//竖
	for(x=16;x<1024;x+=16)
	{
		for(y=0;y<640;y++)
		{
			screenbuf[(y<<10) + x]=0x4400;
		}
	}
*/
}





void background1()
{
	DWORD thatcolor=0xf0f0f0f0;
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=thatcolor;
	}

	//上下
	DWORD b=thatcolor&0xff;
	DWORD g=(thatcolor>>8)&0xff;
	DWORD r=(thatcolor>>16)&0xff;
	thatcolor=(b>>4)+( (g>>4) << 8 )+( (r>>4) << 16 );
	for(y=0;y<16;y++)
	{
		DWORD color=(QWORD)y*thatcolor;

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
		DWORD color=(QWORD)x*thatcolor;

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

	//[0,128]
	for(y=0;y<128;y++)
	{
		for(x=0;x<512;x++)
		{
			screenbuf[y*1024+x]=0xffff00;
		}
	}
	blackstring(0,0,"data");

	//[128,256]
	for(y=128;y<256;y++)
	{
		for(x=0;x<512;x++)
		{
			screenbuf[y*1024+x]=0xff00ff;
		}
	}
	blackstring(0,8,"dir");

	//[256,384]
	for(y=256;y<384;y++)
	{
		for(x=0;x<512;x++)
		{
			screenbuf[y*1024+x]=0xffff;
		}
	}
	blackstring(0,16,"fs");

	//[384,512]
	for(y=384;y<512;y++)
	{
		for(x=0;x<512;x++)
		{
			screenbuf[y*1024+x]=0xffffffff;
		}
	}
	blackstring(0,24,"disk");

	//[512,640]
	for(y=512;y<640;y++)
	{
		for(x=0;x<512;x++)
		{
			screenbuf[y*1024+x]=0xfedcba98;
		}
	}
	blackstring(0,32,"choose");

	//[0,512]
	for(y=0;y<640;y++)
	{
		for(x=512;x<1024;x++)
		{
			screenbuf[y*1024+x]=0xffffffff;
		}
	}

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








void background4()
{
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=0;
	}
}
