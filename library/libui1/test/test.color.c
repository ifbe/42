#define QWORD unsigned long long
#define DWORD unsigned int
//
void hexadecimal(int x,int y,QWORD in);
//
void printmemory(char*,int);
void say(char*,...);




static DWORD* screenbuf;
static int width,height;
static int red=0x8d,green=0x63,blue=0x25;




static void color_list()
{
}
static void color_into()
{
}
static void color_read()
{
	int x,y,min,color;
	if( (width<=0) && (height<=0) )
	{
		min=512;	
	}
	else
	{
		if(width<height)min=width;
		else min=height;
	}

	//(左边)各种颜色的色板
	for(y=0;y<min;y++)
	{
		for(x=0;x<min;x++)
		{
			screenbuf[y*width + x]	= 0xff000000
						+ (red<<16)
						+ ( ( (y*256) / min ) << 8 )
						+ ( (x*256) / min );
		}
	}

	//(白点)被选中的颜色
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			screenbuf[width*(green*min/256+y)+(blue*min/256+x)]=0xffffffff;
		}
	}

	//(右边)选中的颜色的方块
	color=0xff000000+ (red<<16) + (green<<8) + blue;
	if(width<height)
	{
		for(y=width;y<height;y++)
		{
			for(x=0;x<width;x++)
			{
				screenbuf[y*width + x] = color;
			}
		}
	}
	if(width>height)
	{
		for(y=0;y<height;y++)
		{
			for(x=height;x<width;x++)
			{
				screenbuf[y*width + x] = color;
			}
		}
	}

	//
	hexadecimal(min/16, 0, color&0xffffff);
}
static void color_write(QWORD type,QWORD key)
{
	if(type==0x64626b)
	{
		if(key==0x25)		//left
		{
			if(blue>0) blue--;
		}
		if(key==0x27)		//right
		{
			if(blue<255) blue++;
		}
		if(key==0x28)		//down
		{
			if(green<255) green++;
		}
		if(key==0x26)		//up
		{
			if(green>0) green--;
		}
	}
	else if(type==0x72616863)
	{
		if(key=='+')
		{
			if(red<255) red++;
		}
		if(key=='-')
		{
			if(red>0) red--;
		}
	}
	else if( (type&0xffffffff) == 0x207A7978 )
	{
		if((type>>32)==0x7466656C)
		{
			int x=key&0xffff;
			int y=(key>>16)&0xffff;
			int min;
			if(width<height)min=width;
			else min=height;

			blue=x*256/min;
			green=y*256/min;
		}
		else if((type>>32)==0x6E6F7266)
		{
			if(red<0xff)red++;
		}
		else if((type>>32)==0x6B636162)
		{
			if(red>0)red--;
		}
	}
}




static void color_start(QWORD size,void* addr)
{
	int i;

	//
	screenbuf=addr;
	width=size&0xffff;
	height=(size>>16)&0xffff;
}
static void color_stop()
{
}
void color_init(char* uibuf,char* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x726f6c6f63;

	this[10]=(QWORD)color_start;
	this[11]=(QWORD)color_stop;
	this[12]=(QWORD)color_list;
	this[13]=(QWORD)color_into;
	this[14]=(QWORD)color_read;
	this[15]=(QWORD)color_write;
}
void color_kill()
{
}

