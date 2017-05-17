#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdarg.h>
int data2decstr(u64 data, void* str);
int data2hexstr(u64 data, void* str);
int double2decstr(double data, void* str);
void lowlevel_input(void*);
void lowlevel_output(void*, int);




static void* inputqueue;        //stdin
	static u64* incur;			//real position
	static u64* inwin;			//insert position
static void* outputqueue;       //stdout
	static u64* outcur;			//real position
	static u64* outwin;			//display position
static void* journalqueue;      //stderr
	static u64* logcur;			//real position
	static u64* logwin;			//display position
void initsay(char* buf)
{
	inputqueue = buf;
	incur = inputqueue+0x100000-16;
	inwin = inputqueue+0x100000-8;

	outputqueue = buf+0x100000;
	outcur = outputqueue+0x100000-16;
	outwin = outputqueue+0x100000-8;

	journalqueue = buf+0x200000;
	logcur = journalqueue+0x100000-16;
	logwin = journalqueue+0x100000-8;
}




int convert(char* buf, int len, char* str, int align, va_list arg)
{
	int j,k;
	u64 _x;
    char* _s;
    double _f;

	j = k = 0;
	while(1)
	{
		if(k >= len)return len;
		if(str[j] == 0)return k;

		if(str[j] == '%')
		{
			if(str[j+1] == 'c')
			{
				buf[k] = va_arg(arg,int);
				k++;
				j += 2;
				continue;
			}
			else if(str[j+1] == 'd')
			{
				_x = va_arg(arg, u64);
				k += data2decstr(_x, buf+k);

				j += 2;
				continue;
			}
			else if(str[j+1] == 'f')
			{
				_f = va_arg(arg, double);
				k += double2decstr(_f, buf+k);

				j += 2;
				continue;
			}
			else if(str[j+1] == 'x')
			{
				_x = va_arg(arg, u64);
				k += data2hexstr(_x, buf+k);

				j += 2;
				continue;
			}
			else if(str[j+1] == 's')
			{
				_s = va_arg(arg, char*);
				while(1)
				{
					if(k >= len)return len;
					if(*_s == 0)break;
					buf[k] = *_s;
					_s++;
					k++;
				}

				j += 2;
				continue;
			}
		}

		if(str[j] == '\n')
		{
			buf[k] = '\n';
			k = (k+0x80) - ((align+k)%0x80);
		}
		else if(str[j] == '	')
		{
			do
			{
				buf[k] = 0x20;
				k++;
			}while( ( (align+k) % 8 ) != 0);
		}
		else
		{
			buf[k] = str[j];
			k++;
		}

		j++;
	}
	return k;
}
void printout(int cur, int len)
{
	int j,k;
	u8* p;

	p = outputqueue;
	j = cur;
	while(1)
	{	
		if(j >= cur+len)
		{
			if(j%0x80 == 0)break;

			k = j - (j%0x80);
			if(k<cur)k=cur;

			lowlevel_output(p+k, j-k);
			break;
		}
		else if(p[j] == '\n')
		{
			k = j - (j%0x80);
			if(k<cur)k=cur;

			lowlevel_output(p+k, j-k+1);
			j = (j+0x80) - (j%0x80);
		}
		else
		{
			if(j%0x80 == 0x7f)
			{
				k = j - (j%0x80);
				if(k<cur)k=cur;

				lowlevel_output(p+k, j-k+1);
			}
			j++;
		}
	}
}




int fmt(char* buf, int len, char* str, ...)
{
	int ret;
	va_list arg;
	va_start(arg, str);
	ret = convert(buf, len, str, 0, arg);
	va_end(arg);
    return ret;
}
void say(char* str, ...)
{
	int cur,ret;
	va_list arg;
	va_start(arg, str);

	//read position
	cur = *outcur;

	//snprintf
	ret = convert(outputqueue+cur, 0x1000, str, cur%0x80, arg);

	//debugport
	printout(cur, ret);

	//write position
	cur = cur+ret;
	if(cur > 0xf0000)cur = 0;
	*outcur = cur;

	va_end(arg);
}




void printbigint(u8* p, int i)
{
        int j;
        if(i<=0)return;

        say("0x");
        for(j=i-1;j>=0;j--)say("%02x",p[j]);
}
void printmemory(u8* addr,int size)
{
	int x,y;
	u8 ch;
	u8* p;
	say("\n");

	for(y=0;y<size/16;y++)
	{
		p = addr+y*16;

		//addr
		if((y%16) == 0)say( "@%-13llx" , (u64)p );
		else say("+%-13x",y*16);

		//hex
		for(x=0;x<=0xf;x++)
		{
			say("%02x ",p[x]);
		}

		//ascii
		for(x=0;x<=0xf;x++)
		{
			ch = p[x];
			if( (ch>=0x7f)|(ch<0x20) )ch=0x20;
			say("%c",ch);
		}
		say("\n");
	}
	if(y*16 >= size)return;

	p = addr+y*16;
	say("+%-13x",y*16);
	for(x=0;x<16;x++)
	{
		if(x >= size%16)say("   ");
		else say("%02x ",p[x]);
	}
	for(x=0;x<16;x++)
	{
		if(x >= size%16)say(" ");
		else
		{
			ch = p[x];
			if( (ch>=0x7f)|(ch<0x20) )ch=0x20;
			say("%c",ch);
		}
	}
	say("\n");
}
