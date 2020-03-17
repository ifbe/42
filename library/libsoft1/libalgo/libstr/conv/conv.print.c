#define s8 char
#define s16 short
#define s32 int
#define s64 long long
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int data2decstr(s64 data, void* str);
int data2hexstr(u64 data, void* str);
int double2decstr(double data, void* str);




int myvsnprintf(u8* buf, int len, u8* fmt, __builtin_va_list arg)
{
	int j, k;
	int src, dst, tmp;
	int flag1, flag2, lval, rval;	//%-08.7s  %08llx
	s64 _d;
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
			lval = __builtin_va_arg(arg, int);
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
		if(fmt[tmp] == '*')
		{
			rval = __builtin_va_arg(arg, int);
			tmp++;
		}
		else if( (fmt[tmp] < 0x30) | (fmt[tmp] > 0x39) )
		{
			rval = -1000;
		}
		else
		{
			rval = fmt[tmp]-0x30;
			tmp++;

			while( (fmt[tmp] >= 0x30) && (fmt[tmp] <= 0x39) )
			{
				rval = (rval*10) + (fmt[tmp]-0x30);
				tmp++;
			}
		}

		//type
		if(fmt[tmp] == 'f')
		{
			_f = __builtin_va_arg(arg, double);
			dst += double2decstr(_f, buf+dst);

			src = tmp+1;
			continue;
		}
		else if(fmt[tmp] == 'c')
		{
			_x = __builtin_va_arg(arg, int);
			buf[dst] = _x;
			dst++;

			src = tmp+1;
			continue;
		}
		else if(fmt[tmp] == 's')
		{
			_s = __builtin_va_arg(arg, u8*);
			if(rval < 0)
			{
				while(1)
				{
					if(dst >= len)goto retlen;
					if(*_s == 0)break;

					buf[dst] = *_s;
					dst++;
					_s++;
				}
			}
			else
			{
				for(j=0;j<rval;j++)
				{
					if(_s[j] == 0)break;
					buf[dst+j] = _s[j];
				}
				dst += j;
			}
			src = tmp+1;
			continue;
		}
		else if(fmt[tmp] == 'd')
		{
			_d = __builtin_va_arg(arg, int);
			j = data2decstr(_d, buf+dst);
			if(lval == 0)dst += j;
			else if(j == lval)dst += j;
			else if(j > lval)
			{
				for(k=0;k<lval;k++)buf[dst+k] = buf[dst+k+j-lval];
				dst += lval;
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
					for(k=1;k<=j;k++)buf[dst+lval-k] = buf[dst+j-k];
					for(k=0;k<lval-j;k++)buf[dst+k] = flag2;
				}
				dst += lval;
			}

			src = tmp+1;
			continue;
		}
		else if(fmt[tmp] == 'x')
		{
			_x = __builtin_va_arg(arg, u32);

			j = data2hexstr(_x, buf+dst);
			if(lval == 0)dst += j;
			else if(j == lval)dst += j;
			else if(j > lval)
			{
				for(k=0;k<lval;k++)buf[dst+k] = buf[dst+k+j-lval];
				dst += lval;
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
					for(k=1;k<=j;k++)buf[dst+lval-k] = buf[dst+j-k];
					for(k=0;k<lval-j;k++)buf[dst+k] = flag2;
				}
				dst += lval;
			}

			src = tmp+1;
			continue;
		}
		else if((fmt[tmp] == 'l')&&(fmt[tmp+1] == 'l')&&(fmt[tmp+2] == 'x'))
		{
			_x = __builtin_va_arg(arg, u64);

			j = data2hexstr(_x, buf+dst);
			if(lval == 0)dst += j;
			else if(j == lval)dst += j;
			else if(j > lval)
			{
				for(k=0;k<lval;k++)buf[dst+k] = buf[dst+k+j-lval];
				dst += lval;
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
					for(k=1;k<=j;k++)buf[dst+lval-k] = buf[dst+j-k];
					for(k=0;k<lval-j;k++)buf[dst+k] = flag2;
				}
				dst += lval;
			}

			src = tmp+3;
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
	__builtin_va_list arg;

	__builtin_va_start(arg, fmt);
	ret = myvsnprintf(buf, len, fmt, arg);
	__builtin_va_end(arg);

	return ret;
}
