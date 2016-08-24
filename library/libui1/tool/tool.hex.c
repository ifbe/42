#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void printascii(int x, int y, int size, char ch, DWORD fg, DWORD bg);
void printbyte(int x, int y, int size, char ch, DWORD fg, DWORD bg);
void rectbody(int x1, int y1, int x2, int y2, DWORD color);
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

//flostarea
static int inputcount=0;
static BYTE hi[0x100];
	//[0,0x1f]:target,value
	//[0x20,0x3f]:base,value
	//[0x40,0x5f]:offset,value
	//[0x60,0x7f]:data,value

//where
static BYTE* databuf=0;
static QWORD windowoffset;
static QWORD pointeroffset;





static int printmethod=0;
static int xshift=0;
static int byteperline=0;
static int lineperwindow=0;
static void updateconfig()
{
	int width = haha->width;
	unsigned int pixfmt = (haha->pixelformat)&0xffffffff;

        //text
        if( pixfmt == 0x74786574)
	{
		lineperwindow = haha->height;

		if(width >= 0x80)
		{
			byteperline = 0x40;
			xshift = (width - 0x80)/2;
		}
		else if(width >= 0x40)
		{
			byteperline = 0x20;
			xshift = (width - 0x40)/2;
		}
		else if(width >= 0x20)
		{
			byteperline = 0x10;
			xshift = (width - 0x20)/2;
		}
		else
		{
			byteperline = 0;
			xshift = 0;
		}
	}
	else
	{
		lineperwindow = (haha->height)/16;

		if(width >= 2048)
		{
			byteperline = 0x80;
			xshift = (width - 2048)/2;
		}
		else if(width >= 1024)
		{
			byteperline = 0x40;
			xshift = (width - 1024)/2;
		}
		else if(width >= 512)
		{
			byteperline = 0x20;
			xshift = (width - 512)/2;
		}
		else if(width >= 256)
		{
			byteperline = 0x10;
			xshift = (width - 256)/2;
		}
		else
		{
			byteperline = 0;
			xshift = 0;
		}
	}
}
static void foreground()
{
	//一整页
	int x,y;
	int height=haha->height;

	if(printmethod==0)			//hex
	{
		for(y=0;y<height/16;y++)
		{
			for(x=0;x<byteperline;x++)
			{
				printbyte(
					16*x + xshift,
					16*y,
					1,
					databuf[windowoffset + y*byteperline + x],
					0,
					0
				);
			}
		}
	}

	else if(printmethod==1)		//ascii
	{
		for(y=0;y<height/16;y++)
		{
			for(x=0;x<byteperline;x++)
			{
				printascii(
					16*x + xshift,
					16*y,
					1,
					databuf[windowoffset + y*byteperline + x],
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
	DWORD* screenbuf;
	int width,height;
	int thisx,thisy;
	int x,y;

	screenbuf = (DWORD*)(haha->pixelbuffer);
	width = haha->width;
	height = haha->height;
	thisx = (pointeroffset % byteperline) << 4;
	thisy = (pointeroffset / byteperline) << 4;

	//byte框
	for(y=thisy;y<thisy+16;y++)
	{
		for(x=thisx;x<thisx+16;x++)
		{
			screenbuf[xshift + y*width + x] = ~screenbuf[xshift + y*width + x];
		}
	}

	//256*128的详情框
	thisx+=16;
	thisy+=16;
	if(thisx > width -xshift -256)thisx -= (256+16);
	if(thisy >= height - 128)thisy -= (128+16);
	rectbody(xshift + thisx, thisy, xshift + thisx+256, thisy+128, 0xffff);

	//
	data2hexstring((QWORD)databuf, hi + 0x10);
	data2hexstring(windowoffset, hi + 0x30);
	data2hexstring(pointeroffset, hi + 0x50);
	data2hexstring(0, hi + 0x70);

	//target,base,offset,data
	for(y=0;y<8;y++)
	{
		for(x=0;x<32;x++)
		{
			printascii(
				xshift + thisx + x*8,
				thisy + y*16,
				1,
				hi[(y*32) + x],
				0,
				0
			);
		}
	}
}
static void hex_read_pixel()
{
	updateconfig();

	//背景
	background1();

	//
	foreground();

	//
	floatarea();
}
static void hex_read_text()
{
	int x,y;
	unsigned char h,l;

	int width = haha->width;
	int height = haha->height;
	char* p = (char*)(haha->pixelbuffer);

	updateconfig();
	//for(x=0;x<width*height;x++)p[x]=0x20;

	if(printmethod==0)		//hex
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<byteperline;x++)
			{
				h = l = databuf[windowoffset + y*byteperline + x];

				h = ( (h>>4)&0xf ) + 0x30;
				if(h>0x39)h += 0x7;
				p[xshift + y*width + x*2] = h;

				l = (l&0xf) + 0x30;
				if(l>0x39)l += 0x7;
				p[xshift + y*width + x*2 + 1] = l;
			}
		}
	}

	else if(printmethod==1)		//ascii
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<byteperline;x++)
			{

				h = databuf[windowoffset + y*byteperline + x];
				if( (h>0x20)&&(h<0x80) )
				{
					p[xshift + y*width + x*2] = 0x20;
					p[xshift + y*width + x*2 + 1] = h;
				}
				else
				{
					p[xshift + y*width + x*2] = 0x20;
					p[xshift + y*width + x*2 + 1] = 0x20;
				}
			}
		}
	}
}
static void hex_read_html()
{
}
static void hex_read()
{
	unsigned int pixfmt = (haha->pixelformat)&0xffffffff;

	//text
	if( pixfmt == 0x74786574)
	{
		hex_read_text();
	}

	//html
	else if( pixfmt == 0x6c6d7468)
	{
		hex_read_html();
	}

	//pixel
	else
	{
		hex_read_pixel();
	}
}








