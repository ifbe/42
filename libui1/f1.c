#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

void die();

void hexadecimal(int x,int y,QWORD in);
void hexadecimal1234(int x,int y,QWORD in);
void string(int x,int y,char* str);
void anscii(int x,int y,char ch);
void blackanscii(int x,int y,char ch);
void point(int x,int y,DWORD color);
void background1();

int compare(char*,char*);
void data2hexstring(QWORD,char*);
void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
QWORD whereisworld();
QWORD whereisscreen();




//位置
static QWORD base;		//显示区基地址
static QWORD offset;

//data
static BYTE* datahome;
static int printmethod=0;

//---------
struct thisstruct{
	QWORD targetstring;//=0x11223344;
	QWORD targetstring1;//=0x31323334;
	BYTE target[0x10];

	QWORD basestring;//=0x11223344;
	QWORD basestring1;//=0x31323334;
	BYTE base[0x10];

	QWORD offsetstring;//=0x11223344;
	QWORD offsetstring1;//=0x31323334;
	BYTE offset[0x10];

	QWORD datastring;//=0x11223344;
	QWORD datastring1;//=0x31323334;
	BYTE data[0x10];

	BYTE reserved[0x10*2*3];
	BYTE input[0x10*2];
};
static int inputcount=0;
static struct thisstruct haha;
	//string(chx,chy,"target:");
	//string(chx,1+chy,"base:");
	//string(chx,2+chy,"offset:");
	//string(chx,3+chy,"data:");




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
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//byte框
	int thisx=(offset&0x3f)*16;
	int thisy=(offset%0xa00)/0x40*16;
	for(y=thisy;y<thisy+16;y++)
	{
		for(x=thisx;x<thisx+16;x++)
		{
			screenbuf[y*1024+x]=~screenbuf[y*1024+x];
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
			screenbuf[y*1024+x]=0xffffffff;
		}
	}

	//
	data2hexstring(0x33333333,haha.target);
	data2hexstring(base,haha.base);
	data2hexstring(offset,haha.offset);
	data2hexstring(0x2333333,haha.data);

	//target,base,offset,data
	int chx=thisx/8;
	int chy=thisy/16;
	char* ch=(char*)&haha;
	for(y=0;y<8;y++)
	{
		for(x=0;x<32;x++)
		{
			blackanscii(chx+x,chy+y,ch[y*32+x]);
		}
	}
}








void f1show()
{
	//
	background1();
	printhex0();

	//
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
				if(base>=0x40)base-=0x40;
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
		else if(key==0x8)			//backspace
		{
			if(inputcount!=0)inputcount--;
			haha.input[inputcount]=0;
		}
		else if(key==0xd)			//enter
		{
			if(compare( haha.input , "exit" ) == 0)
			{
				die();
				return;
			}
			else if(compare( haha.input , "addr" ) == 0)
			{
			}

			//int i;
			//inputcount=0;
			//for(i=0;i<128;i++)
			//{
			//	haha.input[i]=0;
			//}
		}
		else
		{
			if(inputcount<128)
			{
				haha.input[inputcount]=key;
				inputcount++;
			}
		}
	}
	else if(type==0x7466656c)		//鼠标
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		offset=(y/16*0x40)+(x/16);

		//浮动框以外的
		//px=x/(1024/0x40);
		//py=y/(640/40);
	}
	else if(type==0x6c65656877)		//滚轮
	{
		if(key<0xff0000)
		{
			if(base>=0x40)base-=0x40;
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

	haha.targetstring=0x3a746567726174;
	haha.basestring=0x3a65736162;
	haha.offsetstring=0x3a74657366666f;
	haha.datastring=0x3a61746164;

	base=0;
	offset=0;
	currentcache=0xffffffff;
}