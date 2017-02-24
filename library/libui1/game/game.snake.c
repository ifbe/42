#define u64 unsigned long long 
#define u32 unsigned int
//
void line(
	int x1, int y1, int x2, int y2, u32 color);
void rectbody(
	int x1, int y1, int x2, int y2, u32 color);
void rectframe(
	int x1, int y1, int x2, int y2, u32 color);
void rect(
	int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolor);
void backgroundcolor(
	u64, u64, u64, u64,
	u32);
//
u32 getrandom();
int diary(char*,int,char*,...);
void say(char*,...);




static struct temp{
	u64 type;
	u64 id;
	u64 start;
	u64 end;

	u64 buffer;
	u64 format;
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
static int worldwidth;
static int worldheight;
static int foodx;
static int foody;
static int score=0;
static int die=0;




void snake_read_pixel()
{
	//create screen
	int j;
	int width = haha->width;
	int height = haha->height;

	if(die == 1)
	{
		line(0, 0, width-1, height-1, 0xffffffff);
		line(width-1, 0, 0, height-1, 0xffffffff);
	}

	//shadow
	if( (a.x>=0) && (a.y>=0) )
	{
		rectbody(
			a.x * width/worldwidth,
			a.y * height/worldheight,
			(a.x+1) * width/worldwidth,
			(a.y+1) * height/worldheight,
			0xf
		);
	}

	//snake
	j=0;
	while(1)
	{
		rect(
			snake[j].x * width/worldwidth,
			snake[j].y * height/worldheight,
			(snake[j].x+1) * width/worldwidth,
			(snake[j].y+1) * height/worldheight,
			0xffffffff,
			0
		);

		j++;
		if(j>=len)break;
	}

	//food
	rect(
		foodx * width/worldwidth,
		foody * height/worldheight,
		(foodx+1) * width/worldwidth,
		(foody+1) * height/worldheight,
		0xff00,
		0
	);
}




void snake_read_text()
{
	int j,t;
	int width=haha->width;
	int height=haha->height;
	char* p = (char*)(haha->buffer);
	for(j=0;j<width*height*4;j++)p[j] = 0;

	j=0;
	while(1)
	{
		t = snake[j].x + snake[j].y * width;
		p[t<<2]='#';

		j++;
		if(j>=len)break;
	}

	t = foodx + foody*width;
	p[t<<2] = '@';
}



static int htmlcubie(char* p, u32 color, int x, int y)
{
	return diary(
		p, 0x1000,
		"<div class=\"rect\" style=\""
		"left:%.2f%;"
		"top:%.2f%;"
		"background:#%06x;"
		"\"></div>",
		x*3.1, y*3.1, color
	);
}
void snake_read_html()
{
	int j = 0;
	char* p = (char*)(haha->buffer);

	*(u32*)p = 0x6c6d7468;
	p += 0x1000;

	if(die == 1)
	{
		diary(p, 0x1000, "boooooooooooooooom");
		return;
	}

	p += diary(
		p, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"width:3.1%;"
		"height:3.1%;"
		"}"
		"</style>"
	);
	while(1)
	{
		p += htmlcubie(p, 0xffffff, snake[j].x, snake[j].y);

		j++;
		if(j>=len)break;
	}
 
	htmlcubie(p, 0xff00, foodx, foody);
}




void snake_read()
{
	u32 temp = (haha->format)&0xffffffff;

	//text
	if(temp == 0x74786574)
	{
		snake_read_text();
	}

	//html
	else if(temp == 0x6c6d7468)
	{
		snake_read_html();
	}

	//pixel
	else
	{
		snake_read_pixel();
	}
}




void newfood()
{
	u32 temp = (haha->format)&0xffffffff;
	if(temp == 0x6c6d7468)
	{
		worldwidth = 32;
		worldheight = 32;
	}
	else if(temp == 0x74786574)
	{
		worldwidth = haha->width;
		worldheight = haha->height;
	}
	else
	{
		worldwidth = (haha->width)/32;
		worldheight = (haha->height)/32;
	}

	foodx=getrandom() % worldwidth;
	foody=getrandom() % worldheight;
}
void snake_write(u64* who, u64* aaaa,u64* bbbb)
{
	int j;
	u64 type = *aaaa;
	u64 key = *bbbb;
	if(die==1)return;

	if(type==0x72616863)
	{
		if(key=='a'){type=0x64626b;key=0x25;}
		else if(key=='w'){type=0x64626b;key=0x26;}
		else if(key=='d'){type=0x64626b;key=0x27;}
		else if(key=='s'){type=0x64626b;key=0x28;}
		else if(key==' '){type=0x2d6d;}
	}

	else if(type==0x2d6d)
	{
		if(direction==1){type=0x64626b;key=0x25;}
		else if(direction==2){type=0x64626b;key=0x27;}
		else if(direction==3){type=0x64626b;key=0x26;}
		else if(direction==4){type=0x64626b;key=0x28;}
		else return;
	}

	if(type==0x64626b)
	{
		if(key==0x25)
		{
			if( (snake[0].x-1 == snake[1].x) && (snake[0].y == snake[1].y) )return;
			if( (snake[0].x-1) < 0){die=1;return;}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].x--;
			direction=1;
		}
		else if(key==0x27)
		{
			if(snake[0].x+1 == snake[1].x)
			{
				if(snake[0].y == snake[1].y)return;
			}
			if( (snake[0].x+1) >= worldwidth){die=1;return;}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].x++;
			direction=2;
		}
		else if(key==0x26)
		{
			if( (snake[0].x == snake[1].x) && (snake[0].y-1 == snake[1].y) )return;
			if( (snake[0].y-1) < 0){die=1;return;}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].y--;
			direction=3;
		}
		else if(key==0x28)
		{
			if(snake[0].x == snake[1].x)
			{
				if(snake[0].y+1 == snake[1].y)return;
			}
			if( (snake[0].y+1) >= worldheight){die=1;return;}

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

		if(snake[j].x==snake[0].x)
		{
			if(snake[j].y==snake[0].y)
			{
				die=1;
				return;
			}
		}
	}

	//吃到食物
	if(snake[0].x==foodx)
	{
		if(snake[0].y==foody)
		{
			snake[j].x = a.x;
			snake[j].y = a.y;
			len++;
			newfood();
		}
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
	backgroundcolor(
		haha->buffer, 0, haha->width, haha->height,
		0
	);

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
