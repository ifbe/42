#define u64 unsigned long long 
#define u32 unsigned int
//
void line(       int x1,int y1,int x2,int y2, u32 color);
void circlebody( int x, int y, int r, u32 color);
void circleframe(int x, int y, int r, u32 color);
void backgroundcolor(u32);
u32 getrandom();
//
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
static char data[9];




void ooxx_read()
{
	int x,y;
	int width = haha->width;
	int height = haha->height;
	int min = (width<height) ? width:height;

	backgroundcolor(0);
        line(min/16,    min  /  3,      min *15 /16,    min   /   3,    0xffffffff);
        line(min/16,    min *2 /3,      min *15 /16,    min  *2  /3,    0xffffffff);
        line(min/3,     min  / 16,      min   /   3,    min *15 /16,    0xffffffff);
        line(min*2/3,   min  / 16,      min  *2  /3,    min *16 /16,    0xffffffff);

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			if(data[3*y + x] == 'o')
			{
				circleframe(
					(2*x+1)*min/6,
					(2*y+1)*min/6,
					min/12,
					0xff
				);
			}
			else if(data[3*y + x] == 'x')
			{
				line(
					(4*x+1)*min/12,
					(4*y+1)*min/12,
					(4*x+3)*min/12,
					(4*y+3)*min/12,
					0xff0000
				);
				line(
					(4*x+3)*min/12,
					(4*y+1)*min/12,
					(4*x+1)*min/12,
					(4*y+3)*min/12,
					0xff0000
				);
			}
		}//forx
	}//fory
}




void ooxx_write(u64* type,u64* key)
{
	char val;
	int x,y;
	int width = haha->width;
	int height = haha->height;
	int min = (width<height) ? width:height;

	if(*type==0x7466656C207A7978)
	{
		x=(*key) & 0xffff;
		y=( (*key) >> 16 ) & 0xffff;
		if(x>min)return;
		if(y>min)return;

		x = x*3/min;
		y = y*3/min;
say("%d,%d\n",x,y);
		if(data[y*3 + x] != 0)return;

		if((turn&0x1) == 0x1)val='o';
		else val='x';

		data[y*3 + x] = val;
		turn++;
	}
}




static void ooxx_list()
{
}
static void ooxx_choose()
{
}




static void ooxx_start()
{
	int j;

	turn=0;
	for(j=0;j<9;j++)data[j]=0;
}
static void ooxx_stop()
{
}




void ooxx_create(char* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0]=0x776f646e6977;		//'window'
	this[1]=0x78786f6f;		//'ooxx'

	this[10]=(u64)ooxx_start;
	this[11]=(u64)ooxx_stop;
	this[12]=(u64)ooxx_list;
	this[13]=(u64)ooxx_choose;
	this[14]=(u64)ooxx_read;
	this[15]=(u64)ooxx_write;
}
void ooxx_delete()
{
}
