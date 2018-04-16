#include "actor.h"




int htmlprintf(struct arena* win, int i, char* fmt, ...)
{
	__builtin_va_list arg;
	struct htmlpiece* hp;
	if(i<1)return 0;
	if(i>2)return 0;

	__builtin_va_start(arg, fmt);

	hp = win->hp;
	hp[i].len += myvsnprintf(
		hp[i].buf+hp[i].len, 0x100000-hp[i].len,
		fmt, arg
	);

	__builtin_va_end(arg);
	return 0;
}




void parsepinid(u8* pin, u8* buf, int len)
{
	int j;
	for(j=0;j<len;j++)
	{
		if('\"' == buf[j])
		{
			buf += j+1;
			len -= j+1;
		}
	}
	for(j=0;j<len;j++)
	{
		if('\"' == buf[j])break;
		pin[j] = buf[j];
	}
}
void parsestyle(struct style* sty, u8* buf, int len)
{
	double f;
	int j,k=0;
	for(j=0;j<len;j++)
	{
		if('\"' == buf[j])
		{
			buf += j+1;
			len -= j-1;
			break;
		}
	}
	//say("%.*s\n",len,buf);

	for(j=0;j<len;j++)
	{
		if(0 == buf[j])break;
		if(';' == buf[j])
		{
			if(0 == ncmp(buf+k, "width:", 6))
			{
				decstr2double(buf+k+6, &f);
				//say("w=%f\n", f);
			}
			else if(0 == ncmp(buf+k, "height:", 7))
			{
				decstr2double(buf+k+7, &f);
				//say("h=%f\n", f);
			}
			else if(0 == ncmp(buf+k, "left:", 5))
			{
				decstr2double(buf+k+5, &f);
				//say("l=%f\n", f);

				sty->lx = f;
			}
			else if(0 == ncmp(buf+k, "right:", 6))
			{
				decstr2double(buf+k+6, &f);
				//say("r=%f\n", f);

				sty->rx = f;
			}
			else if(0 == ncmp(buf+k, "bottom:", 7))
			{
				decstr2double(buf+k+7, &f);
				//say("b=%f\n", f);

				sty->ny = f;
			}
			else if(0 == ncmp(buf+k, "top:", 4))
			{
				decstr2double(buf+k+4, &f);
				//say("t=%f\n", f);

				sty->fy = f;
			}

			//
			else if(0 == ncmp(buf+k, "x:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->cx = f;
			}
			else if(0 == ncmp(buf+k, "y:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->cy = f;
			}
			else if(0 == ncmp(buf+k, "z:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->cz = f;
			}
			else if(0 == ncmp(buf+k, "l:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->lx = f;
			}
			else if(0 == ncmp(buf+k, "r:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->rx = f;
			}
			else if(0 == ncmp(buf+k, "n:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->ny = f;
			}
			else if(0 == ncmp(buf+k, "f:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fy = f;
			}
			else if(0 == ncmp(buf+k, "b:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->bz = f;
			}
			else if(0 == ncmp(buf+k, "u:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->uz = f;
			}
			k = j+1;
		}
	}

	say("(%f,%f,%f)\n", sty->rx, sty->fy, sty->uz);
}




void parsehtml(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<=len;j++)
	{
		if((j == len)|(0xa == buf[j])|(0xd == buf[j]))
		{
			say("[%.*s]\n", j-k, buf+k);

			if(0xd == buf[j])j++;
			k = j+1;
		}
	}
}
