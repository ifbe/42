#include "libuser.h"
#define _relation_ hex32('r','e','l',0)




void* systemlist(void*, int);
void* arterylist(void*, int);
void* arenalist(void*, int);
void* actorlist(void*, int);
void* systemchoose(u8* buf, int len);
void* arterychoose(u8* buf, int len);
void* arenachoose(u8* buf, int len);
void* actorchoose(u8* buf, int len);
//
void* allocpinid();
void* allocstyle();
int parsestyle(void*, void*, int);
int parsepinid(void*, void*, int);




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

	int j,k,p,q;
	int a,b,m,n;
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
	if(0 == ncmp(buf+a, "system/", 7))
	{
		p = _fd_;
		a += 7;
		win = systemlist(buf+a, b-a);
		if(0 == win)return;
	}
	else if(0 == ncmp(buf+a, "artery/", 7))
	{
		p = _art_;
		a += 7;
		win = arterylist(buf+a, b-a);
		if(0 == win)return;
	}
	else if(0 == ncmp(buf+a, "arena/", 6))
	{
		p = _win_;
		a += 6;
		win = arenalist(buf+a, b-a);
		if(0 == win)return;
	}
	else if(0 == ncmp(buf+a, "actor/", 6))
	{
		p = _act_;
		a += 6;
		win = actorlist(buf+a, b-a);
		if(0 == win)return;
	}
	else return;

	if(0 == ncmp(buf+m, "system/", 7))
	{
		q = _fd_;
		m += 7;
		act = systemlist(buf+m, n-m);
		if(0 == act)return;
	}
	else if(0 == ncmp(buf+m, "artery/", 7))
	{
		q = _art_;
		m += 7;
		act = arterylist(buf+m, n-m);
		if(0 == act)return;
	}
	else if(0 == ncmp(buf+m, "arena/", 6))
	{
		q = _win_;
		m += 6;
		act = arenalist(buf+m, n-m);
		if(0 == act)return;
	}
	else if(0 == ncmp(buf+m, "actor/", 6))
	{
		q = _act_;
		m += 6;
		act = actorlist(buf+m, n-m);
		if(0 == act)return;
	}
	else return;

	if((_win_ == p)&&(_act_ == q))
	{
		css = allocstyle();
		pin = allocpinid();

		parsestyle(css, buf+a, b-a);
		parsepinid(pin, buf+m, n-m);

		act->oncreate(act, 0);
		act->onstart(win, css, act, pin);
		relationcreate(win, css, _win_, act, pin, _act_);
	}
	else
	{
		relationcreate(win, 0, p, act, 0, q);
	}
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
						//say("relation");
						say("%.*s\n", k+1-sb[sp], buf+sb[sp]);
					}
					else if(_fd_ == ret)
					{
						//say("system");
						systemchoose(buf+sb[sp], k+1-sb[sp]);
					}
					else if(_art_ == ret)
					{
						//say("artery");
						arterychoose(buf+sb[sp], k+1-sb[sp]);
					}
					else if(_win_ == ret)
					{
						//say("arena");
						arenachoose(buf+sb[sp], k+1-sb[sp]);
					}
					else if(_act_ == ret)
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
					else if(0 == ncmp(buf+j+1, "system", 6))ret = _fd_;
					else if(0 == ncmp(buf+j+1, "artery", 6))ret = _art_;
					else if(0 == ncmp(buf+j+1, "arena", 5))ret = _win_;
					else if(0 == ncmp(buf+j+1, "actor", 5))ret = _act_;
				}
			}

			//say("(");
			//for(t=0;t<sp;t++)say("%.*s ", sl[t], buf+sb[t]);
			//say(")\n");
			j = k;
		}
	}
}
