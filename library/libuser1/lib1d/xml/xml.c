#include "actor.h"
#define _arena_ hex32('w','i','n',0)
#define _actor_ hex32('a','c','t',0)
#define _relation_ hex32('r','e','l',0)
void* allocpinid();
void* allocstyle();
void* actorlist(void*, int);
void* arenalist(void*, int);
void* actorchoose(u8* buf, int len);
void* arenachoose(u8* buf, int len);
void* actorcreate(void*, u8*);
void* actordelete(void*, u8*);
//
int parsestyle(void*, void*, int);
int parsepinid(void*, void*, int);
void* relation_read(u64);
void* relation_write(void*, void*, u64, void*, void*, u64);




void parsexml_detail(
	u8* buf, int len,
	u64* name, int* id,
	u8** data, int* dl)
{
	u8* p;
	int j,k,x,y;

	if('<' != buf[0])return;
	x = y = -1;
	for(j=1;j<len;j++)
	{
		if('>' == buf[j])
		{
			if(y < 0)y = j;
			break;
		}
		if(('@' == buf[j])&&(x < 0))x = j;
		if((0x20 >= buf[j])&&(y < 0))y = j;
	}
	*data = buf+j+1;

	for(k = j+1;k<len;k++)
	{
		if('<' == buf[k])break;
	}
	*dl = k-j-1;

	if(x > 0)
	{
		*id = buf[x+1]-0x30;
		k = x;
	}
	else k = y;

	*name = 0;
	p = (u8*)name;
	for(j=1;j<k;j++)
	{
		*p = buf[j];
		p++;
	}
}
void parsexml_relation(u8* buf, int len)
{
	//say("%.*s\n", len, buf);

	int j,k,a,b,m,n;
	struct arena* win;
	struct actor* act;
	u8* css;
	u8* pin;

	for(j=0;j<len;j++)
	{
		if('>' == buf[j])
		{
			b = j;
			j++;
			break;
		}
		if('<' == buf[j])
		{
			a = j+1;
			if('$' == buf[a])a++;
		}
	}
	say("%.*s\n", b-a, buf+a);

	for(;j<len;j++)
	{
		if('>' == buf[j])
		{
			n = j;
			j++;
			break;
		}
		if('<' == buf[j])
		{
			m = j+1;
			if('$' == buf[m])m++;
		}
	}
	say("%.*s\n", n-m, buf+m);

	//find
	win = arenalist(buf+a, b-a);
	if(0 == win)return;
	act = actorlist(buf+m, n-m);
	if(0 == act)return;

	//parse
	css = allocstyle();
	pin = allocpinid();
	parsestyle(css, buf+a, b-a);
	parsepinid(pin, buf+m, n-m);

	//rel
	//say("%llx,%llx,%llx,%llx\n", win, css, act, pin);
	//win->onstart(win, cs);
	act->onstart(act, pin);
	relation_write(win, css, _win_, act, pin, _act_);
}
void parsexml(u8* buf, int len)
{
	int j,k,m,n;
	int t,sp,ret;
	int sb[16];
	int sl[16];

	sp = ret = 0;
	for(j=0;j<len;j++)
	{
		if('<' == buf[j])
		{
			m = n = -1;
			for(k=j+1;k<len;k++)
			{
				if('>' == buf[k])
				{
					if(m < 0)m = k;
					if(n < 0)n = k;
					break;
				}
				if(buf[k] <= 0x20)
				{
					if(n < 0)n = k;
				}
			}
			//say("%.*s\n", k-j+1, buf+j);

			if('/' == buf[k-1])
			{
			}
			else if('/' == buf[j+1])
			{
				sp--;
				if(0 < sp)
				{
					if(_relation_ == ret)
					{
						say("%.*s\n", k+1-sb[sp], buf+sb[sp]);
					}
					else if(_arena_ == ret)
					{
						//say("arena");
						arenachoose(buf+sb[sp], k+1-sb[sp]);
					}
					else if(_actor_ == ret)
					{
						//say("actor");
						actorchoose(buf+sb[sp], k+1-sb[sp]);
					}
				}
			}
			else if('$' == buf[j+1])
			{
				for(k=k+1;k<len;k++)
				{
					if('>' == buf[k])break;
				}
				parsexml_relation(buf+j, k-j+1);
			}
			else
			{
				sb[sp] = j;
				sl[sp] = k-j;
				sp++;

				if(1 == sp)
				{
					if(0 == ncmp(buf+j+1, "relation", 8))ret = _relation_;
					else if(0 == ncmp(buf+j+1, "arena", 5))ret = _arena_;
					else if(0 == ncmp(buf+j+1, "actor", 5))ret = _actor_;
				}
			}

			//say("(");
			//for(t=0;t<sp;t++)say("%.*s ", sl[t], buf+sb[t]);
			//say(")\n");
			j = k;
		}
	}
}
