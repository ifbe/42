#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long 
//
void line(void*,
	int x1, int y1, int x2, int y2, u32 color);
void rectbody(void*,
	int x1, int y1, int x2, int y2, u32 color);
void rect(void*,
	int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolor);
u32 getrandom();
int fmt(char*,int,char*,...);
void say(char*,...);




struct player
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};
struct window
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;

	u8 data[0xe0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};

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




void snake_read_pixel(struct window* win)
{
	//create screen
	int j;
	int width = win->w;
	int height = win->h;

	if(die == 1)
	{
		line(win,
			0, 0,
			width-1, height-1,
			0xffffffff
		);
		line(win,
			width-1, 0,
			0, height-1,
			0xffffffff
		);
	}

	//shadow
	if( (a.x>=0) && (a.y>=0) )
	{
		rectbody(win,
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
		rect(win,
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
	rect(win,
		foodx * width/worldwidth,
		foody * height/worldheight,
		(foodx+1) * width/worldwidth,
		(foody+1) * height/worldheight,
		0xff00,
		0
	);
}




void snake_read_text(struct window* win)
{
	int j,t;
	int width = win->w;
	int height = win->h;
	char* p = (char*)(win->buf);
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
	return fmt(
		p, 0x1000,
		"<div class=\"rect\" style=\""
		"left:%.2f%;"
		"top:%.2f%;"
		"background:#%06x;"
		"\"></div>",
		x*3.1, y*3.1, color
	);
}
void snake_read_html(struct window* win)
{
	int j = 0;
	char* p = (char*)(win->buf);

	*(u32*)p = 0x6c6d7468;
	p += 0x1000;

	if(die == 1)
	{
		fmt(p, 0x1000, "boooooooooooooooom");
		return;
	}

	p += fmt(
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




void snake_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		snake_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		snake_read_html(win);
	}

	//pixel
	else
	{
		snake_read_pixel(win);
	}
}




void newfood()
{
	foodx=getrandom() % worldwidth;
	foody=getrandom() % worldheight;
}
void snake_write(struct event* ev)
{
	int j;
	u64 type = ev->what;
	u64 key = ev->why;
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
	worldwidth = 32;
	worldheight = 32;

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
void snake_create(void* base,void* addr)
{
	struct player* p = addr;
	snake=(struct hehe*)(base+0x300000);

	p->type = 0x656d6167;
	p->name = 0x656b616e73;

	p->start = (u64)snake_start;
	p->stop = (u64)snake_stop;
	p->list = (u64)snake_list;
	p->choose = (u64)snake_choose;
	p->read = (u64)snake_read;
	p->write = (u64)snake_write;
}
void snake_delete()
{
}
