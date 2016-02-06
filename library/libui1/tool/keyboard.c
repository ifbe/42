#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void backgroundcolor(DWORD);




DWORD* screenbuf=0;
int xsize=0;
int ysize=0;




static void writekeyboard(QWORD type,QWORD value)
{
	
}
static void readkeyboard()
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
static void intokeyboard()
{
	backgroundcolor(0);
}
void listkeyboard(QWORD* this)
{
	this[0]=0x776f646e6977;
	this[1]=0x786568;
	this[2]=(0<<16)+0;      //left,up
	this[3]=(768<<16)+1024; //right,down
	this[4]=(QWORD)listkeyboard;
	this[5]=(QWORD)intokeyboard;
	this[6]=(QWORD)readkeyboard;
	this[7]=(QWORD)writekeyboard;
}








void initkeyboard(QWORD size,void* addr)
{
	//
	xsize=size&0xffff;
	ysize=(size>>16)&0xffff;

	//
	screenbuf=addr;
}
void killkeyboard()
{
}
