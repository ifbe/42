#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void characterchoose(char*);
void background_start(QWORD size,void* addr);
void shape_start(QWORD size,void* addr);
void ascii_start(QWORD size,void* addr);
void unicode_start(QWORD size,void* addr);
//
void hexadecimal(int x,int y,QWORD in);
void hexadecimal1234(int x,int y,QWORD in);
void string(int x,int y,char* str);
void ascii(int x,int y,char ch);
void colorascii(int x,int y,char ch,unsigned int color);
void backgroundcolor(DWORD);
void background1();
//
int compare(char*,char*);
void data2hexstring(QWORD,char*);
void arteryread(char* rdi,QWORD rsi,QWORD rcx);
void arterywrite(char* rdi,QWORD rsi,QWORD rcx);
//
void say(char*,...);
void printmemory(char*,int);








//位置
static QWORD base;		//显示区基地址
static QWORD offset;
static BYTE* databuf=0;
static int printmethod=0;

//mainscreen
static DWORD* screenbuf;
static int width;
static int height;

//flostarea
static int inputcount=0;
static BYTE haha[0x100];
	//[0,0x1f]:target,value
	//[0x20,0x3f]:base,value
	//[0x40,0x5f]:offset,value
	//[0x60,0x7f]:data,value








//in:想要哪儿
//out:请求的地方的内存地址
//作用:防止每动一下就读一次硬盘
static QWORD currentcache;
static char* readornotread(QWORD wantaddr)
{
	//假如每次能显示0x1000(实际是0xa00)
	//想要[0,0x1000)：	确保[0,0x2000)		返回databuf+0
	//想要[0xfc0,0x1fc0)：	确保[0,0x2000)		返回databuf+0xfc0
	//想要[0x1000,0x2000)：	确保[0x1000,0x3000)	返回databuf+0x1000
	//想要[0x1040,0x2040)：	确保[0x1000,0x3000)	返回databuf+0x40
	//想要[0x1fc0,0x2fc0)：	确保[0x1000,0x3000)	返回databuf+0xfc0
	//想要[0x2000,0x3000)：	确保[0x2000,0x4000)	返回databuf+0
	//想要[0x2040,0x3040)：	确保[0x2000,0x4000)	返回databuf+0x40
	//想要[0x2fc0,0x3fc0)：	确保[0x2000,0x4000)	返回databuf+0xfc0
	//想要[0x3000,0x4000)：	确保[0x3000,0x5000)	返回databuf+0

	QWORD readwhere=wantaddr & 0xfffffffffffff000;
	if(readwhere!=currentcache)
	{
		arteryread(databuf, readwhere/0x200, 16);
		currentcache=readwhere;
	}
	return databuf+(wantaddr-readwhere);
}
static void foreground()
{
	//一整页
	int x,y;
	int xshift = width & 0x3;
	char* where=readornotread(base);

	//
	if(printmethod==0)			//hex
	{
		for(y=0;y<height/16;y++)
		{
			for(x=0;x<width/16;x+=4)
			{
				DWORD value=*(DWORD*)(where + (y*width/16) + x);
				hexadecimal1234(2*x+xshift,y,value);
			}
		}
	}
	else if(printmethod==1)		//ascii
	{
		for(y=0;y<height/16;y++)
		{
			for(x=0;x<width/16;x+=4)
			{
				DWORD value=*(DWORD*)(where + (y*width/16) + x);
				colorascii(2*x+xshift,y,value&0xff,0);
				colorascii(2*x+2+xshift,y,(value>>8)&0xff,0);
				colorascii(2*x+4+xshift,y,(value>>16)&0xff,0);
				colorascii(2*x+6+xshift,y,(value>>24)&0xff,0);
			}
		}
	}
}
static void floatarea()
{
	int x,y;
	int thisx,thisy;
	int byteperline;

	byteperline=width/16;
	thisx=(offset%byteperline)*16;
	thisy=(offset/byteperline)*16;

	//byte框
	for(y=thisy;y<thisy+16;y++)
	{
		for(x=thisx;x<thisx+16;x++)
		{
			screenbuf[y*width+x]=~screenbuf[y*width+x];
		}
	}

	//256*128的详情框
	thisx+=16;
	if(thisx>width-256)thisx -= (256+16);
	thisy+=16;
	if(thisy>=height-128+16)thisy -= (128+16);

	for(y=thisy;y<thisy+128;y++)
	{
		for(x=thisx;x<thisx+256;x++)
		{
			screenbuf[y*width+x]=0xffff;
		}
	}

	//
	data2hexstring(0x33333333,haha+0x10);
	data2hexstring(base,haha+0x30);
	data2hexstring(offset,haha+0x50);
	data2hexstring(0,haha+0x70);

	//target,base,offset,data
	int chx=thisx/8;
	int chy=thisy/16;
	for(y=0;y<8;y++)
	{
		for(x=0;x<32;x++)
		{
			colorascii(chx+x,chy+y,haha[(y*32) + x],0);
		}
	}
}








