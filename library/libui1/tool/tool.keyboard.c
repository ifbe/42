#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void backgroundcolor(DWORD);




DWORD* screenbuf=0;
int xsize=0;
int ysize=0;




static void keyboard_write(QWORD type,QWORD value)
{
	
}
static void keyboard_read()
{
	int x,y;
	for(y=0;y<768;y++)
	{
		for(x=0;x<1024;x++)
		{
			screenbuf[y*1024+x]=0;
		}
	}

	//横线
	for(y=64;y<=768-64;y+=64)
	{
		for(x=0;x<1024;x++)
		{
			screenbuf[y*1024+x]=0xffffffff;
		}
	}

	//竖线
	for(x=128;x<1024;x+=128)
	{
		for(y=64;y<=768-64;y++)
		{
			screenbuf[y*1024+x]=0xffffffff;
		}
	}
}
static void keyboard_into()
{
	backgroundcolor(0);
}
static void keyboard_list()
{
}








static void keyboard_start()
{
}
static void keyboard_stop()
{
}
void keyboard_init(QWORD size,void* addr)
{
	if(size==0)
	{
		QWORD* this=(QWORD*)addr;
		this[0]=0x776f646e6977;
		this[1]=0x786568;
		this[2]=(0<<16)+0;      //left,up
		this[3]=(768<<16)+1024; //right,down
		this[4]=(QWORD)keyboard_list;
		this[5]=(QWORD)keyboard_into;
		this[6]=(QWORD)keyboard_read;
		this[7]=(QWORD)keyboard_write;
	}
	else
	{
		//
		xsize=size&0xffff;
		ysize=(size>>16)&0xffff;

		//
		screenbuf=addr;
	}
}
void keyboard_kill()
{
}
