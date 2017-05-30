#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void backgroundcolor(
	void*, u32);
void drawascii(
	void*, char ch, int size,
	int x, int y, u32 fg, u32 bg);
void drawstring(
	void*, void* str, int size,
	int x, int y, u32 fg, u32 bg);
//
void arterywrite(void*);
void arteryread();
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
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




static void printstdout(struct window* win)
{
	u8 ch;
	u32 pos;
	int x,y;
	int h = (win->h)/16;
	int w = (win->w)/8;
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
static void printstdin(struct window* win)
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
static void printposition(struct window* win)
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
void xt100_read(struct window* win)
{
	arteryread();
	if(win->fmt == 0x696c63)return;

	backgroundcolor(win, 0);
	printstdout(win);
	printstdin(win);
	printposition(win);
}




void xt100_write(u64* p)
{
	int j;
	int* in = (void*)(input+0xffff0);
	if(p[2] != 0)return;
	if(p[1] != 0x72616863)return;

	arterywrite(p);

	j = *(u8*)p;
	if(j == 0x8)		//backspace
	{
		if(*in <= 0)return;
		(*in)--;
		input[*in] = 0;
	}
	else if(j == 0xa)	//enter
	{
		for(j=0;j<0x80;j++)input[j] = 0;
		*in = 0;
	}
	else
	{
		input[*in] = j;
		(*in)++;
	}
}




void xt100_create(void* addr)
{
	input = addr + 0x400000;
	output = addr + 0x500000;
}
void console_delete()
{
}
