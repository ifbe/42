#include "libuser.h"




int htmlprintf(_obj* win, int j, char* fmt, ...)
{
	__builtin_va_list arg;
	struct str** ctx;
	void* buf;
	int len;
	if(j<1)return 0;
	if(j>2)return 0;

	__builtin_va_start(arg, fmt);

	ctx = win->html.ctx;
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
	//logtoall("%.*s\n",len,buf);

	for(j=0;j<len;j++)
	{
		if(0 == buf[j])break;
		if(';' == buf[j])
		{
			if(0 == ncmp(buf+k, "width:", 6))
			{
				decstr2double(buf+k+6, &f);
				//logtoall("w=%f\n", f);
			}
			else if(0 == ncmp(buf+k, "height:", 7))
			{
				decstr2double(buf+k+7, &f);
				//logtoall("h=%f\n", f);
			}
			else if(0 == ncmp(buf+k, "left:", 5))
			{
				decstr2double(buf+k+5, &f);
				//logtoall("l=%f\n", f);

				sty->fs.vl[0] = f;
			}
			else if(0 == ncmp(buf+k, "right:", 6))
			{
				decstr2double(buf+k+6, &f);
				//logtoall("r=%f\n", f);

				sty->fs.vr[0] = f;
			}
			else if(0 == ncmp(buf+k, "bottom:", 7))
			{
				decstr2double(buf+k+7, &f);
				//logtoall("b=%f\n", f);

				sty->fs.vn[1] = f;
			}
			else if(0 == ncmp(buf+k, "top:", 4))
			{
				decstr2double(buf+k+4, &f);
				//logtoall("t=%f\n", f);

				sty->fs.vf[1] = f;
			}

			//
			else if(0 == ncmp(buf+k, "x:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fs.vc[0] = f;
			}
			else if(0 == ncmp(buf+k, "y:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fs.vc[1] = f;
			}
			else if(0 == ncmp(buf+k, "z:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fs.vc[2] = f;
			}
			else if(0 == ncmp(buf+k, "l:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fs.vl[0] = f;
			}
			else if(0 == ncmp(buf+k, "r:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fs.vr[0] = f;
			}
			else if(0 == ncmp(buf+k, "n:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fs.vn[1] = f;
			}
			else if(0 == ncmp(buf+k, "f:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fs.vf[1] = f;
			}
			else if(0 == ncmp(buf+k, "b:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fs.vb[2] = f;
			}
			else if(0 == ncmp(buf+k, "u:", 2))
			{
				decstr2double(buf+k+2, &f);
				sty->fs.vt[2] = f;
			}
			k = j+1;
		}
	}

	//logtoall("(%f,%f,%f)\n", sty->rx, sty->fy, sty->uz);
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
		//logtoall("%.*s\n", k-1, buf+j+1);

		if('/' == buf[j+1])
		{
			sp--;
			if(sp < 0)break;
		}
		else
		{
			if(sp>1)logtoall("%.*s", (sp-1)*4, space);
			if(sp>0)logtoall("└───");
			logtoall("%.*s", k-1, buf+j+1);
			if(t)logtoall(" %.*s", t-k-1, buf+j+k+1);
			logtoall("\n");

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
