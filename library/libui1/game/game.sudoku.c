#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void printascii(
	int x, int y, int size, u8 data, u32 fg, u32 bg);
void printdecimal(
	int x, int y, int size, int data, u32 fg, u32 bg);
void rectbody(
	int x1, int y1, int x2, int y2, u32 color);
void rectframe(
	int x1, int y1, int x2, int y2, u32 color);
void rect(
	int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolr);
void line(
	int, int, int, int, u32);
//
int data2decstr(u64 data,u8* string);
u32 getrandom();
//
int printmemory(char*,int);
int diary(char*, int, char*, ...);
int say(char*,...);




//
static struct temp{
	u64 type;
	u64 id;
	u64 start;
	u64 end;

	u64 buffer;
	u64 format;
	u64 width;
	u64 height;

	u64 f0;
	u64 f1;
	u64 f2;
	u64 f3;

	u64 f4;
	u64 f5;
	u64 f6;
	u64 f7;
}*haha;
//
static int px,py;
static char table[9][9];
//
static char* buffer;




//
int sudoku_possiable(int px, int py, char* p)
{
	int x,y,ret;
	int basex, basey;
	for(x=0;x<10;x++)p[x] = x;

	//heng
	for(x=0;x<9;x++)
	{
		ret = table[py][x];
		if( (ret >= 1) && (ret <= 9) )
		{
			p[ret] = 0;
		}
	}

	//shu
	for(y=0;y<9;y++)
	{
		ret = table[y][px];
		if( (ret >= 1) && (ret <= 9) )
		{
			p[ret] = 0;
		}
	}

	//quan
	basex = px - (px%3);
	basey = py - (py%3);
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			ret = table[basey+y][basex+x];
			if( (ret >= 1) && (ret <= 9) )
			{
				p[ret] = 0;
			}
		}
	}

//say("%d,%d:	",px,py);
	//sort
	ret=0;
	for(x=1;x<10;x++)
	{
		if(p[x] == 0)continue;

		p[ret] = p[x];
//say("%d ",p[ret]);

		ret++;
	}
//say("\n");

	p[ret] = 0;
	return ret;
}
int sudoku_random(char* p,int count)
{
	int k,ret;
	if(count == 0)return 0;

	k = getrandom()%count;
	ret = p[k];
	if(count > 1)
	{
		p[k] = p[count-1];
		p[count-1] = 0;
	}

	return ret;
}
void sudoku_solve()
{
	int x,y,t,ret;
	int mode,count,timeout;

/*
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			ret = sudoku_possiable(x,y);
			table[y][x] = sudoku_random(possiable);
		}
	}
*/
	t = 0;		//第几个了
	mode = 0;	//进入还是回头
	timeout = 0;
	while(timeout < 9999)
	{
		y = t/9;
		x = t%9;

		//这是头一次进
		if(mode == 0)
		{
			count = sudoku_possiable(x, y, buffer + 10*t);
		}

		//这一次是回头
		else
		{
			for(count=0;count<10;count++)
			{
				if(buffer[10*t + count] == 0)break;
			}
		}

		//只能回头
		if(count == 0)
		{
			if(t == 0)break;	//失败退出

			table[y][x] = 0;
			t--;
			mode = 1;
		}

		//继续回头
		else if( (count == 1) && (mode != 0) )
		{
			if(t == 0)break;	//失败退出

			table[y][x] = 0;
			t--;
			mode = 1;
		}

		//选一个
		else
		{
			ret = sudoku_random(buffer+10*t, count);
			table[y][x] = ret;

			t++;
			mode = 0;

			if(t >= 81)break;	//成功退出
		}

		timeout++;
	}
}




static void sudoku_read_text()
{
	int x,y,j,k,ret,color;
	int width=haha->width;
	int height=haha->height;
	char* p = (char*)(haha->buffer);

	for(x=0;x<width*height*4;x++)p[x] = 0;
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if(table[y][x] == 0)continue;

			//position
			ret = (3*y+1)*width + 6*x + 2;
			ret <<= 2;

			//color
			if( (px == x)&&(py == y) )color = 1;
			else if( ((x>2)&&(x<6)) && ((y<3)|(y>5)) )color = 2;
			else if( ((y>2)&&(y<6)) && ((x<3)|(x>5)) )color = 2;
			else color = 4;
			for(j=-1;j<=1;j++)
			{
				for(k=-2;k<=3;k++)
				{
					p[ret +(j*width*4) +(k*4) +3] = color;
				}
			}

			//data
			p[ret] = table[y][x] + 0x30;
		}
	}
}
static void sudoku_read_html()
{
}
static void sudoku_read_pixel()
{
	int x,y;
	int w = haha->width;
	int h = haha->height;

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			rect(
				x*w/9,	 y*h/9,
				(x+1)*w/9, (y+1)*h/9,
				0x888888,	  0
			);

			if(table[y][x] != 0)
			{
				printdecimal(
					x*w/9,   y*h/9,
					4, table[y][x],
					0,	   0
				);
			}
		}
	}
	rectbody(	0, ( h/3 )-2,	 w, ( h/3 )+2, 0);
	rectbody(	0, (h*2/3)-2,	 w, (h*2/3)+2, 0);
	rectbody(( w/3 )-2,	 0, ( w/3 )+2,	 h, 0);
	rectbody((w*2/3)-2,	 0, (w*2/3)+2,	 h, 0);
}
static void sudoku_read()
{
	u32 temp = (haha->format)&0xffffffff;

	//text
	if(temp == 0x74786574)
	{
		sudoku_read_text();
	}

	//html
	else if(temp == 0x6c6d7468)
	{
		sudoku_read_html();
	}

	//pixel
	else
	{
		sudoku_read_pixel();
	}
}




static void sudoku_write(u64* who, u64* what, u64* key)
{
	if(*what == 0x64626b)
	{
		if(*key == 0x25)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(*key == 0x26)   //up
		{
			if(py<1)return;
			py--;
		}
		else if(*key == 0x27)   //right
		{
			if(px<0)return;
			if(px>=8)return;
			px++;
		}
		else if(*key == 0x28)   //down
		{
			if(py<0)return;
			if(py>=8)return;
			py++;
		}
	}
}








static void sudoku_list()
{
}
static void sudoku_choose()
{
}








static void sudoku_start()
{
	int x,y;
	px = py = 0;

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			table[y][x] = 0;
		}
	}
	sudoku_solve();
}
static void sudoku_stop()
{
}
void sudoku_create(void* base,void* addr)
{
	buffer = base+0x300000;
	haha = addr;

	haha->type = 0x656d6167;
	haha->id = 0x756b6f647573;
	haha->f2 = (u64)sudoku_start;
	haha->f3 = (u64)sudoku_stop;
	haha->f4 = (u64)sudoku_list;
	haha->f5 = (u64)sudoku_choose;
	haha->f6 = (u64)sudoku_read;
	haha->f7 = (u64)sudoku_write;
}
void sudoku_delete()
{
}
