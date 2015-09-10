#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void hexadecimal(int x,int y,QWORD in);
void hexadecimal1234(int x,int y,QWORD in);
void string(int x,int y,char* str);
void anscii(int x,int y,char ch);
void point(int x,int y,DWORD color);
void background1();
void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
QWORD whereisworld();




//键盘输入
static BYTE buffer[128];		//键盘输入专用
static int bufcount=0;

//位置
static QWORD base;		//显示区基地址
static QWORD offset;
static int thisx;			//浮动窗口位置
static int thisy;

//
static BYTE* datahome;
static int printmethod=0;




//in:想要哪儿
//out:请求的地方的内存地址
//作用:防止每动一下就读一次硬盘
static QWORD currentcache;
QWORD readornotread(QWORD wantaddr)
{
	//假如每次能显示0x1000(实际是0xa00)
	//想要[0,0x1000)：			确保[0,0x2000)			返回datahome+0
	//想要[0xfc0,0x1fc0)：		确保[0,0x2000)			返回datahome+0xfc0
	//想要[0x1000,0x2000)：		确保[0x1000,0x3000)		返回datahome+0x1000
	//想要[0x1040,0x2040)：		确保[0x1000,0x3000)		返回datahome+0x40
	//想要[0x1fc0,0x2fc0)：		确保[0x1000,0x3000)		返回datahome+0xfc0
	//想要[0x2000,0x3000)：		确保[0x2000,0x4000)		返回datahome+0
	//想要[0x2040,0x3040)：		确保[0x2000,0x4000)		返回datahome+0x40
	//想要[0x2fc0,0x3fc0)：		确保[0x2000,0x4000)		返回datahome+0xfc0
	//想要[0x3000,0x4000)：		确保[0x3000,0x5000)		返回datahome+0
	QWORD readwhere=wantaddr & 0xfffffffffffff000;
	if(readwhere!=currentcache)
	{
		readmemory((QWORD)datahome, readwhere/0x200, 0, 16);
		currentcache=readwhere;
	}

	return (QWORD)datahome+(wantaddr-readwhere);
}








void gridding()
{
	//横
	int x,y;
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
}
void printhex0()
{
	//告诉这个函数想要什么地方,它会确保想要的地方已经在内存里
	//返回的是请求的地方相对datahome的偏移
	int x,y;
	QWORD readwhere=readornotread(base);

	//一整页
	if(printmethod==0)			//hex
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
	else if(printmethod==1)		//anscii
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
	/*
	else if(printmethod==2)		//text editor
	{
		char* this=(char*)readwhere;
		int i=0;
		x=y=0;
		while(1)
		{
			if(this[i]==9)
			{
				anscii(x,y,0x20);
				anscii(x+1,y,0x20);
				anscii(x+2,y,0x20);
				anscii(x+3,y,0x20);
				x+=4;
				if(x>=0x80){x=0;y++;}
			}
			else if(this[i]==0xa)
			{
				x=0;
				y++;
			}
			else
			{
				anscii(x,y,this[i]);
				x++;
				if(x>=0x80){x=0;y++;}
			}

			i++;
			if(y>=0x40)break;
		}
	}*/
}
void floatarea()
{
	int x,y;

	//byte框
	thisx=(offset&0x3f)*16;
	thisy=(offset%0xa00)/0x40*16;
	for(y=thisy;y<thisy+16;y++)
	{
		for(x=thisx;x<thisx+16;x++)
		{
			point(x,y,0xffff);
		}
	}

	//256*128的详情框
	thisx+=16;
	if(thisx>768)thisx -= (256+16);
	thisy+=16;
	if(thisy>=640-128+16)thisy -= (128+16);

	for(y=thisy;y<thisy+128;y++)
	{
		for(x=thisx;x<thisx+256;x++)
		{
			point(x,y,0x77777777);
		}
	}

	//target,base,offset,data
	int chx=thisx/8;
	int chy=thisy/16;

	string(chx,chy,"target:");
	string(16+chx,chy,"/dev/sda");

	string(chx,1+chy,"base:");
	hexadecimal(16+chx,1+chy,base);

	string(chx,2+chy,"offset:");
	hexadecimal(16+chx,2+chy,offset);

	string(chx,3+chy,"data:");
	hexadecimal(16+chx,3+chy,0x2333333);

	string(chx,7+chy,buffer);
}








void f1show()
{
	background1();
	gridding();
	printhex0();
	floatarea();
}
void f1message(QWORD type,QWORD key)
{
	if(type==0x64626b)			//kbd
	{
		if(key==0x25)			//left	0x4b
		{
			if( (offset&0x3f) > 0 )offset--;
			else
			{
				if(base>=0x800)base-=0x800;
			}
		}
		else if(key==0x27)		//right	0x4d
		{
			if( (offset&0x3f) < 0x3f)offset++;
			else
			{
				base+=0x800;
			}
		}
		else if(key==0x26)		//up	0x4b
		{
			if(offset>=0x40)offset-=0x40;
			else
			{
				if(base>0x40)base-=0x40;
			}
		}
		else if(key==0x28)		//down	0x4d
		{
			if(offset<0xa00-0x40)offset+=0x40;
			else base+=0x40;
		}
	}
	else if(type==0x72616863)		//char
	{
		if(key==9)					//tab
		{
			printmethod=(printmethod+1)%2;
		}
		else if(key==0x1c){}		//enter
		else if(key==0x8)			//backspace
		{
			if(bufcount!=0)bufcount--;
			buffer[bufcount]=0;
		}
		else
		{
			if(bufcount<128)
			{
				buffer[bufcount]=key;
				bufcount++;
			}
		}
	}
	else if(type==0x7466656c)		//鼠标
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		offset=(y/16*0x40)+(x/16);

		/*
		//浮动框
		if(x>=thisx)
		{
			if(x<thisx+256)
			{
				if(y>=thisy)
				{
					if(y<thisy+64)
					{
						return;
						int tempx=(x-thisx)/128;
						int tempy=(y-thisy)/64;

						if(tempx==0&&tempy==2)printmethod=0;
						if(tempx==1&&tempy==2)printmethod=1;
						if(tempx==0&&tempy==3)printmethod=2;
					}
				}
			}
		}
		*/

		//浮动框以外的
		//px=x/(1024/0x40);
		//py=y/(640/40);
	}
	else if(type==0x6c65656877)		//滚轮
	{
		if(key<0xff0000)
		{
			if(base>0x40)base-=0x40;
		}
		else if(key>0xff0000)
		{
			base+=0x40;
		}
	}
}








void f1init()
{
	int i;
	datahome=(BYTE*)whereisworld()+0x400000;
	for(i=0;i<0x2000;i++)datahome[i]=0;

	base=0;
	offset=0;
	currentcache=0xffffffff;
}