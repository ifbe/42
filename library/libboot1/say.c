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




u64 fixdata(u64 data, int val)
{
	u64 mask;
	if(val == 0)return data;

	mask = 0;
	while(val > 0)
	{
		mask = (mask<<8) | 0xff;
		val--;
	}
	return data&mask;
}
int myvsnprintf(u8* buf, int len, u8* fmt, va_list arg)
{
	int j, k;
	int src, dst, tmp;
	int flag1, flag2, lval, rval;	//%-08.7s  %08llx
	u64 _x;
    u8* _s;
    double _f;

	src = dst = 0;
	while(1)
	{
		if(dst >= len)goto retlen;
		if(fmt[src] == 0)goto retdst;
		if(fmt[src] != '%')
		{
			buf[dst] = fmt[src];

			src++;
			dst++;
			continue;
		}

		//%%
		tmp = src+1;
		if(fmt[tmp] == '%')
		{
			buf[dst] = '%';

			src += 2;
			dst++;
			continue;
		}

		//flag1: '-', '+', '#'
		flag1 = fmt[tmp];
		if( (flag1=='-') | (flag1=='+') | (flag1=='#') )tmp++;
		else flag1 = 0;

		//flag2: '0'
		flag2 = fmt[tmp];
		if(flag2 == '0')tmp++;
		else flag2 = 0;

		//lval
		lval = 0;
		if(fmt[tmp] == '*')
		{
			lval = va_arg(arg, int);
			tmp++;
		}
		else
		{
			while( (fmt[tmp] >= 0x30) && (fmt[tmp] <= 0x39) )
			{
				lval = (lval*10) + (fmt[tmp]-0x30);
				tmp++;
			}
		}

		//'.'
		if(fmt[tmp] == '.')tmp++;

		//rval
		rval = 0;
		if(fmt[tmp] == '*')
		{
			rval = va_arg(arg, int);
			tmp++;
		}
		else
		{
			while( (fmt[tmp] >= 0x30) && (fmt[tmp] <= 0x39) )
			{
				rval = (rval*10) + (fmt[tmp]-0x30);
				tmp++;
			}
		}

		//type
		if(fmt[tmp] == 'd')
		{
			_x = va_arg(arg, u64);
			dst += data2decstr(_x, buf+dst);

			src = tmp+1;
			continue;
		}
		else if(fmt[tmp] == 'f')
		{
			_f = va_arg(arg, double);
			dst += double2decstr(_f, buf+dst);

			src = tmp+1;
			continue;
		}
		else if(fmt[tmp] == 'x')
		{
			_x = va_arg(arg, u64);

			j = data2hexstr(_x, buf+dst);
			if(j >= lval)
			{
				dst += j;
			}
			else
			{
				if(flag1 == '-')
				{
					for(;j<lval;j++)buf[dst+j] = 0x20;
				}
				else
				{
					if(flag2 != '0')flag2 = 0x20;
					for(k=1;k<=lval-j;k++)buf[dst+lval-k] = buf[dst+j-k];
					for(k=0;k<lval-j;k++)buf[dst+k] = flag2;
				}
				dst += lval;
			}

			src = tmp+1;
			continue;
		}
		else if(fmt[tmp] == 'c')
		{
			_x = va_arg(arg, int);
			buf[dst] = _x;
			dst++;

			src = tmp+1;
			continue;
		}
		else if(fmt[tmp] == 's')
		{
			_s = va_arg(arg, char*);
			while(1)
			{
				if(dst >= len)goto retlen;
				if(*_s == 0)break;

				buf[dst] = *_s;
				dst++;
				_s++;
			}
			src = tmp+1;
			continue;
		}
		else
		{
			while(1)
			{
				if(dst >= len)goto retlen;
				if(src > tmp)break;

				buf[dst] = fmt[src];
				dst++;
				src++;
			}
		}
	}
retdst:
	buf[dst] = 0;
	return dst;
retlen:
	buf[len] = 0;
	return len;
}




int mysnprintf(u8* buf, int len, u8* fmt, ...)
{
	int ret;
	va_list arg;
	va_start(arg, fmt);
	ret = myvsnprintf(buf, len, fmt, arg);
	va_end(arg);
    return ret;
}
void say(u8* fmt, ...)
{
	int cur,ret;
	va_list arg;

	//read position
	cur = *outcur;

	//
	va_start(arg, fmt);

	//snprintf
	//ret = diary(outputqueue+cur, 0x1000, fmt, arg, cur%0x80);
	ret = myvsnprintf(outputqueue+cur, 0x1000, fmt, arg);

	//
	va_end(arg);

	//debugport
	//printout(cur, ret);
	lowlevel_output(outputqueue+cur, ret);

	//write position
	cur = cur+ret;
	if(cur > 0xf0000)cur = 0;
	*outcur = cur;
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
		if((y%16) == 0)say( "@%-13x" , (u64)p );
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
