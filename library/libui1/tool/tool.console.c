#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void string(int x,int y,char* str);
void backgroundcolor(u32);
//
void characterchoose(char*);
void command(char* in);
//
int cmp(char* first,char* second);
void say(char*,...);





//palette
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

//log位置
static char* logbuf=0;
static u64 backward=0;

//键盘输入
static u8 buffer[128];//键盘输入专用
static int bufcount=0;








static void background4()
{
	//用指定颜色清屏
	int width,height;
	u32 x,y,color;
	u32* palette = (u32*)(haha->pixelbuffer);
	backgroundcolor(0);

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
static void printposition(int start,int count,int max)
{
	//位置
	int x,y;
	u32* palette = (u32*)(haha->pixelbuffer);

	if(max<0x80*45)return;

	//显示区大小/总大小
	u64 top=(haha->height)*start/max;
	u64 bottom=(haha->height)*(start+0x80*count)/max;//temp变量=max
	say("printposition:%x,%x\n",top,bottom);

	for(y=top;y<bottom;y++)
	{
		for(x=(haha->width)-16+4;x<(haha->width)-4;x++)
		{
			palette[(haha->width)*y + x] = 0x01234567;
		}
	}
}
static void printstdout(int start,int count)
{
	//总共38行，必须保证start>=0x80*行数
	int x,y;
	char* p=logbuf+start;
	//say("printstdout:%d,%d\n",start,count);
	for(y=0;y<count;y++)
	{
		//string(0 , y*16 , p + y * 0x80);
		string(0, y*16, "haha");
	}
}
static void printstdin(int count)
{
	string(0, count*16, "[user@42]");
	string(9*8, count*16, buffer);
}








static void console_write(u64* who, u64* a, u64* b)
{
	u64 type = *a;
	u64 key = *b;

	if(type==0x72616863)		//'char'
	{
		if(key==0x8)		//backspace
		{
			if(bufcount!=0)
			{
				bufcount--;
				buffer[bufcount]=0;
			}
		}
		else if(key==0xd)		//回车
		{
			if(cmp( buffer , "exit" ) == 0)
			{
				characterchoose(0);
				return;
			}
			else
			{
				say("%s\n",buffer);
				command(buffer);

				for(bufcount=0;bufcount<127;bufcount++) buffer[bufcount]=0;
				bufcount=0;
			}
		}
		else
		{
			if(bufcount<0x80)
			{
				buffer[bufcount]=key&0xff;
				bufcount++;
			}
		}
	}
	else if(type == 0x2d6d)
	{
		//'xyz fron'
		if((key>>48) == 4)
		{
			u32 enqueue=*(u32*)(logbuf+0xffff0);
			if(enqueue>=0x80*40)		//大于一页才准上翻
			{
				if(backward<enqueue-0x80*40)	//没到头才能继续
				{
					backward+=0x80;
				}
			}
		}
		if((key>>48) == 5)
		{
			if(backward>=0x80)backward-=0x80;
		}
	}
}
static void console_read()
{
	//显示哪儿开始的一块
	int count=(haha->height)/16 - 1;
	int enqueue=*(u32*)(logbuf+0xffff0);

	int start=enqueue-(count*0x80)-backward;//代表末尾位置而不是开头
	if( start<0 )start=0;

	background4();
	printposition(start,count,enqueue);
	printstdout(start,count);
	printstdin(count);
}
static void console_into()
{
}
static void console_list()
{
}








static void console_start()
{
	backgroundcolor(0);
}
static void console_stop()
{
}
void console_create(char* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x6c6f6f74;
	this[1] = 0x656c6f736e6f63;

	this[10]=(u64)console_start;
	this[11]=(u64)console_stop;
	this[12]=(u64)console_list;
	this[13]=(u64)console_into;
	this[14]=(u64)console_read;
	this[15]=(u64)console_write;

	//
	logbuf=base+0x300000;
}
void console_delete()
{
}
