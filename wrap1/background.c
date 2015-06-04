#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




void background1()
{
	DWORD thatcolor=0xe0e0e0;
	DWORD* screenbuf;
	whereisscreenbuf(&screenbuf);

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=thatcolor;
	}

	//上下
	thatcolor=0xfdfdfd;		//这个颜色挺好看的
	//say("thatcolor:%x\n",thatcolor);
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
	DWORD thatcolor=0xe0e0e0;
	DWORD* screenbuf;
	whereisscreenbuf(&screenbuf);

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