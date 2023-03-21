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
int float2decstr(float data, void* str);
int double2decstr(double data, void* str);




//this func can handle \b and \t
//pre = line head addr
//buf = curr pos addr
int termprintf(u8* pre, int cnt, u8* buf, int len, u8* fmt, __builtin_va_list arg)
{
	return 0;
}




int myvsnprintf(u8* buf, int len, u8* fmt, __builtin_va_list arg)
{
	int j, k;
	int src, dst, tmp;
	int aflag, zflag, lval, rval, size;	//%-08.7s  %08llx
	u8* _s;

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

		//aflag: '-', '+', '#'
		aflag = fmt[tmp];
		if( (aflag=='-') | (aflag=='+') | (aflag=='#') )tmp++;
		else aflag = 0;

		//zflag: '0'
		zflag = fmt[tmp];
		if(zflag == '0')tmp++;
		else zflag = 0;

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

		//ll
		if('l' == fmt[tmp]){
			if('l' == fmt[tmp+1]){
				size = 2;
				tmp += 2;
			}
			else{
				size = 1;
				tmp += 1;
			}
		}
		else size = 0;

		//type
		if(fmt[tmp] == 'c')
		{
			buf[dst] = __builtin_va_arg(arg, int);	//gcc: use int not char
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
		else if(
			('f' == fmt[tmp]) |
			('d' == fmt[tmp]) |
			('x' == fmt[tmp]) |
			('p' == fmt[tmp]) )
		{
			if('f' == fmt[tmp]){
				if(size > 0){
					j = float2decstr(__builtin_va_arg(arg, double), buf+dst);	//gcc: use double not float
				}
				else{
					j = double2decstr(__builtin_va_arg(arg, double), buf+dst);
				}
			}
			else if('d' == fmt[tmp]){
				if(2 == size){
					j = data2decstr(__builtin_va_arg(arg, s64), buf+dst);
				}
				else{
					j = data2decstr(__builtin_va_arg(arg, int), buf+dst);
				}
			}
			else if('x' == fmt[tmp]){
				if(2 == size){
					j = data2hexstr(__builtin_va_arg(arg, u64), buf+dst);
				}
				else{
					j = data2hexstr(__builtin_va_arg(arg, u32), buf+dst);
				}
			}
			else if('p' == fmt[tmp]){
				_s = __builtin_va_arg(arg, u8*);
				j = data2hexstr((u64)_s, buf+dst);
			}

			if(lval == 0)dst += j;
			else if(j == lval)dst += j;
			else if(j > lval)
			{
				for(k=0;k<lval;k++)buf[dst+k] = buf[dst+k+j-lval];
				dst += lval;
			}
			else
			{
				if(aflag == '-')
				{
					for(;j<lval;j++)buf[dst+j] = 0x20;
				}
				else
				{
					if(zflag != '0')zflag = 0x20;
					for(k=1;k<=j;k++)buf[dst+lval-k] = buf[dst+j-k];
					for(k=0;k<lval-j;k++)buf[dst+k] = zflag;
				}
				dst += lval;
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
	__builtin_va_list arg;

	__builtin_va_start(arg, fmt);
	ret = myvsnprintf(buf, len, fmt, arg);
	__builtin_va_end(arg);

	return ret;
}
