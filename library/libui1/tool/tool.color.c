#define QWORD unsigned long long
#define DWORD unsigned int
//
void string(int x,int y,char* str);
void hexadecimal(int x,int y,QWORD in);
//
void background_start(QWORD size,void* addr);
void shape_start(QWORD size,void* addr);
void ascii_start(QWORD size,void* addr);
void unicode_start(QWORD size,void* addr);
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
	for(x=0;x<min;x++)
	{
		for(y=0;y<min;y++)
		{
			screenbuf[y*width + x]	= (red<<16)
						+ ( ( (y*256) / min ) << 8 )
						+ ( (x*256) / min );
		}
	}
say("min=%d\n",min);

	//(白点)被选中的颜色
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			screenbuf[width*(green*min/256+y)+(blue*min/256+x)]=0xffffffff;
		}
	}

	//(右边)选中的颜色的方块
	color=(red<<16)+(green<<8)+blue;
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
	string(0x40,0,"color= #");
	hexadecimal(0x48,0,color);
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
	else if(type==0x7466656C207A7978)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		int min;
		if(width<height)min=width;
		else min=height;

		blue=x*256/min;
		green=y*256/min;
say("(%d,%d)\n",x,y);
	}
}




static void color_start(QWORD size,void* addr)
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
}
static void color_stop()
{
}
void color_init(char* uibuf,char* addr)
{
        QWORD* this=(QWORD*)addr;
        this[0]=0x776f646e6977;
        this[1]=0x726f6c6f63;
        this[2]=(QWORD)color_start;
        this[3]=(QWORD)color_stop;
        this[4]=(QWORD)color_list;
        this[5]=(QWORD)color_into;
        this[6]=(QWORD)color_read;
        this[7]=(QWORD)color_write;
}
void color_kill()
{
}

