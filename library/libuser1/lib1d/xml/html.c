#include "libuser.h"




int htmlprintf(struct arena* win, int j, char* fmt, ...)
{
	__builtin_va_list arg;
	struct str** ctx;
	void* buf;
	int len;
	if(j<1)return 0;
	if(j>2)return 0;

	__builtin_va_start(arg, fmt);

	ctx = win->ctx;
	len = ctx[j]->len;
	buf = ctx[j]->buf;
	ctx[j]->len += myvsnprintf(buf+len, 0xffff8-len, fmt, arg);

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

				sty->vl[0] = f;
			}
			else if(0 == ncmp(buf+k, "right:", 6))
			{
				decstr2double(buf+k+6, &f);
				//say("r=%f\n", f);

				sty->vr[0] = f;
			}
			else if(0 == ncmp(buf+k, "bottom:", 7))
			{
				decstr2double(buf+k+7, &f);
				//say("b=%f\n", f);

				sty->vn[1] = f;
			}
			else if(0 == ncmp(buf+k, "top:", 4))
			{
				decstr2double(buf+k+4, &f);
				//say("t=%f\n", f);

				sty->vf[1] = f;
			}

			//
			else if(0 == ncmp(buf+k, "x:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->vc[0] = f;
			}
			else if(0 == ncmp(buf+k, "y:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->vc[1] = f;
			}
			else if(0 == ncmp(buf+k, "z:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->vc[2] = f;
			}
			else if(0 == ncmp(buf+k, "l:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->vl[0] = f;
			}
			else if(0 == ncmp(buf+k, "r:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->vr[0] = f;
			}
			else if(0 == ncmp(buf+k, "n:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->vn[1] = f;
			}
			else if(0 == ncmp(buf+k, "f:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->vf[1] = f;
			}
			else if(0 == ncmp(buf+k, "b:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->vb[2] = f;
			}
			else if(0 == ncmp(buf+k, "u:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->vu[2] = f;
			}
			k = j+1;
		}
	}

	//say("(%f,%f,%f)\n", sty->rx, sty->fy, sty->uz);
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
