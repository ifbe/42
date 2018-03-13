#include "actor.h"
#define _arena_ hex32('w','i','n',0)
#define _actor_ hex32('a','c','t',0)
#define _relation_ hex32('r','e','l',0)
int arenachoose(u8* buf, int len);
int actorchoose(u8* buf, int len);




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
			m = -1;
			for(k=j+1;k<len;k++)
			{
				if('>' == buf[k])
				{
					if(m < 0)m = k;
					break;
				}
				if(buf[k] <= 0x20)
				{
					if(m < 0)m = k;
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