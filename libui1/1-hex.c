#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//键盘输入
static BYTE buffer[128];//键盘输入专用
static int bufcount=0;

//显示区域是0x1000，那么缓冲区就要0x2000以上
static BYTE databuffer[0x2000];

//
static int printmethod=0;

//浮动窗口位置
static int thisx;
static int thisy;

//位置
static QWORD baseaddr;
static QWORD offset;
static int py=0;
static int px=0;


void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void background1();
void point(int x,int y,DWORD color);
void anscii(int x,int y,char ch);
void string(int x,int y,char* str);
void hexadecimal(int x,int y,QWORD in);
void hexadecimal1234(int x,int y,QWORD in);




//
static QWORD currentcache;
QWORD readornotread(QWORD wantaddr)
{
	//假如每次能显示0x1000(实际是0xa00)
	//想要[0,0x1000)：			确保[0,0x2000)			返回databuffer+0
	//想要[0xfc0,0x1fc0)：		确保[0,0x2000)			返回databuffer+0xfc0
	//想要[0x1000,0x2000)：		确保[0x1000,0x3000)		返回databuffer+0
	//想要[0x1040,0x2040)：		确保[0x1000,0x3000)		返回databuffer+0x40
	//想要[0x1fc0,0x2fc0)：		确保[0x1000,0x3000)		返回databuffer+0xfc0
	//想要[0x2000,0x3000)：		确保[0x2000,0x4000)		返回databuffer+0
	//想要[0x2040,0x3040)：		确保[0x2000,0x4000)		返回databuffer+0x40
	//想要[0x2fc0,0x3fc0)：		确保[0x2000,0x4000)		返回databuffer+0xfc0
	//想要[0x3000,0x4000)：		确保[0x3000,0x5000)		返回databuffer+0
	QWORD roundnum=wantaddr & 0xfffffffffffff000;
	if(roundnum!=currentcache)
	{
		readmemory((QWORD)databuffer, roundnum/0x200, 0, 16);
		currentcache=roundnum;
	}

	//已经在里面，直接拿出来
	return (QWORD)databuffer+(wantaddr%0x1000);
}




