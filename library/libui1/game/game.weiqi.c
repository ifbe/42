#define QWORD unsigned long long 
#define DWORD unsigned int
//
void line(       int x1,int y1,int x2,int y2, DWORD color);
void circlebody( int x, int y, int r, DWORD color);
void circleframe(int x, int y, int r, DWORD color);
void backgroundcolor(DWORD);
DWORD getrandom();
//
void say(char*,...);




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
static int turn;
static char* data;




void weiqi_read()
{
	int x,y;
	int cx,cy,half;
	DWORD color;

	cx = (haha->width)/2;
	cy = (haha->height)/2;

	if(cy > cx)half = cx/20;
	else half = cy/20;

	if( ((haha->pixelformat)&0xffffffff) == 0x61626772)
        {
		backgroundcolor(0x256f8d);
        }
	else
	{
		backgroundcolor(0x8d6f25);
	}

	//heng
	for(y=-9;y<10;y++)
	{
		line(	cx - half*2*9,	cy + half*2*y,
			cx + half*2*9,	cy + half*2*y,	0);
	}

	//shu
	for(x=-9;x<10;x++)
	{
		line(	cx + half*2*x,	cy - half*2*9,
			cx + half*2*x,	cy + half*2*9,	0);
	}

	for(y=-9;y<=9;y++)
	{
		for(x=-9;x<=9;x++)
		{
			if(data[(y+9)*19 + x+9] == 'b')color = 0;
			else if(data[(y+9)*19 + x+9] == 'w')color = 0xffffffff;
			else continue;

			circlebody(
				cx + half*2*x,
				cy + half*2*y,
				half,
				color
			);
		}
	}
}




void weiqi_write(QWORD type,QWORD key)
{
	char val;
	int x,y;
	int cx,cy,half;

	cx = (haha->width)/2;
	cy = (haha->height)/2;
	if(cy > cx)half = cx/20;
	else half = cy/20;

	if(type==0x7466656C207A7978)
	{
		x=key&0xffff;
		y=(key>>16)&0xffff;

		x = (((x-cx)<<8)/half + 4608 + 128)>>9;
		y = (((y-cy)<<8)/half + 4608 + 128)>>9;
		say("%d,%d\n",x,y);

		turn++;
	}
}




static void weiqi_list()
{
}
static void weiqi_choose()
{
}




static void weiqi_start()
{
	int x,y;

	turn=0;
	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			data[y*19 + x] = 0;
		}
	}

	data[9*19 + 3] = 'b';
	data[7*19 + 9] = 'w';
}
static void weiqi_stop()
{
}




void weiqi_init(void* base,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0x776f646e6977;	//'window'
	this[1]=0x6971696577;	//'weiqi'

	this[10]=(QWORD)weiqi_start;
	this[11]=(QWORD)weiqi_stop;
	this[12]=(QWORD)weiqi_list;
	this[13]=(QWORD)weiqi_choose;
	this[14]=(QWORD)weiqi_read;
	this[15]=(QWORD)weiqi_write;

	data=base+0x300000;
}
void weiqi_kill()
{
}
