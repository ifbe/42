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
static char data[10][9];




void xiangqi_read()
{
	int x,y;
	int black,red;
	int cx,cy,half;

	cx = (haha->width)/2;
	cy = (haha->height)/2;

	if(cy*9 > cx*10)half = cx/9;
	else half = cy/10;

	if( ((haha->pixelformat)&0xffffffff) == 0x61626772)
        {
		backgroundcolor(0x256f8d);
		black=0;
		red=0xff;
        }
	else
	{
		backgroundcolor(0x8d6f25);
		black=0;
		red=0xff0000;
	}

	//heng
	for(y=-5;y<5;y++)
	{
		line(	cx - half*8,	cy + half*(2*y+1),
			cx + half*8,	cy + half*(2*y+1),	0);
	}

	//shu
	for(x=-4;x<5;x++)
	{
		line(	cx + x*half*2,	cy - half*9,
			cx + x*half*2,	cy - half*1,	0);
		line(	cx + x*half*2,	cy + half*9,
			cx + x*half*2,	cy + half*1,	0);
	}

	//pie,na
	line(	cx - half*2,	cy - half*9,
		cx + half*2,	cy - half*5,	0);
	line(	cx + half*2,	cy - half*9,
		cx - half*2,	cy - half*5,	0);
	line(	cx - half*2,	cy + half*9,
		cx + half*2,	cy + half*5,	0);
	line(	cx + half*2,	cy + half*9,
		cx - half*2,	cy + half*5,	0);
	//chess
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			//>0x61
			if(data[y][x] >= 'a')
			{
				circlebody(
					cx + (2*x-8)*half,
					cy + (2*y-9)*half,
					half,
					0
				);
			}

			//>0x41
			else if(data[y][x] >= 'A')
			{
				circlebody(
					cx + (2*x-8)*half,
					cy + (2*y-9)*half,
					half,
					red
				);
			}
		}//forx
	}//fory
}




void xiangqi_write(QWORD type,QWORD key)
{
	char val;
	int x,y;
	int width = haha->width;
	int height = haha->height;
	int min = (width<height) ? width:height;

	if(type==0x7466656C207A7978)
	{
		x=key&0xffff;
		y=(key>>16)&0xffff;
		say("%d,%d\n",x,y);

		turn++;
	}
}




static void xiangqi_list()
{
}
static void xiangqi_choose()
{
}




static void xiangqi_start()
{
	int j;
	char* p=(char*)data;

	turn=0;
	for(j=0;j<90;j++)p[j]=0;




	//(red)JU,MA,XIANG,SHI,JIANG
	for(j=0;j<6;j++)
	{
		data[0][ j ]='A' + j;
		data[0][8-j]='A' + j;
	}
	//(red)SOLDIER
	for(j=0;j<5;j++)data[3][j*2]='S';
	//(red)PAO
	data[2][1]='Z';
	data[2][7]='Z';




	//(black)ju,ma,xiang,shi,jiang
	for(j=0;j<6;j++)
	{
		data[9][ j ]='a' + j;
		data[9][8-j]='a' + j;
	}
	//(black)soldier
	for(j=0;j<5;j++)data[6][j*2]='s';
	//(black)pao
	data[7][1]='z';
	data[7][7]='z';
}
static void xiangqi_stop()
{
}




void xiangqi_init(char* base,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0x776f646e6977;		//'window'
	this[1]=0x6971676e616978;	//'xiangqi'

	this[10]=(QWORD)xiangqi_start;
	this[11]=(QWORD)xiangqi_stop;
	this[12]=(QWORD)xiangqi_list;
	this[13]=(QWORD)xiangqi_choose;
	this[14]=(QWORD)xiangqi_read;
	this[15]=(QWORD)xiangqi_write;
}
void xiangqi_kill()
{
}