void hexinit()
{
	//whereisrealworld(&baseaddr);
	int i;
	for(i=0;i<0x2000;i++)databuffer[i]=0;

	baseaddr=0;
	offset=0;
	currentcache=0xffffffff;
}
void hexbg()
{
	//横
	int x,y;
	background1();
	for(y=16;y<640;y+=16)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0xff);
		}
	}
	//竖
	for(x=16;x<1024;x+=16)
	{
		for(y=0;y<640;y++)
		{
			point(x,y,0xff);
		}
	}
	//dword框
	for(y=16*py;y<16*py+16;y++)
	{
		for(x=16*(px&0xfc);x<16*(px&0xfc)+64;x++)
		{
			point(x,y,0xff00);
		}
	}
	//byte框
	for(y=16*py+1;y<16*py+14;y++)
	{
		for(x=16*px+1;x<16*px+14;x++)
		{
			point(x,y,0xff0000);
		}
	}
}
void printhex0()
{
	//把想要的部分读进来
	QWORD readwhere=readornotread(baseaddr+offset);
	int x,y;

	//一整页
	if(printmethod==0)
	{
		for(y=0;y<40;y++)
		{
			for(x=0;x<0x40;x+=4)
			{
				DWORD value=*(DWORD*)(readwhere+y*0x40+x);
				hexadecimal1234(2*x,y,value);
			}
		}
	}
	else if(printmethod==1)
	{
		for(y=0;y<40;y++)
		{
			for(x=0;x<0x40;x+=4)
			{
				DWORD value=*(DWORD*)(readwhere+y*0x40+x);
				anscii(2*x,y,value&0xff);
				anscii(2*x+2,y,(value>>8)&0xff);
				anscii(2*x+4,y,(value>>16)&0xff);
				anscii(2*x+6,y,(value>>24)&0xff);
			}
		}
	}
	else if(printmethod==2)
	{
		
	}
}
void kuangbg()
{
	//256*256的详情框
	int x,y;
	for(y=thisy;y<thisy+256;y++)
	{
		for(x=thisx;x<thisx+256;x++)
		{
			point(x,y,0xffffffff);
		}
	}
	for(y=thisy+128;y<thisy+192;y++)
	{
		for(x=thisx;x<thisx+128;x++)
		{
			point(x,y,0xff);
		}
	}
	for(y=thisy+128;y<thisy+192;y++)
	{
		for(x=thisx+128;x<thisx+256;x++)
		{
			point(x,y,0xff00);
		}
	}
	for(y=thisy+192;y<thisy+256;y++)
	{
		for(x=thisx;x<thisx+128;x++)
		{
			point(x,y,0xff0000);
		}
	}
	for(y=thisy+192;y<thisy+256;y++)
	{
		for(x=thisx+128;x<thisx+256;x++)
		{
			point(x,y,0xffffff);
		}
	}
	//横线
	for(x=0;x<256;x++)point(thisx+x,thisy+128,0);
	//横线
	for(x=thisx+128;x<thisx+256;x++)
	{
		point(x,thisy+192,0);
	}

	//四个功能
	string(thisx/8,8+thisy/16,"hex");
	string(16+thisx/8,8+thisy/16,"anscii");
	string(thisx/8,0xc+thisy/16,"utf8");
	string(16+thisx/8,0xc+thisy/16,"reserved");
}
void kuangcontent()
{
	/*
	QWORD thisaddr=readwhere+py*0x40+px;
	QWORD position=thisaddr & 0x3;		//0,1,2,3

	DWORD data32=*(DWORD*)(thisaddr & 0xfffffffffffffffc);
	WORD data16=( data32 >> ( (position/2) << 4 ) ) & 0xffff;
	BYTE data8=( data32 >> (position*8) ) & 0xff;
	*/
	//baseaddr
	string(thisx/8,thisy/16,"base:");
	hexadecimal(16+thisx/8,thisy/16,baseaddr);
	//offset
	string(thisx/8,1+thisy/16,"offset:");
	anscii(15+thisx/8,1+thisy/16,'+');
	hexadecimal(16+thisx/8,1+thisy/16,offset);
	//y
	string(thisx/8,2+thisy/16,"y:");
	anscii(15+thisx/8,2+thisy/16,'+');
	hexadecimal(16+thisx/8,2+thisy/16,py);
	string(18+thisx/8,2+thisy/16,"*40");
	string(26+thisx/8,2+thisy/16,"(    )");
	hexadecimal(27+thisx/8,2+thisy/16,0x40*py);
	//x
	string(thisx/8,3+thisy/16,"x:");
	anscii(15+thisx/8,3+thisy/16,'+');
	hexadecimal(16+thisx/8,3+thisy/16,px);
	//baseaddr+offset+y*0x40+x
	anscii(15+thisx/8,4+thisy/16,'=');
	hexadecimal(16+thisx/8,4+thisy/16,baseaddr+offset+py*0x40+px);

	/*
	//hex
	string(thisx/8,thisy/16,"data:");
	string(16+thisx/8,thisy/16,"   ,   ,   ,  ");
	hexadecimal(16+thisx/8,thisy/16,data32&0xff);
	hexadecimal(20+thisx/8,thisy/16,(data32>>8)&0xff);
	hexadecimal(24+thisx/8,thisy/16,(data32>>16)&0xff);
	hexadecimal(28+thisx/8,thisy/16,(data32>>24)&0xff);
	//dword
	string(thisx/8,1+thisy/16,"dword:");
	hexadecimal(16+thisx/8,1+thisy/16,data32);
	//word
	string(thisx/8,2+thisy/16,"word:");
	hexadecimal(16+thisx/8,2+thisy/16,data16);
	//byte
	string(thisx/8,3+thisy/16,"byte:");
	hexadecimal(16+thisx/8,3+thisy/16,data8);
	//string
	string(thisx/8,4+thisy/16,"ansciistring:");
	string(16+thisx/8,4+thisy/16,thisaddr);
	*/
	//手工输入区域
	//string(16+thisx/8,15+thisy/16,buffer);
}
void hex()
{
	thisx=px*16+16;
	if(thisx>768)thisx-=256+16;
	thisy=py*16+16;
	if(thisy>640-256)thisy-=256+16;

	hexbg();
	printhex0();
	kuangbg();
	kuangcontent();
}
void hexmessage(QWORD type,QWORD key)
{
	if(type==0x776f727261)		//方向键
	{
		if(key==0x25)	//left	0x4b
		{
			if(px>0)px--;
			else
			{
				if(offset>=0x800)offset-=0x800;
			}
		}
		else if(key==0x27)	//right	0x4d
		{
			if(px<0x3f)px++;
			else offset+=0x800;
		}
		else if(key==0x26)	//up	0x4b
		{
			if(py>0)py--;
			else
			{
				if(offset>=0x40)offset-=0x40;
			}
		}
		else if(key==0x28)	//down	0x4d
		{
			if(py<39)py++;
			else offset+=0x40;
		}
	}
	else if(type==0x7466656c)		//鼠标
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;

		//浮动框
		if(x>=thisx)
		{
			if(x<thisx+256)
			{
				if(y>=thisy)
				{
					if(y<thisy+256)
					{
						int tempx=(x-thisx)/128;
						int tempy=(y-thisy)/64;

						if(tempx==0&&tempy==2)printmethod=0;
						if(tempx==1&&tempy==2)printmethod=1;
						if(tempx==0&&tempy==3)printmethod=2;

						return;
					}
				}
			}
		}

		//浮动框以外的
		px=x/(1024/0x40);
		py=y/(640/40);
	}
	else if(type==0x6c65656877)		//滚轮
	{
		if(key<0xff0000)
		{
			if(offset>=0x40)offset-=0x40;
		}
		else if(key>0xff0000)
		{
			//if(offset<0x2000-0xa00)offset+=0x40;
			offset+=0x40;
		}
	}
}