static void hex_write(QWORD type,QWORD key)
{
	int byteperline=width/16;
	int totalbyte=( width >> 4 ) * ( height >> 4);

	if(type==0x64626b)			//'kbd'
	{
		if(key==0x25)			//left	0x4b
		{
			if( offset % byteperline == 0 )
			{
				if(base >= totalbyte)base -= totalbyte;
			}
			else
			{
				offset--;
			}
		}
		else if(key==0x27)		//right	0x4d
		{
			if( offset % byteperline == byteperline-1 )
			{
				base += totalbyte;
			}
			else
			{
				offset++;
			}
		}
		else if(key==0x26)		//up	0x4b
		{
			if(offset<byteperline)
			{
				if(base >= byteperline)base -= byteperline;
			}
			else
			{
				offset -= byteperline;
			}
		}
		else if(key==0x28)		//down	0x4d
		{
			if( offset*16 < byteperline*(height-16) )
			{
				offset += byteperline;
			}
			else
			{
				base+=byteperline;
			}
		}
	}
	else if(type==0x72616863)		//'char'
	{
		if(key==9)					//tab
		{
			printmethod=(printmethod+1)%2;
		}
		else if(key==0x8)			//backspace
		{
			if(inputcount!=0)inputcount--;
			haha[0x80+inputcount]=0;
		}
		else if(key==0xd)			//enter
		{
			if(compare( haha+0x80 , "exit" ) == 0)
			{
				characterchoose(0);
				return;
			}
			else if(compare( haha+0x80 , "addr" ) == 0)
			{
			}
		}
		else
		{
			if(inputcount<128)
			{
				haha[0x80+inputcount]=key;
				inputcount++;
			}
		}
	}
	else if(type==0x7466656C207A7978)		//'xyz left'
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		offset=( (y/16) * byteperline ) + ( x / 16 );

		//浮动框以外的
		//px=x/(1024/0x40);
		//py=y/(640/40);
	}
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
		if(base>=0x40)base-=0x40;
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
		base+=0x40;
	}
}
static void hex_read()
{
	//背景
	background1();

	//
	foreground();

	//
	floatarea();
}
static void hex_into()
{
}
static void hex_list(QWORD* this)
{
}








void hex_start(QWORD size,void* addr)
{
	int i;

	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);

	//
	screenbuf=addr;
	width=size&0xffff;
	height=(size>>16)&0xffff;

	//浮动框
	for(i=0;i<0x100;i++)haha[i]=0;
	*(QWORD*)haha=0x3a746567726174;
	*(QWORD*)(haha+0x20)=0x3a65736162;
	*(QWORD*)(haha+0x40)=0x3a74657366666f;
	*(QWORD*)(haha+0x60)=0x3a61746164;

	//文件内部偏移
	base=0;
	offset=0;
	currentcache=0xffffffff;
	backgroundcolor(0);
}
void hex_stop()
{
}
void hex_init(char* uibuf,char* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x786568;
	this[2]=(QWORD)hex_start;
	this[3]=(QWORD)hex_stop;
	this[4]=(QWORD)hex_list;
	this[5]=(QWORD)hex_into;
	this[6]=(QWORD)hex_read;
	this[7]=(QWORD)hex_write;

	databuf=uibuf+0x300000;
}
void hex_kill()
{
}
