#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void printascii(void*,
	int x, int y, int size,
	char ch, u32 fg, u32 bg);
void string(void*,
	int x,int y,void* str);
void backgroundcolor(void*,
	u32);
//
void charactercommand(u8*);
void arterywrite(u8*);
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








static void background4(struct window* win)
{
	//用指定颜色清屏
	int width,height;
	u32 x,y,color;
	u32* palette = (u32*)(win->buf1);
	backgroundcolor(win, 0);

	//输入框颜色
	for(y=height-16;y<height;y++)
	{
		for(x=0;x<width/2;x++)
		{
			color=(x/2)*0x01010101;
			palette[width*y + x]=color;
			palette[width*y + width -x -1]=color;
		}
	}

	//滚动框颜色
	for(y=0;y<height/2;y++)
	{
		color = y*0xff/height;//0x44444488;

		for(x=width-16;x<width;x++)
		{
			palette[width*y + x] = color;
		}
		for(x=width-16;x<width;x++)
		{
			palette[(height-1-y)*width + x] = color;
		}
	}
}
static void printposition(struct window* win, int start,int count,int max)
{
	//位置
	int x,y;
	int w = win->w;
	int h = win->h;
	u32* palette = (u32*)(win->buf1);

	if(max<0x80*45)return;

	//显示区大小/总大小
	u64 top = (win->h)*start/max;
	u64 bottom = (win->h)*(start+0x80*count)/max;//temp变量=max
	say("printposition:%x,%x\n",top,bottom);

	for(y=top;y<bottom;y++)
	{
		for(x=w-16+4;x<w-4;x++)
		{
			palette[w*y + x] = 0x01234567;
		}
	}
}
static void printstdout(struct window* win)
{
	u8 ch;
	u32 pos;
	int x,y;
	int w = (win->w)/8;
	int h = (win->h)/16;

	if(w>0x80)x=0x80;
	pos = *(u32*)(output+0x100000-16);
	pos = pos - (pos%0x80);

	if(pos < h*0x80)pos = 0;
	else pos -= h*0x80;

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			ch = output[pos + (y*0x80) + x];
			if( (ch==0) | (ch=='\n') )break;

			printascii(win,
				x*8, y*16, 1,
				ch, 0xffffff, 0
			);
		}
	}
}
static void printstdin(struct window* win, int count)
{
	string(win, 0, count*16, "[user@42]");
	string(win, 9*8, count*16, input);
}
void xt100_read(struct window* win)
{
	background4(win);
	printstdout(win);
}








void xt100_create(void* addr)
{
	input = addr + 0x400000;
	output = addr + 0x500000;
}
void console_delete()
{
}
