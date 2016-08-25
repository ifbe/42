#define u64 unsigned long long 
#define u32 unsigned int
//
void line(int x1, int y1, int x2, int y2, u32 color);
void rectbody( int x1, int y1, int x2, int y2, u32 color);
void rectframe(int x1, int y1, int x2, int y2, u32 color);
void rect(     int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolor);
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

struct hehe{
	int x;
	int y;
};
static struct hehe* snake;
static struct hehe a;
static struct hehe b;
//
static int len;
static int direction;
//
static int foodx;
static int foody;
static int score=0;
static int die=0;




void snake_read_text()
{
	char bg;
	int j;
	int width=haha->width;
	int height=haha->height;
	char* p = (char*)(haha->pixelbuffer);

	if(die == 1)bg = 0x30;
	else bg = 0x20;
	for(j=0;j<width*height;j++)p[j]=bg;

	j=0;
	while(1)
	{
		p[snake[j].x + snake[j].y * width]='#';

		j++;
		if(j>=len)break;
	}

	p[foodx + foody*width] = '@';
}
void snake_read_pixel()
{
	//create screen
	int j;

	if(die == 1)
	{
		line(0, 0, (haha->width)-1, (haha->height)-1, 0xffffffff);
		line((haha->width)-1, 0, 0, (haha->height)-1, 0xffffffff);
	}

	//shadow
	if( (a.x>=0) && (a.y>=0) )
	{
		rectbody(
			32*a.x,
			32*a.y,
			32*a.x + 31,
			32*a.y + 31,
			0xf
		);
	}

	//snake
	j=0;
	while(1)
	{
		rect(
			32*snake[j].x,
			32*snake[j].y,
			32*snake[j].x + 31,
			32*snake[j].y + 31,
			0xffffffff,
			0
		);

		j++;
		if(j>=len)break;
	}

	//food
	rect(
		32*foodx,
		32*foody,
		32*foodx + 31,
		32*foody + 31,
		0xff00,
		0
	);
}
void snake_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		snake_read_text();
	}
/*
	//html
	else if()
*/
	//pixel
	else
	{
		snake_read_pixel();
	}
}




void newfood()
{
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		foodx=getrandom() % (haha->width);
		foody=getrandom() % (haha->height);
	}
	else
	{
		foodx=getrandom() % ((haha->width)/32);
		foody=getrandom() % ((haha->height)/32);
	}
}
void snake_write(u64* aaaa,u64* bbbb)
{
	int j;
	u64 type = *aaaa;
	u64 key = *bbbb;
	if(die==1)return;

	if(type==0x7466656C207A7978)
	{
		if(direction==1){type=0x64626b;key=0x25;}
		else if(direction==2){type=0x64626b;key=0x27;}
		else if(direction==3){type=0x64626b;key=0x26;}
		else if(direction==4){type=0x64626b;key=0x28;}
		else return;
	}

	if(type==0x64626b)
	{
		if(key=='a'|key==0x25)
		{
			if( (snake[0].x-1 == snake[1].x) && (snake[0].y == snake[1].y) )return;
			if( (snake[0].x-1) < 0){die=1;return;}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].x--;
			direction=1;
		}
		else if(key=='d'|key==0x27)
		{
			if( (snake[0].x+1 == snake[1].x) && (snake[0].y == snake[1].y) )return;
			if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
			{
				if( (snake[0].x+1) > (haha->width)){die=1;return;}
			}
			else
			{
				if( (snake[0].x+1) > ((haha->width)/32)){die=1;return;}
			}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].x++;
			direction=2;
		}
		else if(key=='w'|key==0x26)
		{
			if( (snake[0].x == snake[1].x) && (snake[0].y-1 == snake[1].y) )return;
			if( (snake[0].y-1) < 0){die=1;return;}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].y--;
			direction=3;
		}
		else if(key=='s'|key==0x28)
		{
			if( (snake[0].x == snake[1].x) && (snake[0].y+1 == snake[1].y) )return;
			if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
			{
				if( (snake[0].y+1) > (haha->height)){die=1;return;}
			}
			else
			{
				if( (snake[0].y+1) > ((haha->height)/32)){die=1;return;}
			}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].y++;
			direction=4;
		}
		else return;
	}
	else return;

	//蛇身往前挪动
	j=0;
	while(1)
	{
		j++;
		if(j >= len)break;

		b.x = snake[j].x;
		b.y = snake[j].y;
		snake[j].x = a.x;
		snake[j].y = a.y;
		a.x = b.x;
		a.y = b.y;

		if( (snake[j].x==snake[0].x) && (snake[j].y==snake[0].y) )
		{
			die=1;
			return;
		}
	}

	//吃到食物
	if(snake[0].x==foodx && snake[0].y==foody)
	{
		snake[j].x = a.x;
		snake[j].y = a.y;
		len++;
		newfood();
	}
}




static void snake_list()
{
}
static void snake_choose()
{
}




static void snake_start()
{
	//1.create
	int x;
	backgroundcolor(0);

	//create food and snake 
	newfood();
	snake[0].x = foodx;
	snake[0].y = foody;
	newfood();

	a.x = -1;
	b.x = -1;

	len=1;
	direction=1;

	die=0;
}
static void snake_stop()
{
}




void snake_create(char* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x656d6167;
	this[1] = 0x656b616e73;

	this[10]=(u64)snake_start;
	this[11]=(u64)snake_stop;
	this[12]=(u64)snake_list;
	this[13]=(u64)snake_choose;
	this[14]=(u64)snake_read;
	this[15]=(u64)snake_write;

	snake=(struct hehe*)(addr+0x300000);
}
void snake_delete()
{
}
