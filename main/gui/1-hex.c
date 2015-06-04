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
	//dword框
	int x,y;			//x<0xff
	for(y=16*py;y<16*py+16;y++)
	{
		point(16*(px&0xfc),y,0xff);
		point(16*(px&0xfc)+1,y,0xff);
		point(16*(px&0xfc)+62,y,0xff);
		point(16*(px&0xfc)+63,y,0xff);
	}
	for(x=16*(px&0xfc);x<16*(px&0xfc)+64;x++)
	{
		point(x,16*py,0xff);
		point(x,16*py+1,0xff);
		point(x,16*py+0xf-1,0xff);
		point(x,16*py+0xf,0xff);
	}
	//word框
	for(y=16*py+2;y<16*py+14;y++)
	{
		point(16*(px&0xfe)+2,y,0xff00);
		point(16*(px&0xfe)+3,y,0xff00);
		point(16*(px&0xfe)+31-3,y,0xff00);
		point(16*(px&0xfe)+31-2,y,0xff00);
	}
	for(x=16*(px&0xfe)+2;x<16*(px&0xfe)+30;x++)
	{
		point(x,16*py+2,0xff00);
		point(x,16*py+3,0xff00);
		point(x,16*py+0xf-3,0xff00);
		point(x,16*py+0xf-2,0xff00);
	}
	//byte框
	for(y=16*py+4;y<16*py+12;y++)
	{
		point(16*px+4,y,0xff0000);
		point(16*px+5,y,0xff0000);
		point(16*px+15-5,y,0xff0000);
		point(16*px+15-4,y,0xff0000);
	}
	for(x=16*px+4;x<16*px+12;x++)
	{
		point(x,16*py+4,0xff0000);
		point(x,16*py+5,0xff0000);
		point(x,16*py+0xf-5,0xff0000);
		point(x,16*py+0xf-4,0xff0000);
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

	//数据
	QWORD thisaddr=(baseaddr+offset+py*0x40+px);
	QWORD position=thisaddr & 0x3;		//0,1,2,3

	DWORD data32=*(DWORD*)(thisaddr & 0xfffffffffffffffc);
	string(thisx/8,thisy/16,"data:");
	string(16+thisx/8,thisy/16,"   ,   ,   ,  ");
	hexadecimal(16+thisx/8,thisy/16,data32&0xff);
	hexadecimal(20+thisx/8,thisy/16,(data32>>8)&0xff);
	hexadecimal(24+thisx/8,thisy/16,(data32>>16)&0xff);
	hexadecimal(28+thisx/8,thisy/16,(data32>>24)&0xff);

	string(thisx/8,1+thisy/16,"dword:");
	hexadecimal(16+thisx/8,1+thisy/16,data32);

	WORD data16=( data32 >> ( (position/2) << 4 ) ) & 0xffff;
	string(thisx/8,2+thisy/16,"word:");
	hexadecimal(16+thisx/8,2+thisy/16,data16);

	BYTE data8=( data32 >> (position*8) ) & 0xff;
	string(thisx/8,3+thisy/16,"byte:");
	hexadecimal(16+thisx/8,3+thisy/16,data8);

	string(thisx/8,4+thisy/16,"ansciistring:");
	string(16+thisx/8,4+thisy/16,thisaddr);

	//横线
	for(x=0;x<256;x++)point(thisx+x,thisy+128,0);

	//地址，偏移，y，x
	string(thisx/8,8+thisy/16,"base:");
	hexadecimal(16+thisx/8,8+thisy/16,baseaddr);

	string(thisx/8,9+thisy/16,"offset:");
	anscii(15+thisx/8,9+thisy/16,'+');
	hexadecimal(16+thisx/8,9+thisy/16,offset);

	string(thisx/8,10+thisy/16,"y:");
	anscii(15+thisx/8,10+thisy/16,'+');
	hexadecimal(16+thisx/8,10+thisy/16,py);
	string(18+thisx/8,10+thisy/16,"*40");
	string(26+thisx/8,10+thisy/16,"(    )");
	hexadecimal(27+thisx/8,10+thisy/16,0x40*py);

	string(thisx/8,11+thisy/16,"x:");
	anscii(15+thisx/8,11+thisy/16,'+');
	hexadecimal(16+thisx/8,11+thisy/16,px);

	for(x=thisx+128;x<thisx+256;x++)
	{
		point(x,thisy+192,0);
	}
	anscii(15+thisx/8,12+thisy/16,'=');
	hexadecimal(16+thisx/8,12+thisy/16,baseaddr+offset+py*0x40+px);

	//手工输入区域
	string(16+thisx/8,15+thisy/16,buffer);

}
void printhex1()
{
	
}
void printhex2()
{
	
}
void hex()
{
	background1();

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
			else{
				if(offset>=0xa00)offset-=0xa00;
			}
		}
		else if(key==0x27)	//right	0x4d
		{
			if(px<0x3f)px++;
			else offset+=0xa00;
		}
		else if(key==0x26)	//up	0x4b
		{
			if(py>0)py--;
			else{
				if(offset>=0x40)offset-=0x40;
			}
		}
		else if(key==0x28)	//down	0x4d
		{
			if(py<39)py++;
			else offset+=0x40;
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

		//（特殊的）点了方向箭头
		//if(offset>=0x40)offset-=0x40;		//上
		//offset+=0x40;						//下
		//if(offset>=0xa00)offset-=0xa00;		//左
		//offset+=0xa00;						//右

		//（普通的）选择了某个字节
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