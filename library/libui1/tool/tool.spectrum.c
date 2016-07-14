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
//
void rectangle(DWORD x1y1z1,DWORD x2y2z2,DWORD color);
void backgroundcolor(DWORD);
void background1();
//
int compare(char*,char*);
void data2hexstring(QWORD,char*);
void arteryread(char* rdi,QWORD rsi,QWORD rcx);
void arterywrite(char* rdi,QWORD rsi,QWORD rcx);
//
unsigned int getrandom();
void printmemory(char*,int);
void say(char*,...);




//data
static int real[64];
static int imag[64];
static maxpower;
static BYTE* databuf=0;

//mainscreen
static DWORD* screenbuf;
static int width;
static int height;




static void spectrum_write(QWORD type,QWORD key)
{
	if(type==0x656c6966706f7264)		//'dropfile'
	{
	}
	if(type==0x64626b)			//'kbd'
	{
		if(key==0x25)			//left	0x4b
		{
		}
		else if(key==0x27)		//right	0x4d
		{
		}
		else if(key==0x26)		//up	0x4b
		{
		}
		else if(key==0x28)		//down	0x4d
		{
		}
	}
	else if(type==0x72616863)		//'char'
	{
		if(key==9)					//tab
		{
		}
		else if(key==0x8)			//backspace
		{
		}
		else if(key==0xd)			//enter
		{
		}
		else
		{
		}
	}
	else if(type==0x7466656C207A7978)		//'xyz left'
	{
		int i;
		for(i=0;i<64;i++)
		{
			real[i]=random()%maxpower;
		}
	}
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
	}
}
static void spectrum_read()
{
	int leftupper;
	int rightbottom;
	int min;
	int x,y;

	if(width<height)min=width;
	else min=height;
	rectangle(0, (min<<16)+min, 0);

	for(x=0;x<64;x++)
	{
		y=min - real[x] * min / maxpower;

		leftupper = (x*min/64) + (y<<16);
		rightbottom=(x*min/64 + 4) + (min<<16);
		rectangle(leftupper, rightbottom, 0xffffffff);
	}
}
static void spectrum_into()
{
}
static void spectrum_list(QWORD* this)
{
}








void spectrum_start(QWORD size,void* addr)
{
	int i;

	//
	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);

	//
	screenbuf=addr;
	width=size&0xffff;
	height=(size>>16)&0xffff;
	for(i=0;i<width*height;i++)
	{
		screenbuf[i]=0;
	}

	//
	maxpower=65536;
	for(i=0;i<64;i++)
	{
		real[i]=random()%maxpower;
	}
}
void spectrum_stop()
{
}
void spectrum_init(char* uibuf,char* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x6d75727463657073;
	this[2]=(QWORD)spectrum_start;
	this[3]=(QWORD)spectrum_stop;
	this[4]=(QWORD)spectrum_list;
	this[5]=(QWORD)spectrum_into;
	this[6]=(QWORD)spectrum_read;
	this[7]=(QWORD)spectrum_write;

	databuf=uibuf+0x300000;
}
void spectrum_kill()
{
}
