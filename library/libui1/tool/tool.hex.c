#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void string(int x,int y,char* str);
void ascii(int x,int y,char ch);
void printascii(int x, int y, int size, char ch, DWORD fg, DWORD bg);
void printbyte(int x, int y, int size, char ch, DWORD fg, DWORD bg);
void backgroundcolor(DWORD);
void background1();
//
void characterchoose(char*);
void arteryread(char* rdi,QWORD rsi,QWORD rcx);
void arterywrite(char* rdi,QWORD rsi,QWORD rcx);
void data2hexstring(QWORD,char*);
int compare(char*,char*);
//
void say(char*,...);
void printmemory(char*,int);




//
static struct temp{
        QWORD type;
        QWORD id;
        QWORD start;
        QWORD end;

        QWORD pixelbuffer;
        QWORD pixelformat;
        QWORD width;
        QWORD height;
}*haha;

//位置
static QWORD base;		//显示区基地址
static QWORD offset;
static BYTE* databuf=0;
static int printmethod=0;

//flostarea
static int inputcount=0;
static BYTE hi[0x100];
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
	int xshift = ((haha->width) & 0x3) <<3;
	unsigned char* where=readornotread(base);

	//
	//printmemory(where,0x200);
	if(printmethod==0)			//hex
	{
		for(y=0;y<(haha->height)/16;y++)
		{
			for(x=0;x<(haha->width)/16;x++)
			{
				printbyte(
					16*x+xshift,
					16*y,
					1,
					where[((haha->width)*y/16) + x],
					0,
					0
				);
			}
		}
	}

	else if(printmethod==1)		//ascii
	{
		for(y=0;y<(haha->height)/16;y++)
		{
			for(x=0;x<(haha->width)/16;x++)
			{
				printascii(
					16*x + xshift,
					16*y,
					1,
					where[((haha->width)*y/16) + x],
					0,
					0
				);
			}
		}
	}

	else if(printmethod == 2)	//utf8
	{
	}
}
static void floatarea()
{
	int x,y;
	int thisx,thisy;
	int byteperline;
	DWORD* screenbuf = (DWORD*)(haha->pixelbuffer);

	byteperline = (haha->width)/16;
	thisx=(offset%byteperline)*16;
	thisy=(offset/byteperline)*16;

	//byte框
	for(y=thisy;y<thisy+16;y++)
	{
		for(x=thisx;x<thisx+16;x++)
		{
			screenbuf[(haha->width)*y + x]=~screenbuf[(haha->width)*y + x];
		}
	}

	//256*128的详情框
	thisx+=16;
	if(thisx > (haha->width) - 256)thisx -= (256+16);
	thisy+=16;
	if(thisy >= (haha->height) - 128+16)thisy -= (128+16);

	for(y=thisy;y<thisy+128;y++)
	{
		for(x=thisx;x<thisx+256;x++)
		{
			screenbuf[(haha->width)*y + x]=0xffff;
		}
	}

	//
	data2hexstring(0x33333333, hi + 0x10);
	data2hexstring(base, hi + 0x30);
	data2hexstring(offset, hi + 0x50);
	data2hexstring(0, hi + 0x70);

	//target,base,offset,data
	for(y=0;y<8;y++)
	{
		for(x=0;x<32;x++)
		{
			printascii(
				thisx + x*8,
				thisy + y*16,
				1,
				hi[(y*32) + x],
				0,
				0
			);
		}
	}
}








static void hex_write(QWORD type,QWORD key)
{
	int byteperline = (haha->width)/16;
	int totalbyte=( (haha->width) >> 4 ) * ( (haha->height) >> 4);

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
			if( offset*16 < byteperline*((haha->height)-16) )
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
			hi[0x80+inputcount]=0;
		}
		else if(key==0xd)			//enter
		{
			if(compare( hi+0x80 , "exit" ) == 0)
			{
				characterchoose(0);
				return;
			}
			else if(compare( hi+0x80 , "addr" ) == 0)
			{
			}
		}
		else
		{
			if(inputcount<128)
			{
				hi[0x80+inputcount]=key;
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








static void hex_list(QWORD* this)
{
}
static void hex_into()
{
}








void hex_start()
{
	int j;
	backgroundcolor(0);

	//浮动框
	for(j=0;j<0x100;j++)hi[j]=0;
	*(QWORD*)hi=0x3a746567726174;
	*(QWORD*)(hi+0x20)=0x3a65736162;
	*(QWORD*)(hi+0x40)=0x3a74657366666f;
	*(QWORD*)(hi+0x60)=0x3a61746164;

	//文件内部偏移
	base=0;
	offset=0;
	currentcache=0xffffffff;
}
void hex_stop()
{
}
void hex_init(void* uibuf,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0x776f646e6977;
	this[1]=0x786568;

	this[10]=(QWORD)hex_start;
	this[11]=(QWORD)hex_stop;
	this[12]=(QWORD)hex_list;
	this[13]=(QWORD)hex_into;
	this[14]=(QWORD)hex_read;
	this[15]=(QWORD)hex_write;

	databuf = uibuf;
}
void hex_kill()
{
}
