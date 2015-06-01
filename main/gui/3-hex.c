#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//log位置
static QWORD baseaddr;
static QWORD offset;

//位置
static int px=0;
static int py=0;

//键盘输入
static BYTE buffer[128];//键盘输入专用
static bufcount=0;


void hexinit()
{
	//whereisrealworld(&baseaddr);
	baseaddr=0x400000;
	baseaddr&=0xfffffffffffffffc;
	offset=0;
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
	//写字
	int x,y;
	DWORD value;
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
	for(y=0;y<40;y++)
	{
		for(x=0;x<0x40;x+=4)
		{
			value=*(DWORD*)(baseaddr+offset+y*0x40+x);
			hexadecimal1234(2*x,y,value);
		}
	}

}
void printfloat()
{
	//详细:选中的点
	int x,y;
	for(y=16*py;y<16*py+16;y++)
	{
		for(x=16*px;x<16*px+16;x++)
		{
			point(x,y,0xff);
		}
	}
	//256*256的详情框
	int thisx=px*16+16;
	int thisy=py*16+16;
	if(thisx>768)thisx-=256+16;
	if(thisy>640-256)thisy-=256+16;
	for(y=thisy;y<thisy+256;y++)
	{
		for(x=thisx;x<thisx+256;x++)
		{
			point(x,y,0xffffffff);
		}
	}
	//详情内容文字
	string(thisx/8,thisy/16,"address:");
	string(thisx/8,1+thisy/16,"data:");
	string(thisx/8,8+thisy/16,"realworld:");
	string(thisx/8,9+thisy/16,"logicworld:");
	string(thisx/8,10+thisy/16,"1234:");
	string(thisx/8,11+thisy/16,"12351234:");

	QWORD address=baseaddr+offset+py*0x40+px;
	DWORD data=*(DWORD*)(address&0xfffffffffffffffc);
	data=( data >> ( ( address & (QWORD)0x3 ) * 8) ) & 0xff;
	hexadecimal(16+thisx/8,thisy/16,address);
	hexadecimal(16+thisx/8,1+thisy/16,data);
	string(16+thisx/8,11+thisy/16,buffer);
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

	printfloat();
}
void hexmessage(DWORD type,DWORD key)
{
	if(type==1)
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
		else if(key==0xd)
		{
			//
			//command(buffer);
			baseaddr=0x600000;
			offset=0;

			//清空
			int i;
			for(i=0;i<128;i++)
			{
				buffer[i]=0;
			}
			bufcount=0;
		}
		else if(key==0x8)		//backspace
		{
			if(bufcount!=0)
			{
				bufcount--;
				buffer[bufcount]=0;
			}
		}
		else
		{
			if(bufcount<0x80)
			{
				buffer[bufcount]=key&0xff;
				bufcount++;
			}
		}
	}
	else if(type==2)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		px=x/(1024/0x40);
		py=y/(640/40);
	}
	else if(type==3)
	{
		if(key<0xff0000)
		{
			if(offset>=0x40)offset-=0x40;
		}
		else if(key>0xff0000)
		{
			//if(offset<0x100000-0xa00)offset+=0x40;
			offset+=0x40;
		}
	}
}