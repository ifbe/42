#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void drawstring(
	void*, void* str, int size,
	int x, int y, u32 fg, u32 bg);
void drawascii(
	void*, u8 ch, int size,
	int x, int y, u32 fg, u32 bg);
void rect(void*,
	int x1, int y1,
	int x2, int y2,
	u32 bc, u32 fc);
//
double calculator(char* postfix, u64 x, u64 y);
void postfix2binarytree(char* postfix,void* out);
void infix2postfix(char* infix,char* postfix);
void double2decstr(double,char*);
//
void printmemory(char*,int);
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
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	u8 data[0xc0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
//
static char infix[128];
static char postfix[128];
static char result[128];
//
static char buffer[128];
static int count=0;
//
static char table[4][8] = {
'0', '1', '2', '3', '+', '-', '*', '/',
'4', '5', '6', '7', '^', '%', '!', ' ',
'8', '9', 'a', 'b', '<', '>', '(', ')',
'c', 'd', 'e', 'f', '.', ' ', '`', '=',
};




static void calculator_read_pixel(struct window* win, struct player* pl)
{
	u32 fg;
	int x,y;
	int w8 = (win->w)/8;
	int h8 = (win->h)/8;

	rect(win,
		0, 0,
		w8*8-1, h8*4-1,
		0, 0xff00
	);
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			if(x<4)fg = y*0x10 + x*0x100000;
			else fg = x*0x10 + y*0x100000;

			rect(win,
				w8*x, h8*(y+4),
				w8*(x+1), h8*(y+5),
				fg, 0xffffffff
			);
			drawascii(
				win, table[y][x], 4,
				w8*x, h8*(y+4), 0xffffffff, 0
			);
		}
	}
	drawstring(
		win, buffer, 2,
		16, 16, 0xffffffff, 0xff000000
	);
	drawstring(
		win, infix, 2,
		16, 16+32, 0xffffffff, 0xff000000
	);
	drawstring(
		win, postfix, 2,
		16, 16+64, 0xffffffff, 0xff000000
	);
	drawstring(
		win, result, 2,
		16, 16+96, 0xffffffff, 0xff000000
	);
}
static void calculator_read_html(struct window* win, struct player* pl)
{
}
static void calculator_read_text(struct window* win, struct player* pl)
{
}
static void calculator_read_cli(struct window* win, struct player* pl)
{
	say("buffer:%s\n", infix);
	say("postfix:%s\n", postfix);
	say("result:%s\n", result);
}
static void calculator_read(struct window* win, struct player* pl)
{
	u64 fmt = win->fmt;

	//cli
	if(win->dim == 1)calculator_read_cli(win, pl);

	//text
	if(fmt == 0x74786574)calculator_read_text(win, pl);

	//html
	else if(fmt == 0x6c6d7468)calculator_read_html(win, pl);

	//pixel
	else calculator_read_pixel(win, pl);
}




static void calculator_write(struct event* ev)
{
	double final;
	int x,y,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 0x2d70)
	{
		x = key&0xffff;
		y = (key>>16)&0xffff;

		x = x*8 / 512;
		y = y*8 / 512;
		if(y < 4)return;

		type = 0x72616863;
		key = table[y-4][x];

		if(key == '`')key = 0x8;
		if(key == '=')key = 0xd;
	}

	if(type == 0x72616863)	       //'char'
	{
		if(key==0x8)		    //backspace
		{
			if(count <= 0)return;

			count--;
			buffer[count] = 0x20;
		}
		else if(key==0xd)	       //enter
		{
			//清空输入区
			for(ret=0;ret<count;ret++)
			{
				infix[ret] = buffer[ret];
				buffer[ret] = 0x20;
			}
			infix[count] = 0;
			count=0;
			say("buffer:%s\n", infix);

			infix2postfix(infix, postfix);
			say("postfix:%s\n", postfix);

			final = calculator(postfix, 0, 0);
			double2decstr(final, result);
			say("result:%s\n", result);
		}
		else
		{
			if(count<128)
			{
				buffer[count] = key;
				count++;
			}
		}
	}
}




static void calculator_list()
{
}
static void calculator_change()
{
}
static void calculator_start()
{
	int j;
	buffer[0] = '1';
	buffer[1] = '+';
	buffer[2] = '2';
	buffer[3] = 0;
	count = 3;
}
static void calculator_stop()
{
}
void calculator_create(void* base,void* addr)
{
	struct player* p = addr;
	p->type = 0x6c6f6f74;
	p->name = 0x636c6163;

	p->start = (u64)calculator_start;
	p->stop = (u64)calculator_stop;
	p->list = (u64)calculator_list;
	p->choose = (u64)calculator_change;
	p->read = (u64)calculator_read;
	p->write = (u64)calculator_write;
}
void calculator_delete()
{
}
