#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void rectbody(void*,
	int x1, int y1,
	int x2, int y2,
	u32 body);
void drawascii(
	void*, char ch, int size,
	int x, int y, u32 fg, u32 bg);
void drawstring(
	void*, void* str, int size,
	int x, int y, u32 fg, u32 bg);
//
void cli_read();
void cli_write();
//
void say(void*, ...);




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
static char* input = 0;
static char* output = 0;




static void printstdout(struct window* win,
	int x0, int y0, int x1, int y1)
{
	u8 ch;
	u32 pos;
	int x,y;
	int h = win->h;
	int w = win->w;
	rectbody(win,
		w*x0/0x10000, h*y0/0x10000,
		w*x1/0x10000, h*y1/0x10000,
		0
	);

	h >>= 4;
	w >>= 3;
	if(w>0x80)w = 0x80;

	pos = *(u32*)(output+0x100000-16);
	pos = pos - (pos%0x80);
	if(pos < h*0x80)pos = 0;
	else pos -= (h-1)*0x80;

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			ch = output[pos + (y*0x80) + x];
			if( (ch==0) | (ch=='\n') )break;

			drawascii(
				win, ch, 1,
				x*8, (y*16)+(h%16), 0xffffff, 0
			);
		}
	}
}
static void printstdin(struct window* win,
	int x0, int y0, int x1, int y1)
{
	int w = win->w;
	int h = win->h;
	drawstring(
		win, "[user@42]", 1,
		0, 0, 0, 0xffffffff);
	drawstring(
		win, input, 1,
		9<<3, 0, 0, 0xffffffff);
}
static void printposition(struct window* win,
	int x0, int y0, int x1, int y1)
{
	//位置
	int x,y,pos;
	int w = win->w;
	int h = win->h;
	u32* palette = (u32*)(win->buf1);

	//
	pos = *(u32*)(output+0x100000-16);
	pos = pos - (pos%0x80);
	if(pos < (h/16)*0x80)pos = 0;
	else pos -= ((h/16)-1)*0x80;

	//显示区大小/总大小
	u64 top = h * pos / (pos+(h/16)*0x80);
	u64 bottom = h;

	//
	for(y=0;y<h;y++)
	{
		for(x=w-16;x<w;x++)
		{
			palette[w*y + x] = 0xff888888;
		}
	}
	for(y=top;y<bottom;y++)
	{
		for(x=w-16+1;x<w-1;x++)
		{
			palette[w*y + x] = 0xffffffff;
		}
	}
}
void draw_vt100(struct window* win,
	int x0, int y0, int x1, int y1)
{
	//backgroundcolor(win, 0);
	printstdout(win, x0, y0, x1, y1);
	printstdin(win, x0, y0, x1, y1);
	printposition(win, x0, y0, x1, y1);
}




void vt100_create(void* addr)
{
	input = addr + 0x400000;
	output = addr + 0x500000;
}
void vt100_delete()
{
}
