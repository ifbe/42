#define u64 unsigned long long 
#define u32 unsigned int
//
void line(       int x1,int y1,int x2,int y2, u32 color);
void circlebody( int x, int y, int r, u32 color);
void circleframe(int x, int y, int r, u32 color);
void backgroundcolor(u32);
u32 getrandom();
//
int diary(char*,int,char*,...);
void say(char*,...);




static struct temp{
        u64 type;
        u64 id;
        u64 start;
        u64 end;

        u64 pixelbuffer;
        u64 pixelformat;
        u64 width;
        u64 height;
}*haha;

//
static int turn;
static char data[10][9];




void xiangqi_read_pixel()
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
static int htmlcircle(char* p, int x, int y)
{
	u32 textcolor;
	char* hanzi;
	if(data[y][x] >= 'a')textcolor=0;
	else textcolor=0xff0000;

	switch(data[y][x])
	{
		case 'a':hanzi="车";break;
		case 'b':hanzi="马";break;
		case 'c':hanzi="象";break;
		case 'd':hanzi="士";break;
		case 'e':hanzi="将";break;
		case 's':hanzi="兵";break;
		case 'z':hanzi="炮";break;

		case 'A':hanzi="车";break;
		case 'B':hanzi="马";break;
		case 'C':hanzi="相";break;
		case 'D':hanzi="仕";break;
		case 'E':hanzi="帅";break;
		case 'S':hanzi="兵";break;
		case 'Z':hanzi="炮";break;

		default:return 0;
	}
	return diary(
		p, 0x1000,
		"<div class=\"circle\" style=\""
		"left:%d%;"
		"top:%d%;"
		"background:#ffff00;"
		"color:#%06x;"
		"\"><br>%s</div>",
		x*11, y*10,
		textcolor, hanzi
	);
}
static void xiangqi_read_html()
{
	int x,y;
	char* p = (char*)(haha->pixelbuffer) + 0x1000;

	p += diary(
		p, 0x1000,
		"<style type=\"text/css\">"
		".circle{"
		"position:absolute;"
		"border-radius:50%;"
		"width:10%;"
		"height:10%;"
		"text-align:center;"
		"}"
		"</style>"
	);
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			p += htmlcircle(p, x, y);
		}//forx
	}//fory
}
static void xiangqi_read_text()
{
}
static void xiangqi_read()
{
	u32 temp = (haha->pixelformat)&0xffffffff;
	//say("(@2048.read)temp=%x\n",temp);

	//text
	if(temp == 0x74786574)
	{
		xiangqi_read_text();
	}

	//html
	else if(temp == 0x6c6d7468)
	{
		xiangqi_read_html();
	}

	//pixel
	else
	{
		xiangqi_read_pixel();
	}
}




void xiangqi_write(u64* who, u64* what, u64* key)
{
	char val;
	int x,y;
	int width = haha->width;
	int height = haha->height;
	int min = (width<height) ? width:height;

	if(*what == 0x7466656C207A7978)
	{
		x=(*key) & 0xffff;
		y=( (*key) >> 16 ) & 0xffff;
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




	for(j=0;j<=4;j++)
	{
		//(red)JU,MA,XIANG,SHI,JIANG
		data[0][ j ]='A' + j;
		data[0][8-j]='A' + j;
		
		//(black)ju,ma,xiang,shi,jiang
		data[9][ j ]='a' + j;
		data[9][8-j]='a' + j;
	}
	for(j=0;j<5;j++)
	{
		//(red)SOLDIER
		data[3][j*2]='S';

		//(black)soldier
		data[6][j*2]='s';
	}

	//(red)PAO
	data[2][1]='Z';
	data[2][7]='Z';

	//(black)pao
	data[7][1]='z';
	data[7][7]='z';
}
static void xiangqi_stop()
{
}




void xiangqi_create(char* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x656d6167;
	this[1] = 0x6971676e616978;

	this[10]=(u64)xiangqi_start;
	this[11]=(u64)xiangqi_stop;
	this[12]=(u64)xiangqi_list;
	this[13]=(u64)xiangqi_choose;
	this[14]=(u64)xiangqi_read;
	this[15]=(u64)xiangqi_write;
}
void xiangqi_delete()
{
}
