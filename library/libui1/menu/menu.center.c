#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int characterchoose(char* p);
//
void printstring(int x, int y, int size, char* str, DWORD fg, DWORD bg);
void rect(int x1, int y1, int x2, int y2, DWORD bodycolor, DWORD framecolor);
//
void say(char*,...);




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
//菜单
static char buffer[128];
static int bufp=0;




static void menu_read_text()
{
	int x,y;
	int width=haha->width;
	int height=haha->height;
	char* p = (char*)(haha->pixelbuffer);
	char* src;
	char* dst;

	for(y=height/4;y<height*3/4;y++)
	{
		for(x=width/4;x<width*3/4;x++)
		{
			p[x + width*y] = 0x20;
		}
	}
	for(x=width/4;x<=width*3/4;x++)
	{
		p[x + (height/4)*width] = '-';
		p[x + (height*3/4)*width] = '-';
	}
	for(y=height/4;y<=height*3/4;y++)
	{
		p[width/4 + y*width] = '|';
		p[width*3/4 + y*width] = '|';
	}

	dst = p + (height/4+1)*width + width/4 + 1;
	src = "what do you want?";
	x=0;
	while(src[x] != 0)
	{
		dst[x]=src[x];
		x++;
	}

	dst = p + (height/4+2)*width + width/4 + 1;
	src = buffer;
	for(x=0;x<bufp;x++)
	{
		dst[x]=src[x];
	}
}
static void menu_read_pixel()
{
	int x,y;
	int width=haha->width;
	int height=haha->height;

	//title
	rect(
		width/4,
		(height/4)&0xfffffff0,
		width*3/4,
		(height/4+16)&0xfffffff0,
		0x01234567,
		0xfedcba98
	);

	//body
	rect(
		width/4,
		(height/4+16)&0xfffffff0,
		width*3/4,
		height*3/4,
		0,
		0xffffffff
	);

	//button
	rect(
		(width*3/4) - 16,
		(height/4)&0xfffffff0,
		width*3/4,
		((height/4) + 16)&0xfffffff0,
		0xff0000,
		0
	);

	//string
	printstring(
		width/4,
		height/4 + 16,
		1,
		"what do you want?",
		0xffffffff,
		0
	);
	printstring(
		width/4,
		height/4 + 32,
		1,
		buffer,
		0xffffffff,
		0
	);
}
static void menu_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		menu_read_text();
	}
/*
	//html
	else if()
	{
	}
*/
	//pixel
	else
	{
		menu_read_pixel();
	}
}




//write,read,into,list
static void menu_write(QWORD type,QWORD key)
{
	int width=haha->width;
	int height=haha->height;

	//'xyz left'
	if(type==0x7466656C207A7978)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;

		//点击框框外面，关掉菜单
		if(	(x<width/4) |
			(x>width*3/4) |
			(y>(height*3/4)&0xfffffff0) |
			(y<height/4) )
		{
			haha->start=0;
			return;
		}

		//点击红色矩形，退出
		else if( (y<256+16) && (x>768-16) )
		{
			//退出
			characterchoose("exit");
			return;
		}
	}//left

	//'char'
	else if(type==0x72616863)
	{
		if(key==0x8)		//backspace
		{
			if(bufp!=0)
			{
				bufp--;
				buffer[bufp]=0;
			}
		}
		else if( (key==0xa) | (key==0xd) )	//回车
		{
			//say("%s\n",buffer);
			characterchoose(buffer);

			//clear
			for(bufp=0;bufp<127;bufp++) buffer[bufp]=0;
			bufp=0;
		}
		else
		{
			if(bufp<0x80)
			{
				buffer[bufp]=key&0xff;
				bufp++;
			}
		}
	}//kbd
}




static void menu_choose()
{
}
static void menu_list()
{
}




static void menu_start()
{
}
static void menu_stop()
{
}
void menu_create(void* base,void* addr)
{
	QWORD* this=addr;
	haha=addr;

	this[0]=0;
	this[1]=0x756e656d;

	this[10]=(QWORD)menu_start;
	this[11]=(QWORD)menu_stop;
	this[12]=(QWORD)menu_list;
	this[13]=(QWORD)menu_choose;
	this[14]=(QWORD)menu_read;
	this[15]=(QWORD)menu_write;
}
void menu_delete()
{
}
