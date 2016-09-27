#define u64 unsigned long long
#define u32 unsigned int
//
void printstring(int x, int y, int size, char* str, u32 fgcolor, u32 bgcolor);
void backgroundcolor();
//
double calculator(char* postfix, u64 x, u64 y);
void postfix2binarytree(char* postfix,void* out);
void infix2postfix(char* infix,char* postfix);
//
void double2decimalstring(double,char*);
//
void printmemory(char*,int);
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
static char infix[128];
static char postfix[128];
static char result[128];
//
static char buffer[128];
static int count=0;




static void calculator_read_html()
{
}
static void calculator_read_pixel()
{
	backgroundcolor(0);
	printstring(0, 0, 4, buffer, 0xffffffff, 0);
	printstring(0, 64, 4, infix, 0xffffffff, 0);
	printstring(0, 128, 4, postfix, 0xffffffff, 0);
	printstring(0, 192, 4, result, 0xffffffff, 0);
}
static void calculator_read_text()
{
}
static void calculator_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		calculator_read_text();
	}

	//html
	else if( ( (haha->pixelformat)&0xffffffff) == 0x6c6d7468)
	{
		calculator_read_html();
	}

	//pixel
	else
	{
		calculator_read_pixel();
	}
}




static void calculator_write(u64* who, u64* what, u64* value)
{
	u32 type = *(u32*)what;
	u32 key = *(u32*)value;
	double final;

	if(type==0x72616863)	       //'char'
	{
		if(key==0x8)		    //backspace
		{
			if(count > 0)count--;
			buffer[count] = 0x20;
		}
		else if(key==0xd)	       //enter
		{
			//检查buffer，然后给infix
			say("buffer:%s\n",buffer);

			//清空输入区
			for(count=0;count<127;count++)
			{
				infix[count] = buffer[count];
				buffer[count] = 0x20;
			}
			count=0;

			say("infix2postfix:%s\n",postfix);
			infix2postfix(infix,postfix);

			say("postfix2binarytree......\n");
			final = calculator(postfix, 0, 0);
			double2decimalstring(final, result);
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
	for(j=0;j<128;j++)buffer[j]=0;
	count = 0;
}
static void calculator_stop()
{
}
void calculator_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x6c6f6f74;
	this[1] = 0x636c6163;

	this[10]=(u64)calculator_start;
	this[11]=(u64)calculator_stop;
	this[12]=(u64)calculator_list;
	this[13]=(u64)calculator_change;
	this[14]=(u64)calculator_read;
	this[15]=(u64)calculator_write;
}
void calculator_delete()
{
}
