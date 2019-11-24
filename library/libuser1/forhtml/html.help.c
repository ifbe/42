#include "libuser.h"




int htmlprintf(struct entity* win, int j, char* fmt, ...)
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

				sty->f.vl[0] = f;
			}
			else if(0 == ncmp(buf+k, "right:", 6))
			{
				decstr2double(buf+k+6, &f);
				//say("r=%f\n", f);

				sty->f.vr[0] = f;
			}
			else if(0 == ncmp(buf+k, "bottom:", 7))
			{
				decstr2double(buf+k+7, &f);
				//say("b=%f\n", f);

				sty->f.vn[1] = f;
			}
			else if(0 == ncmp(buf+k, "top:", 4))
			{
				decstr2double(buf+k+4, &f);
				//say("t=%f\n", f);

				sty->f.vf[1] = f;
			}

			//
			else if(0 == ncmp(buf+k, "x:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->f.vc[0] = f;
			}
			else if(0 == ncmp(buf+k, "y:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->f.vc[1] = f;
			}
			else if(0 == ncmp(buf+k, "z:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->f.vc[2] = f;
			}
			else if(0 == ncmp(buf+k, "l:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->f.vl[0] = f;
			}
			else if(0 == ncmp(buf+k, "r:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->f.vr[0] = f;
			}
			else if(0 == ncmp(buf+k, "n:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->f.vn[1] = f;
			}
			else if(0 == ncmp(buf+k, "f:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->f.vf[1] = f;
			}
			else if(0 == ncmp(buf+k, "b:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->f.vb[2] = f;
			}
			else if(0 == ncmp(buf+k, "u:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->f.vt[2] = f;
			}
			k = j+1;
		}
	}

	//say("(%f,%f,%f)\n", sty->rx, sty->fy, sty->uz);
}




void printhtmlbody(u8* buf, int len)
{
	int j,k,t;
	int sp,flag;
	int stack[256][2];
	char* space = "                                                           ";

	sp = 0;
	for(j=0;j<len;j++)
	{
		if('<' != buf[j])continue;
		if('!' == buf[j+1])continue;

		t = 0;
		flag = 0;
		for(k=1;k<16;k++)
		{
			if(('/' == buf[j+k])&&('>' == buf[j+k+1]))flag = 1;
			if('>' == buf[j+k])break;
			if(' ' == buf[j+k])
			{
				for(t=k+1;t<len;t++)
				{
					if(('/' == buf[j+t])&&('>' == buf[j+t+1]))flag = 1;
					if('>' == buf[j+t])break;
				}
				break;
			}
		}
		//say("%.*s\n", k-1, buf+j+1);

		if('/' == buf[j+1])
		{
			sp--;
			if(sp < 0)break;
		}
		else
		{
			if(sp>1)say("%.*s", (sp-1)*4, space);
			if(sp>0)say("└───");
			say("%.*s", k-1, buf+j+1);
			if(t)say(" %.*s", t-k-1, buf+j+k+1);
			say("\n");

			if(flag)continue;
			if(0 == ncmp(buf+j+1, "meta", 4))continue;
			if(0 == ncmp(buf+j+1, "br", 2))continue;
			if(0 == ncmp(buf+j+1, "script", 6))
			{
				for(k=j+7;k<len;k++)
				{
					if('<' != buf[k])continue;
					if(0 == ncmp(buf+k, "</script", 8))
					{
						j = k+7;
						break;
					}
				}
				continue;
			}
			stack[sp][0] = j+1;
			stack[sp][1] = k;
			sp++;
		}
	}
}