static void hex_write(QWORD type,QWORD key)
{
	updateconfig();

	if(type==0x64626b)			//'kbd'
	{
		if(key==0x25)			//left	0x4b
		{
			if( pointeroffset % byteperline == 0 )
			{
				windowoffset -= byteperline * lineperwindow;
			}
			else
			{
				pointeroffset--;
			}
		}
		else if(key==0x27)		//right	0x4d
		{
			if( pointeroffset % byteperline == byteperline-1 )
			{
				windowoffset += byteperline * lineperwindow;
			}
			else
			{
				pointeroffset++;
			}
		}
		else if(key==0x26)		//up	0x4b
		{
			if( pointeroffset < byteperline )
			{
				windowoffset -= byteperline;
			}
			else
			{
				pointeroffset -= byteperline;
			}
		}
		else if(key==0x28)		//down	0x4d
		{
			if( pointeroffset > (lineperwindow-1) * byteperline )
			{
				windowoffset += byteperline;
			}
			else
			{
				pointeroffset += byteperline;
			}
		}
	}
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
		if( pointeroffset < byteperline )
		{
			windowoffset -= byteperline;
		}
		else
		{
			pointeroffset -= byteperline;
		}
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
		if( pointeroffset < (lineperwindow-1) * byteperline )
		{
			windowoffset += byteperline;
		}
		else
		{
			pointeroffset += byteperline;
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
		pointeroffset = ( (y/16) * byteperline ) + ( (x-xshift) / 16 );

		//浮动框以外的
		//px=x/(1024/0x40);
		//py=y/(640/40);
	}
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

	//偏移
	windowoffset = pointeroffset = 0;

	//浮动框
	for(j=0;j<0x100;j++)hi[j]=0;
	*(QWORD*)hi=0x3a746567726174;
	*(QWORD*)(hi+0x20)=0x3a65736162;
	*(QWORD*)(hi+0x40)=0x3a74657366666f;
	*(QWORD*)(hi+0x60)=0x3a61746164;
}
void hex_stop()
{
}
void hex_create(void* uibuf,void* addr)
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
void hex_delete()
{
}
