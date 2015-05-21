#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//log位置
static QWORD logbuf;
static int px=0,py=0;



void hexinit()
{
	whereislogbuf(&logbuf);
}
void hexkbd(key)
{
	if(key==0x25)	//left	0x4b
	{
		if(px>0)px--;
	}
	else if(key==0x27)	//right	0x4d
	{
		if(px<0x3f)px++;
	}
	else if(key==0x26)	//up	0x4b
	{
		if(py>0)py--;
	}
	else if(key==0x28)	//down	0x4d
	{
		if(py<39)py++;
	}
}
void hexmouse(int x,int y)
{
	px=x/(1024/0x40);
	py=y/(640/40);
}
void hexbg()
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
	for(y=16;y<640;y+=16)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0xff);
		}
	}
	for(x=16;x<1024;x+=16)
	{
		for(y=0;y<640;y++)
		{
			point(x,y,0xff);
		}
	}
	/*
	//左上
	for(y=0;y<32;y++)
		for(x=0;x<32-y;x++)
			point(x,y,0);
	//右上
	for(y=0;y<32;y++)
		for(x=1024-32+y;x<1024;x++)
			point(x,y,0);
	//左下
	for(x=0;x<32;x++)
		for(y=640-32+x;y<640;y++)
			point(x,y,0);
	//右下
	for(y=0;y<32;y++)
		for(x=1024-y;x<1024;x++)
			point(x,640-32+y,0);
	*/
}
void printhex0()
{
	DWORD value;
	int x,y;

	//第一个框框
	int thisx=px*16;
	int thisy=py*16;
	for(y=thisy;y<thisy+16;y++)
	{
		for(x=thisx;x<thisx+16;x++)
		{
			point(x,y,0xff);
		}
	}

	//写字
	for(y=0;y<40;y++)
	{
		for(x=0;x<0x40;x+=4)
		{
			value=*(DWORD*)(logbuf+y*0x40+x);
			hexadecimal1234(2*x,y,value);
		}
	}

	//详细
	if(thisx>768-16)thisx-=256+16;
	if(thisy>640-256-16)thisy-=256+16;
	for(y=thisy+16;y<thisy+256+16;y++)
	{
		for(x=thisx+16;x<thisx+256+16;x++)
		{
			point(x,y,0xffffffff);
		}
	}

	QWORD address=logbuf+py*0x40+px;
	DWORD data=*(DWORD*)((logbuf+py*0x40+px)&0xfffffffffffffffc);
	data=( data >> ( ( address & (QWORD)0x3 ) * 8) ) & 0xff;
	string(2+thisx/8,1+thisy/16,"address:");
	hexadecimal(2+16+thisx/8,1+thisy/16,address);
	string(2+thisx/8,2+thisy/16,"data:");
	hexadecimal(2+16+thisx/8,2+thisy/16,data);
}
void printhex1()
{
	
}
void printhex2()
{
	
}
void hex()
{
	hexbg();

	printhex0();
}