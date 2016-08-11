#define QWORD unsigned long long
#define DWORD unsigned int
//
void hexadecimal(int x,int y,QWORD in);
//
void printmemory(char*,int);
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

//
static int red=0x8d,green=0x63,blue=0x25;




static void color_list()
{
}
static void color_into()
{
}
static void color_read()
{
	int x,y,min;
	DWORD color;
	DWORD* screenbuf = (DWORD*)(haha->pixelbuffer);

	if( ((haha->width) <= 0) && ((haha->height) <= 0) )
	{
		min=512;	
	}
	else
	{
		if((haha->width) < (haha->height))min = haha->width;
		else min = haha->height;
	}

	//(左边)各种颜色的色板
	if( ((haha->pixelformat)&0xffffffff) == 0x61626772)
	{
		for(y=0;y<min;y++)
		{
			for(x=0;x<min;x++)
			{
				color = 0xff000000
					+ ( ( (x*256) / min ) << 16)
					+ ( ( (y*256) / min ) << 8 )
					+ red;
				screenbuf[(haha->width)*y + x]	=color;
			}
		}

		color=0xff000000+ (blue<<16) + (green<<8) + red;
	}
	else
	{
		for(y=0;y<min;y++)
		{
			for(x=0;x<min;x++)
			{
				color = 0xff000000
					+ (red<<16)
					+ ( ( (y*256) / min ) << 8 )
					+ ( (x*256) / min );
				screenbuf[(haha->width)*y + x]	=color;
			}
		}

		color=0xff000000+ (red<<16) + (green<<8) + blue;
	}

	//(右边)选中的颜色的方块
	if((haha->width) < (haha->height))
	{
		for(y=haha->width;y<haha->height;y++)
		{
			for(x=0;x<haha->width;x++)
			{
				screenbuf[(haha->width)*y + x] = color;
			}
		}
	}
	if((haha->width) > (haha->height))
	{
		for(y=0;y<(haha->height);y++)
		{
			for(x=(haha->height);x<(haha->width);x++)
			{
				screenbuf[(haha->width)*y + x] = color;
			}
		}
	}

	//(白点)被选中的颜色
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			screenbuf[ (haha->width)*(green*min/256+y) + (blue*min/256+x) ]
				= 0xffffffff;
		}
	}

	//
	hexadecimal(0, 0, (red<<16) + (green<<8) + blue);
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

			if( (haha->width) < (haha->height) )min = haha->width;
			else min = haha->height;

			if((x>min)|(y>min))return;
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




static void color_start()
{
}
static void color_stop()
{
}
void color_init(void* base, void* addr)
{
	QWORD* this=(QWORD*)addr;
	haha = addr;

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

